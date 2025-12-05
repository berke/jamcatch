let fp = Printf.fprintf;;

let generate_window oc alpha m =
  fp oc "#include \"fix.h\"\n\n";
  fp oc "const fix_t window[%d] = {\n" m;
  for i = 0 to m - 1 do
    let t = float (i - (m / 2)) /. (float m) in
    let x = exp (-. alpha *. t *. t) in
    let y = int_of_float (16777216.0 *. x) in
    fp oc "  %d, /* %f */\n" y x
  done;
  fp oc "};\n"
;;

let _ = generate_window stdout (float_of_string Sys.argv.(1)) (int_of_string Sys.argv.(2));;
