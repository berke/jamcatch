(* Serdump *)
(* By Berke Durak *)

open Unix

module Opt =
  struct
    open Arg;;

    let sao x y = x := Some y;;
    let set_int32 x y = x := (Int32.of_string y);;

    let modem_device = ref "/dev/ttyS1";;
    let baud = ref 115200;;

    let spec =
      align [
        "-device",        String( (:=) modem_device ), "<device> set modem device";
        "-baud",          Int( (:=) baud ),            "<rate> baud rate";
      ]
    ;;
  end
;;

let pf = Printf.printf;;
let fp = Printf.fprintf;;
let sf = Printf.sprintf;;

exception Timeout;;
exception Match_failure;;

let input_char_with_timeout ?(timeout=2.0) fd =
  let w = String.make 1 '\000' in
  match select [fd] [] [] timeout with
  | [fd],[],[] ->
    let n = read fd w 0 1 in
    if n = 0 then
      raise End_of_file
    else
      w.[0]
  | _ -> raise Timeout
;;

let match_string ?timeout u fd =
  let m = String.length u in
  let rec loop i =
    if i = m then
      ()
    else
      let c = input_char_with_timeout ?timeout fd in
      if c = u.[i] then
        loop (i + 1)
      else
        raise Match_failure
  in
  loop 0
;;

let ( !!! ) = Int32.of_int;;
let ( !!? ) = Int32.to_int;;
let ( +++ ) = Int32.add;;
let ( --- ) = Int32.sub;;
let ( ||| ) = Int32.logor;;
let ( &&& ) = Int32.logand;;
let ( <<< ) = Int32.shift_left;;
let ( >>> ) = Int32.shift_right_logical;;

let main () =
  Arg.parse Opt.spec (fun x -> Printf.eprintf "Extraneous argument %S ignored.\n%!" x) 
    "Will output data from serial port to stdout.\n";
  let fd = openfile !Opt.modem_device [O_RDWR] 0 in
  let ta = tcgetattr fd in
  let ta' = { ta with
	      c_istrip = false;
	      c_inlcr = false;
	      c_igncr = false;
	      c_icrnl = false;
	      c_ixoff = false;
	      c_ixon = false;
	      c_obaud = !Opt.baud;
	      c_ibaud = !Opt.baud;
	      c_csize = 8;
	      c_cstopb = 1;
	      c_cread = true;
	      c_parenb = false;
	      c_hupcl = false;
	      c_clocal = true;
	      c_isig = false;
	      c_icanon = false;
	      c_noflsh = true;
	      c_echo = false;
	      c_echoe = false;
	      c_echok = false;
	      c_echonl = false;
	      c_vmin = 1;
	      c_vtime = 0;
	    } 
  in
  Sys.catch_break true;
  tcsetattr fd TCSANOW ta';
  try
    let _ic = in_channel_of_descr fd
    and _oc = out_channel_of_descr fd
    in
    let get_char () = input_char_with_timeout fd in
    let match_char c =
      let c' = get_char () in
      if c <> c' then raise Match_failure
    in
    let read_line () =
      let b = Buffer.create 1024 in
      let rec loop () =
        let c = get_char () in
        if c = '\r' then
          begin
            match_char '\n';
            let u = Buffer.contents b in
            u
          end
        else
          begin
            Buffer.add_char b c;
            loop ()
          end
      in
      loop ()
    in
    let read_string fd buf =
      let m = String.length buf in
      let rec loop i m =
        if m = 0 then
          ()
        else
          let n = read fd buf i m in
          loop (i + n) (m - n)
      in
      loop 0 m
    in
    while true do
      let l = read_line () in
      let dot_length = 512 in
      let buf = String.make (4 * dot_length) '\000' in
      if l = "dotframe" then
        begin
          read_string fd buf;
          for i = 0 to dot_length - 1 do
            let f j =
              let k = Char.code buf.[j] in
              !!! k
            in
            let x0 = f (4 * i)
            and x1 = f (4 * i + 1)
            and x2 = f (4 * i + 2)
            and x3 = f (4 * i + 3)
            in
            let x = x0 ||| (x1 <<< 8) ||| (x2 <<< 16) ||| (x3 <<< 16) in
            let y = Int32.to_float x in
            let y = y /. 65536.0 in
            Printf.printf "%f\n" y
          done;
          Printf.printf "%!"
        end
      else
        Printf.eprintf "Unknown line %S\n%!" l
    done
  with
  | Sys.Break|End_of_file ->
    tcsetattr fd TCSANOW ta;
    close fd
;;

let _ = main ();;
