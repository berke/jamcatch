(* Sfs *)
(* Simple file system *)

(* 128 words *)
(* 124 plaintext *)
(* Group overhead : start (1) + length (0.5) + free (0.5) = 2 *)
(* 122 words * 32 = 3904 *)

open Pffpsf;;
open Int32_syntax;;

type index = int;;

(*** Bitmap *)
module Bitmap =
  struct
    type t = int32 array;;

    let to_int32_array b = b;;
    let of_int32_array b = b;;

    let create m =
      let n = (m + 31) / 32 in
      Array.make n 0l
    ;;

    let set b i = b.(i lsr 5) <- b.(i lsr 5) ||| (1l <<< (i land 31));;

    let clr b i = b.(i lsr 5) <- b.(i lsr 5) &&& ~~~(1l <<< (i land 31));;

    let get b i = 0l <> (b.(i lsr 5) &&& (1l <<< (i land 31)));;

    let print oc b =
      let m = 32 * Array.length b in
      for i = 0 to m - 1 do
        if get b i then fp oc "1" else fp oc "0"
      done
    ;;
  end
;;
(* ***)

type group =
  {
    gr_parent : index;     (* The parent block *)
    gr_start : index;      (* The start block of this group *)
    gr_length : int;       (* The number of blocks in this group -- usually maximum *)
    mutable gr_free : int; (* The number of blocks in this group that are either free (if they are data blocks),
                            * or contain at least one free block. *)
    gr_bitmap : Bitmap.t;  (* The bitmap; true means has free. *)
  }
;;

type data =
  {
    dt_parent : index;
    dt_data : int32 array
  }
;;

type block =
  | Data of data
  | Group of group
;;

let group_tag = 0x22222222l;;
let data_tag  = 0xddddddddl;;

(*** print_block *)
let print_block oc = function
  | Data d -> fp oc "Data{parent=%d, data:%d words}" d.dt_parent (Array.length d.dt_data)
  | Group g ->
      fp oc "Group{parent=%d, start=%d, length=%d, free=%d, bitmap=%a}"
        g.gr_parent
        g.gr_start
        g.gr_length
        g.gr_free
        Bitmap.print
        g.gr_bitmap
;;
(* ***)
(*** get_block *)
let get_block device block =
  let a = device#get block in
  let f i = ??? (a.(i)) in
  match a.(0) with
  | x when x = data_tag ->
      Data{dt_parent = f 1;
           dt_data = Array.sub a 2 (Array.length a - 2)}
  | x when x = group_tag ->
      Group{
        gr_parent = f 1;
        gr_start = f 2;
        gr_length = f 3;
        gr_free = f 4;
        gr_bitmap = Bitmap.of_int32_array (Array.sub a 5 (Array.length a - 5))
      }
  | x -> failwith (sf "Invalid block tag 0x%08lx in block %d" x block)
;;
(* ***)
(*** put_block *)
let put_block device block = function
  | Data d ->
      device#put block (Array.concat [ [|data_tag; !!! (d.dt_parent)|]; d.dt_data ])
  | Group g ->
      device#put block
        (Array.concat
          [
            [|
              group_tag;
              !!! (g.gr_parent);
              !!! (g.gr_start);
              !!! (g.gr_length);
              !!! (g.gr_free);
            |];
            Bitmap.to_int32_array g.gr_bitmap
          ])
;;
(* ***)
(*** allocate_block *)
let allocate_block ?(root=0) device =
  let rec alloc b =
    Talk.debug "Alloc %d" b;
    match get_block device b with
    | Data d ->
        Talk.debug "Found free block at %d" b;
        b
    | (Group g) as bk ->
        if g.gr_free = 0 then
          raise Not_found
        else
          let rec loop i =
            if i = g.gr_length then
              begin
                Talk.debug "Bitmap free count wrong";
                raise Not_found
              end
            else
              if Bitmap.get g.gr_bitmap i then
                begin
                  Bitmap.clr g.gr_bitmap i;
                  g.gr_free <- g.gr_free - 1;
                  put_block device b bk;
                  alloc (g.gr_start + i)
                end
              else
                loop (i + 1)
          in
          loop 0
  in
  alloc root
;;
(* ***)
(*** format_device *)
let format_device device start blocks =
  let block_size = device#block_size in
  let group_overhead = 5 * 4 in
  let table_entries = (block_size - group_overhead) * 8 in

  let rec make_group ~parent b_start b_end =
    Talk.debug "Group %d %d %d" parent b_start b_end;
    (* First block will be a group block *)
    let m = b_end - b_start in (* Number of terminal entries in this group *)
    (* Recursion *)
    let g =
      let group_size = m / table_entries in
      if m < table_entries then
        begin
          let num_entries = b_end - b_start in
          let bm = Bitmap.create num_entries in
          for e = 0 to num_entries - 1 do
            Bitmap.set bm e;
            make_free ~parent:b_start (b_start + 1 + e)
          done;
          Group{
            gr_parent = parent;
            gr_start = b_start + 1;
            gr_length = num_entries; (* Excluding this block *)
            gr_free = num_entries;
            gr_bitmap = bm;
          }
        end
      else
        begin
          let num_entries = table_entries in
          let bm = Bitmap.create num_entries in
          (* Too big for a group. *)
          for e = 0 to num_entries - 1 do
            let b_start' = b_start + 1 + (e * group_size) in
            let b_end' = min b_end (b_start' + group_size - 1) in
            Bitmap.set bm e;
            make_group ~parent:b_start b_start' b_end'
          done;
          Group{
            gr_parent = parent;
            gr_start = b_start + 1;
            gr_length = num_entries; (* Excluding this block *)
            gr_free = num_entries;
            gr_bitmap = bm;
          }
        end
    in
    put_block device b_start g;
  and make_free ~parent b =
    Talk.debug "Make_free %d %d" parent b;
    put_block device b (Data{dt_parent = parent; dt_data = [||]})
  in
  make_group ~parent:start start (start + blocks - 1)
;;
(* ***)
