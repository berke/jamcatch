let fp = Printf.fprintf;;

let generate oc fix f_r f_0 f_c q m name =
  fp oc "#include <fix.h>\n\n";
  fp oc "const fix_t %s[%d] = {\n" name m;
  let fix = float (1 lsl fix) in
  for i = 0 to m - 1 do
    let f = 0.5 *. f_r *. (float i) /. (float m) in
    let f' = f_c -. f in
    let df = (f' -. f_0) /. f_0 in
    let zeta = q in
    let x = exp (-. 1.0 /. zeta *. df *. df) in
    let y = int_of_float (fix *. x) in
    fp oc "  %d, /* %fHz (%fHz) : %f */\n" y f f' x
  done;
  fp oc "};\n"
;;

let _ =
  match Array.to_list Sys.argv with
  | _ :: fix' :: f_r' :: f_0' :: f_c' :: q' :: m' :: name :: [] ->
    let fix = int_of_string fix' in
    let f_r = float_of_string f_r' in (* Sample rate *)
    let f_0 = float_of_string f_0' in (* Center frequency *)
    let f_c = float_of_string f_c' in (* Carrier *)
    let q   = float_of_string q' in (* Bandwidth *)
    let m   = (int_of_string m') / 2 in (* FFT window length *)
    generate stdout fix f_r f_0 f_c q m name
  | _ ->
      Printf.eprintf "usage: genclass <fix> <f_r> <f_0> <f_c> <q> <2m> <name>\n";
      exit 1
;;
