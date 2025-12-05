let fp = Printf.fprintf;;

let generate oc f_r f_0 f_c q m name =
  fp oc "#include \"fix.h\"\n\n";
  fp oc "const fix_t %s[%d] = {\n" name m;
  for i = 0 to m - 1 do
    let f = 0.5 *. f_r *. (float i) /. (float m) in
    let f' = f_c -. f in
    let df = (f' -. f_0) /. f_0 in
    let zeta = q in
    let x = exp (-. zeta *. df *. df) in
    let y = int_of_float (16777216.0 *. x) in
    fp oc "  %d, /* %fHz (%fHz) : %f */\n" y f f' x
  done;
  fp oc "};\n"
;;

let _ =
  let f_r = float_of_string Sys.argv.(1) in (* Sample rate *)
  let f_0 = float_of_string Sys.argv.(2) in (* Center frequency *)
  let f_c = float_of_string Sys.argv.(3) in (* Carrier *)
  let q   = float_of_string Sys.argv.(4) in (* Bandwidth *)
  let m   = int_of_string Sys.argv.(5) in (* FFT window length *)
  let name = Sys.argv.(6) in
  generate stdout f_r f_0 f_c q m name
;;
