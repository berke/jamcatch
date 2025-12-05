let fp = Printf.fprintf;;

let pi = 3.14159265358979323846264338327950288419716939937510;;

let generate_gaussian_window oc fix alpha m name =
  fp oc "#include \"fix.h\"\n\n";
  fp oc "const fix_t %s[%d] = {\n" name m;
  for i = 0 to m - 1 do
    let t = float (i - (m / 2)) /. (float m) in
    let x = exp (-. alpha *. t *. t) in
    let y = int_of_float (fix *. x) in
    fp oc "  %d, /* %f */\n" y x
  done;
  fp oc "};\n"
;;

let generate_cos_window oc fix alpha m name =
  fp oc "#include \"fix.h\"\n\n";
  fp oc "const fix_t %s[%d] = {\n" name m;
  let beta = 1.0 -. alpha in
  for i = 0 to m - 1 do
    let t = (float i) /. (float m) in
    let x =
      if t < alpha then
        0.5 *. (1.0 +. cos (pi +. (pi /. alpha) *. t))
      else if t > beta then
        0.5 *. (1.0 +. cos ((pi /. alpha) *. (t -. beta)))
      else
        1.0
    in
    let y = int_of_float (fix *. x) in
    fp oc "  %d, /* %f */\n" y x
  done;
  fp oc "};\n"
;;

let _ =
  match Array.to_list Sys.argv with
  | _ :: "gaussian" :: alpha' :: fix' :: m' :: file :: name :: [] ->
      let fix = float (1 lsl (int_of_string fix')) in
      let alpha = float_of_string alpha' in
      let m = int_of_string m' in
      let oc = open_out file in
      generate_gaussian_window oc fix alpha m name;
      close_out oc
  | _ :: "cos" :: alpha' :: fix' :: m' :: file :: name :: [] ->
      let fix = float (1 lsl (int_of_string fix')) in
      let alpha = float_of_string alpha' in
      let m = int_of_string m' in
      let oc = open_out file in
      generate_cos_window oc fix alpha m name;
      close_out oc
  | _ ->
      Printf.eprintf "usage: window {gaussian|cos} <alpha> <fix> <m> <file> <name>\n%!";
      exit 1
;;
