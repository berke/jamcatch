(* Trigtable *)

let pf = Printf.printf;;

let pi = 4.0 *. atan 1.0;;

let sintable m =
  pf "\
/* dac.c
 *
 * Copyright (C)2007 Therodox R&D
 * All rights reserved
 */

#include \"types.h\"

";
  pf "const s16 sinus[%d] = {\n" m;
  for i = 0 to m - 1 do
    let t = 2.0 *. pi *. (float i) /. (float (m - 1)) in
    let x = sin t in
    let d = int_of_float (32767.0 *. x) in
    pf "  %d, /* sin(%f) = %f */\n" d t x
  done;
  pf "};\n"
;;

let _ = sintable 1024;;
