(* Hexdump*)

open Pffpsf;;

let hexdump oc a =
  let m = Array.length a in
  for i = 0 to m - 1 do
    if i land 3 = 0 then fp oc "%08x:" (4 * i);
    fp oc " %08lx" a.(i);
    if i = m - 1 or i land 3 = 3 then fp oc "\n"
  done
;;
