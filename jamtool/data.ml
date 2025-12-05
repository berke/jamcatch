(* Data *)

open Int32_syntax;;

(*** pad_to *)
let pad_to m a =
  let n = Array.length a in
  if n > m then
    invalid_arg (Printf.sprintf "pad_to: %d=|a| > m=%d" n m)
  else
    if m = n then
      a
    else
      Array.concat [ a; Array.make (m - n) 0l ]
;;
(* ***)

(*** string_of_int32_array *)
let string_of_int32_array ?(big=true) a =
  let m = Array.length a in
  let u = String.create (4 * m) in
  let f i x = u.[i] <- Char.chr (??? x) in
  for i = 0 to m - 1 do
    let x = a.(i) in
    if big then
      begin
        f (4 * i + 0) (x >>> 24);
        f (4 * i + 1) ((x >>> 16) &&& 255l);
        f (4 * i + 2) ((x >>> 8) &&& 255l);
        f (4 * i + 3) (x &&& 255l);
      end
    else
      begin
        f (4 * i + 3) (x >>> 24);
        f (4 * i + 2) ((x >>> 16) &&& 255l);
        f (4 * i + 1) ((x >>> 8) &&& 255l);
        f (4 * i + 0) (x &&& 255l);
      end
  done;
  u
;;
(* ***)
(*** int32_array_of_string *)
let int32_array_of_string ?(big=true) u =
  let m = String.length u in
  if m land 3 <> 0 then invalid_arg "String length not dividible by 4";
  let n = m / 4 in
  let f j = !!! (Char.code u.[j]) in
  if big then
    Array.init n
      begin fun i ->
        (f (4 * i) <<< 24) |||
        (f (4 * i + 1) <<< 16) |||
        (f (4 * i + 2) <<< 8) |||
        f (4 * i + 3)
      end
  else
    Array.init n
      begin fun i ->
        (f (4 * i + 3) <<< 24) |||
        (f (4 * i + 2) <<< 16) |||
        (f (4 * i + 1) <<< 8) |||
        f (4 * i + 0)
      end
;;
(* ***)
