let fp = Printf.fprintf;;

let gen () =
  let oc = open_out "log2.c" in
  fp oc "u32 log_2(u32 x) {\n";
  let indent o =
    for i = 1 to o do
      fp oc "  "
    done
  in
  let rec loop o i0 i1 =
    if i0 = i1 then
      begin
        indent o;
        fp oc "return %d;\n" i0
      end
    else
      begin
        let i = (i0 + i1) / 2 in
        let x = Int32.shift_left 1l i in
        indent o; fp oc "if(x < 0x%08lx) {\n" x;
        loop (o + 1) i0 i;
        indent o; fp oc "} else {\n";
        loop (o + 1) (i + 1) i1;
        indent o; fp oc "}\n"
      end
  in
  loop 1 0 31;
  fp oc "}\n";
  close_out oc
;;
