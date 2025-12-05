(* Device *)

open Int32_syntax;;

class type device =
  object
    method block_size : int
    method get : int -> string
    method put : int -> string -> unit
  end
;;

(*** block_device *)
class block_device ?(big=false) ?(block_size=512) ~fn () : device =
  let fd = Unix.openfile fn [O_RDWR;] 0o644 in
  let offset b = block_size * b in
  object(self)
    method block_size = block_size

    method put block data =
      Talk.debug "Put %d -> %d" block (offset block);
      Unix.lseek fd (offset block) Unix.SEEK_SET;
      let rec write i =
        XXX
      Unix.write
      output_string oc data;
      flush oc

    method get block =
      let u = String.create block_size in
      Unix.lseek fd (offset block) Unix.SEEK_SET;
      really_input ic u 0 block_size;
      u
  end
;;
(* ***)
