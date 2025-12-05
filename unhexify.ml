let _ =
  try
    while true do
      let l = input_line stdin in
      let g i =
        let b = String.sub l i 2 in
        let c = int_of_string ("0x"^b) in
        output_char stdout (Char.chr c)
      in
      g 6;
      g 4;
      g 2;
      g 0
    done
  with
  | End_of_file -> ()
;;
