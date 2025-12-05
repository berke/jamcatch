(* Fourier *)
(* Copyright (C)2007 Therodox R&D *)
(* All rights reserved *)

module C = Complex;;

let swap a i j =(*{{{*)
  let t = a.(i) in
  a.(i) <- a.(j);
  a.(j) <- t
;;(*}}}*)
let bit_reverse a =(*{{{*)
  let m = Array.length a in
  let rec loop i j =
    if i = m / 2 then
      ()
    else
      begin
        swap a i j;
        let i = i + 1 in
        let rec increment mask j =
          let j' = j lxor mask in
          if j land mask = 0 then
            j'
          else
            increment (mask lsr 1) j'
        in
        let j = increment (m lsr 1) j in
        loop i j
      end
  in
  loop 0 0
;;(*}}}*)
let pi = 3.14159265358979323846264338327950288419716939937510;;
let omega k m = C.polar 1.0 (-2.0 *. pi *. (float k) /. (float m));;
let butterfly a b w w' =(*{{{*)
  let a' = C.add a (C.mul w b) in
  let b' = C.add a (C.mul w' b) in
  (a',b')
;;(*}}}*)
let fft x =(*{{{*)
  let m = Array.length x in
  bit_reverse x;
  let rec loop k =
    if k = m then
      ()
    else
      begin
        (* Input size is k *)
        (* Combine two inputs (a,b) of size k into an output of size 2 * k *)
        (* This is done in-place *)
        (* The array a is seen as being made of inputs
         *  a0 b0   a1 b1   a2 b2 ...
         * Lower half ai starts at 2 * k * i 
         * Upper half ai starts at 2 * k * i + k
        *)
        let n = m / (2 * k) in
        for i = 0 to n - 1 do
          let ai = 2 * k * i in
          let bi = ai + k in
          for j = 0 to k - 1 do
            let a = x.(ai + j) in
            let b = x.(bi + j) in
            Printf.printf "  k=%d i=%d j=%d butter %d %d\n" k i j (ai + j) (bi + j);
            let (a', b') = butterfly a b (omega j (2 * k)) (omega (j + k) (2 * k)) in
            x.(ai + j) <- a';
            x.(bi + j) <- b'
          done;
        done;
        loop (2 * k)
      end
  in
  loop 1;
  x
;;(*}}}*)
let fp = Printf.fprintf;;

type precision =
| Fix of int
| Double
;;

let genfft base ?(precision=Double) ms =
  let type_name =
    match precision with
    | Fix _ -> "fix_t"
    | Double -> "double"
  in
  let fnh = base^".h" in
  let oc = open_out fnh in
  fp oc "/* fft.c\n";
  fp oc " *\n";
  fp oc " * Copyright (C)2007 Therodox R&D\n";
  fp oc " * All rights reserved.\n";
  fp oc " */\n";
  fp oc "\n";
  fp oc "#ifndef %s_H\n" (String.uppercase base);
  fp oc "#define %s_H\n" (String.uppercase base);
  fp oc "\n";
  fp oc "#include <fix.h>\n";
  fp oc "\n";
  List.iter
    begin fun m ->
      fp oc "void fft_%d(%s *x);\n" m type_name;
    end
    ms;
  fp oc "\n";
  fp oc "#endif\n";
  close_out oc;
  let oc = open_out (base^".c") in
  fp oc "/* fft.c\n";
  fp oc " *\n";
  fp oc " * Copyright (C)2007 Therodox R&D\n";
  fp oc " * All rights reserved.\n";
  fp oc " */\n";
  fp oc "\n";
  fp oc "#include \"%s\"\n" (base^".h");
  fp oc "\n";
  let fix f x =
    let c = float (1 lsl f) in
    Int32.of_float (c *. x)
  in
  let m = List.fold_left max 0 ms in
  let rec loop k =
    if k > m then
      ()
    else
      begin
        (* Generate butterfly constants for order k *)
        fp oc "static const %s w_%d[%d] = {\n" type_name k k;
        for j = 0 to k / 2 - 1 do
          let w = omega j k in
          match precision with
          | Double -> fp oc "  %f, %f,\n" w.C.re w.C.im
          | Fix f ->
              fp oc "  %ld, %ld,\n" (fix f w.C.re) (fix f w.C.im)
        done;
        fp oc "};\n\n";
        loop (2 * k)
      end
  in
  begin
    match precision with
    | Double -> fp oc "static inline double mul(double a, double b) { return a * b; }\n"
    | Fix f -> ()
  end;
  loop 2;
  List.iter
    begin fun m ->
      fp oc "void fft_%d(%s *x) {\n" m type_name;
      fp oc "  int i, j;\n";
      fp oc "  %s *ap, *bp, ar, ai, br, bi, wr, wi, tr, ti;\n" type_name;
      fp oc "\n";
      let rec loop k =
        if k = m then
          ()
        else
          begin
            (* Input size is k *)
            (* Combine two inputs (a,b) of size k into an output of size 2 * k *)
            (* This is done in-place *)
            (* The array a is seen as being made of inputs
             *  a0 b0   a1 b1   a2 b2 ...
             * Lower half ai starts at 2 * k * i 
             * Upper half ai starts at 2 * k * i + k
            *)
            let n = m / (2 * k) in
            fp oc "  ap = x;\n";
            fp oc "  bp = x + %d;\n" (2 * k);
            fp oc "  for(i = 0; i < %d; i ++) {\n" n;

            fp oc "    for(j = 0; j < %d; j ++) {\n" k;

            fp oc "      ar = ap[0];\n";
            fp oc "      ai = ap[1];\n";
            fp oc "      br = bp[0];\n";
            fp oc "      bi = bp[1];\n";

            (*let (a', b') = butterfly a b (omega j (2 * k)) (omega (j + k) (2 * k)) in*)

            (* (ar + i ai) * (wr + i wi) = (ar wr - ai wi) + i (ai wr + wi ar) *)
            fp oc "      wr = w_%d[2 * j];\n" (2 * k);
            fp oc "      wi = w_%d[2 * j + 1];\n" (2 * k);
            fp oc "      tr = ar + mul(br, wr) - mul(bi, wi);\n";
            fp oc "      ti = ai + mul(bi, wr) + mul(br, wi);\n";
            fp oc "      ap[0] = tr;\n";
            fp oc "      ap[1] = ti;\n";

            fp oc "      tr = ar - mul(br, wr) + mul(bi, wi);\n";
            fp oc "      ti = ai - mul(bi, wr) - mul(br, wi);\n";
            fp oc "      bp[0] = tr;\n";
            fp oc "      bp[1] = ti;\n";

            fp oc "      ap += 2;\n";
            fp oc "      bp += 2;\n";
            fp oc "    }\n";

            fp oc "    ap += %d;\n" (2 * k);
            fp oc "    bp += %d;\n" (2 * k);

            fp oc "  }\n";

            loop (2 * k)
          end
      in
      loop 1;
      fp oc "}\n";
    end
    ms;
  close_out oc
;;

let seq () =
  let a = C.one in
  let b = C.neg a in
  [| a; b; b; b; a; a; a; b |]
;;

let _ =
  let al = Array.to_list Sys.argv in
  match al with
  | _ :: fix' :: file :: orders' ->
      let fix = int_of_string fix' in
      let orders = List.map int_of_string orders' in
      genfft file ~precision:(Fix fix) orders
  | _ ->
    Printf.eprintf "Usage: fourier <fix> <file> <order1> <order2> ...\n%!";
    exit 1
;;
