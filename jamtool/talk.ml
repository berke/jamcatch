(* Talk *)

let error x =
  Printf.printf "ERROR: ";
  Printf.kfprintf
    (fun oc ->
      Printf.fprintf oc "\n%!";
      exit 1)
    stdout
    x
;;

let info x =
  Printf.printf "\027[32m";
  Printf.kfprintf (fun oc -> Printf.fprintf oc "\027[0m\n%!") stdout x
;;

let debug x =
  Printf.printf "\027[33mDEBUG: ";
  Printf.kfprintf (fun oc -> Printf.fprintf oc "\027[0m\n%!") stdout x
;;
