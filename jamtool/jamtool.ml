(* Jamtool *)

open Talk;;
open Sfs;;

let ( & ) f x = f x;;

module Opt =
  struct
    open Arg;;

    let sao x y = x := Some y;;
    let device = ref None;;

    let key = [|
      0xd4811664l; 0xbdc350fbl; 0xd364d2f5l; 0x66a04a6dl;
      0xc7b393c5l; 0xc5e0af4bl; 0x23b99f05l; 0x5d9e7b5el
    |];;

    let format_device = ref false;;

    let size = ref 1024;;

    let actions :
      [`Format
      |`Show of int
      |`Show_cipher of int
      |`Show_sfs of int
      |`Allocate of int] list ref = ref []
    ;;

    let (+=) l x = l := x :: !l;;

    let spec =
      align [
        "-device",        String(sao device), "<device> set MMC device";
        "-size",          Set_int size,       "<blocks> set device size";

        "-format",        Unit(fun () -> actions += `Format),  " format device";
        "-show",          Int(fun x ->   actions += `Show x),  "<block> show block content";
        "-show-cipher",   Int(fun x ->   actions += `Show_cipher x),  "<block> show encrypted block content";
        "-show-sfs",      Int(fun x ->   actions += `Show_sfs x),  "<block> show SFS block";
        "-alloc",         Int(fun x ->   actions += `Allocate x),  "<blocks> allocate blocks"
      ]
  end
;;

(*** mandatory *)
let mandatory ?otherwise x =
  match x with
  | Some y -> y
  | None ->
      match otherwise with
      | None -> raise Not_found
      | Some g -> g ()
;;
(* ***)

let _ =
  Arg.parse Opt.spec (fun x -> Printf.eprintf "Extraneous argument %S ignored.\n%!" x) 
    "Stuff.\n";
  let device_fn = mandatory ~otherwise:(fun () -> error "No device specified") !Opt.device in
  let bd = new Device.block_device ~fn:device_fn () in
  let ed = new Encrypted.encrypted_device ~key:Opt.key ~device:bd () in
  List.iter
    begin function
      | `Show_sfs b ->
          let bk = Sfs.get_block ed b in
          info "Block:\n%a\n" Sfs.print_block bk
      | `Show_cipher b ->
          info "Reading encrypted block %d" b;
          let block = bd#get b in
          Hexdump.hexdump stdout (Data.int32_array_of_string block)
      | `Show b ->
          info "Reading block %d" b;
          let block = ed#get b in
          Hexdump.hexdump stdout block
      | `Format ->
          info "Formatting device";
          Sfs.format_device ed 0 !Opt.size
      | `Allocate n ->
          info "Allocating %d blocks" n;
          for i = 1 to n do
            let b = Sfs.allocate_block ed in
            info "  #%d" b
          done
    end
    !Opt.actions
;;
