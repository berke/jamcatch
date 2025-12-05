(* Xtea *)

open Int32_syntax;;

let wordify a =
  let m = (Array.length a) / 4 in
  let b = Array.make m 0l in
  for i = 0 to m - 1 do
    let rec loop j q =
      if j = 4 then
        q
      else
        loop (j + 1) ((q <<< 8) ||| (!!! (a.(4 * i + j))))
    in
    b.(i) <- loop 0 0l
  done;
  b
;;

let byteify a =
  let m = Array.length a in
  let b = Array.make (4 * m) 0 in
  for i = 0 to m - 1 do
    let x = a.(i) in
    b.(4 * i + 3) <- ??? (x &&& 255l);
    b.(4 * i + 2) <- ??? ((x >>> 8) &&& 255l);
    b.(4 * i + 1) <- ??? ((x >>> 16) &&& 255l);
    b.(4 * i    ) <- ??? ((x >>> 24) &&& 255l)
  done;
  b
;;

let encipher_block v0 v1 k =
  let v0 = ref v0
  and v1 = ref v1
  and sum = ref 0l
  and delta = ref 0x9e3779b9l
  in
  for i = 0 to 31 do
    v0 := !v0 +++
      ((((!v1 <<< 4) ^^^ (!v1 >>> 5)) +++ !v1) ^^^ (!sum +++ k.(??? (!sum &&& 3l))));
    sum := !sum +++ !delta;
    v1 := !v1 +++
      ((((!v0 <<< 4) ^^^ (!v0 >>> 5)) +++ !v0) ^^^ (!sum +++ k.(??? ((!sum >>> 11) &&& 3l))));
  done;
  (!v0,!v1)
;;

let decipher_block v0 v1 k =
  let v0 = ref v0
  and v1 = ref v1
  and sum = ref 0xc6ef3720l
  and delta = ref 0x9e3779b9l
  in
  for i = 0 to 31 do
    v1 := !v1 --- ((((!v0 <<< 4) ^^^ (!v0 >>> 5)) +++ !v0) ^^^ (!sum +++ k.(??? ((!sum >>> 11) &&& 3l))));
    sum := !sum --- !delta;
    v0 := !v0 --- ((((!v1 <<< 4) ^^^ (!v1 >>> 5)) +++ !v1) ^^^ (!sum +++ k.(??? (!sum &&& 3l))));
  done;
  (!v0,!v1)
;;

let encipher_cbc v k =
  let m = (Array.length v) / 2 in
  let x0 = ref 0l
  and x1 = ref 0l
  in
  for i = 0 to m - 1 do
    let v0 = v.(2 * i)
    and v1 = v.(2 * i + 1)
    in
    let v0 = v0 ^^^ !x0
    and v1 = v1 ^^^ !x1
    in
    let (v0,v1) = encipher_block v0 v1 k in
    v.(2 * i) <- v0;
    v.(2 * i + 1) <- v1;
    x0 := v0;
    x1 := v1
  done
;;

let decipher_cbc v k =
  let m = (Array.length v) / 2 in
  let x0 = ref 0l
  and x1 = ref 0l
  in
  for i = 0 to m - 1 do
    let x0' = v.(2 * i)
    and x1' = v.(2 * i + 1)
    in
    let (v0,v1) = decipher_block x0' x1' k in
    v.(2 * i) <- v0 ^^^ !x0;
    v.(2 * i + 1) <- v1 ^^^ !x1;
    x0 := x0';
    x1 := x1';
  done
;;

let decipher v k =
  let v' = wordify v in
  decipher_cbc v' k;
  byteify v'
;;
