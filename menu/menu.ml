(* Menu *)

type flag =
  | SEL_INIT
  | SEL_DEFAULT1
;;

let string_of_flag = function
  | SEL_INIT -> "SEL_INIT"
  | SEL_DEFAULT1 -> "SEL_DEFAULT1"
;;

type menu = string * selection
and selection =
| M of menu array
| MN of menu array * notify * notify
| A of string * notify (* Action *) * flag list
| D of string (* Display static string *) * flag list
| I of int * int * var * int * notify * flag list
| IF of int * int * var * int * notify * flag list * string
| C of string array * var * notify * flag list
and notify =
| Ignore
| Notify of string (* C function name *)
and var =
| G of string
| L of string
;;

let sf = Printf.sprintf;;

let bindings : (var * int) list ref = ref [];;

let bind name x = bindings := (name, x) :: !bindings;;

let range ?fmt ?(flags=[]) name notify low cur high =
  bind name cur;
  match fmt with
  | None -> I(low, cur, name, high, notify, flags)
  | Some fmt -> IF(low, cur, name, high, notify, flags, fmt)
;;

let choice ?(flags=[]) name notify a =
  bind name 0;
  C(a, name, notify, flags)
;;

let yes_no ?flags name notify = choice ?flags name notify [|"No";"Yes"|];;

let root =
  "Menu",
  M[|
    "Log",      A("View log?", Notify "ui_view_log",         []);

    "Auto-adjust",  A("Readjust?", Notify "ui_readjust",         []);

    "Settings",
    M[|
       "Load",      A("Load from disk?", Notify "ui_load_settings", []);
       "Threshold", range ~flags:[SEL_INIT]
                      (G "settings.st_alarm_threshold") (Notify "ui_alarm_threshold_notify") 0 16 30;
       "Speed",     range ~flags:[SEL_INIT]
                      (G "settings.st_alarm_decay") (Notify "ui_alarm_decay_notify") 0 20 30;
       "Enabled",   yes_no ~flags:[SEL_INIT; SEL_DEFAULT1]
                      (G "settings.st_alarm_enabled") (Notify "ui_alarm_enabled_notify");
       "EAS detect", range
                      ~flags:[SEL_INIT]
                      ~fmt:"ui_eas_threshold_formatter"
                      (G "settings.st_eas_threshold")
                      Ignore
                      0 2 16;
       "Indicator", range ~flags:[SEL_INIT]
                      (G "settings.st_indicator_threshold")
                      Ignore
                      0 5 10;
       "Save",      A("Save to disk?",   Notify "ui_save_settings", []);
    |];

    "Reset",
      M[|
         "Log",       A("Erase log?",   Notify "ui_erase_log", []);
         "Counter",   A("Reset?",   Notify "ui_reset_counter", []);
      |];

    "Date & time",
      MN([|
        "Year",  range (L "ui_date_year")     Ignore 2007 2007 2030;
        "Month", range ~fmt:"ui_month_formatter" (L "ui_date_month")    Ignore 01 06 12;
        "Day",   range (L "ui_date_day")      Ignore 01 19 31;
        "Hour",  range (L "ui_date_hours")    Ignore 00 20 23;
        "Min",   range (L "ui_date_minutes")  Ignore 00 33 59;
        "Apply",   A("Change clock?", Notify "ui_date_and_time_set", []);
      |], Notify "ui_date_and_time_enter", Ignore);

    "Info",
    M[|
      "Version", D("Firmware 1.00",    []);
      "Serial",  D("J-0002",           []);
      "About",   D("(C) Therodox R&D", []);
      "URL",     D("therodox.com",     []);
    |];
  |]
;;

type path = (menu * int) list;;

let down_arrow = "↓";;
let up_arrow = "↑";;
let left_arrow = "←";;
let right_arrow = "→";;
let right_head_arrow = right_arrow;;
let left_head_arrow = left_arrow;;
let star = " ↓:Set";;

module Display =
  struct
    open Unix;;

    exception Quit;;

    let fp = Printf.fprintf;;

    class display ~root ?(bindings=[]) ?(m=2) ?(n=16) ic oc =
      let ifd = descr_of_in_channel ic
      and ofd = descr_of_out_channel oc
      in
      let settings = Hashtbl.create 100 in
      let find u =
        try
          Hashtbl.find settings u
        with
        | Not_found ->
            let r = ref 0 in
            Hashtbl.replace settings u r;
            r
      in
      let cur = ref 0 in
      let get_cur () = !cur in
      let set_cur x = cur := x in
      let get u = !(find u) in
      let set u x = (find u) := x in
      let _ = List.iter (fun (k,v) -> set k v) bindings in
      object(self)
        val display = Array.init m (fun _ -> String.make n ' ')
        val mutable current_node = root
        val mutable current_choice = 0
        val mutable path = []
        val mutable flag_up = false
        val mutable flag_right = " "
        val mutable flag_left = " "

        val ta = tcgetattr ifd
        initializer
          let ta' =
            { ta with
	      c_istrip = false;
	      c_inlcr = false;
	      c_igncr = false;
	      c_icrnl = false;
	      c_ixoff = false;
	      c_ixon = false;
              c_opost = false;
	      c_parenb = false;
	      c_hupcl = false;
	      c_isig = false;
	      c_icanon = false;
	      c_echo = false;
	      c_echonl = false;
	    } 
          in
          tcsetattr ifd TCSANOW ta';
          self#clear;
          self#update;
          self#redraw

        method restore =
          self#clear;
          tcsetattr ofd TCSANOW ta

        method set_line i u =
          let p = min n (String.length u) in
          for j = 0 to p - 1 do
            display.(i).[j] <- u.[j]
          done;
          for j = p to n - 1 do
            display.(i).[j] <- ' '
          done

        method current_arity =
          let (_, menu) = current_node in
          match menu with
          | M sa|MN(sa, _, _) -> Array.length sa
          | _ -> 0

        method update =
          begin
            match path with
            | [] -> flag_up <- false
            | _::_ -> flag_up <- true
          end;
          let (t, menu) = current_node in
          self#set_line 0 t;
          flag_left <- " ";
          flag_right <- " ";
          match menu with
          | D(u, _) ->
              self#set_line 1 u
          | A(t, _, _) ->
              self#set_line 1 (t^down_arrow)
          | C(choices, old, _notify, _) ->
              flag_left <- "-";
              flag_right <- "+";
              self#set_line 1 (sf "%s%s" choices.(get_cur ()) (if get_cur () <> get old then star else ""))
          | I(low, def, old, high, _notify, _)|
            IF(low, def, old, high, _notify, _, _) ->
              flag_left <- "-";
              flag_right <- "+";
              self#set_line 1 (sf "%d%s" (get_cur ()) (if get_cur () <> get old then star else ""))
          | M sa|MN(sa, _, _) ->
            let (u, _) = sa.(current_choice) in
            self#set_line 1 (u^down_arrow);
            let arity = self#current_arity in
            if arity > 0 then
              begin
                flag_left <-
                  if current_choice = 0 then
                    left_head_arrow
                  else
                    left_arrow;
                flag_right <-
                  if current_choice = arity - 1 then
                    right_head_arrow
                  else
                    right_arrow;
              end
            else
              begin
                flag_left <- " ";
                flag_right <- " ";
              end


        method clear =
          fp oc "\027c"

        method home =
          fp oc "\027[1;1H"

        method redraw =
          self#home;
          let l = flag_left in
          let r = flag_right in
          let u = String.concat "" (List.map (fun _ -> up_arrow) path) in
          fp oc "%s\027[K\027[1;%dH%s\r\n"
            display.(0)
            (n - (List.length path) + 1)
            u;
          fp oc "%s %s\027[K\027[2;%dH%s\r\n" l display.(1) n r;
          fp oc "%!"

        method step =
          let c = input_char ic in
          try
            begin
              match c with
              | 'h' -> self#left
              | 'l' -> self#right
              | 'j' -> self#down
              | 'k' -> self#up
              | 'q' -> raise Quit
              | _ -> raise Not_found
            end;
            self#update;
            self#redraw
          with
          | Not_found -> ()

        method run =
          Sys.catch_break true;
          try
            while true do
              self#step
            done
          with
          | Quit -> self#restore
          | x ->
              self#restore;
              raise x

        method movement delta =
          let (_, menu) = current_node in
          match menu with
          | M _|MN _ ->
            current_choice <- delta + current_choice;
            if current_choice < 0 then
              current_choice <- self#current_arity - 1 
            else
              if current_choice = self#current_arity then
                current_choice <- 0
              else
                ()
          | I(low, _def, old, high, _notify, _) ->
              let c = get_cur () in
              if low <= c + delta && c + delta < high then
                set_cur (c + delta)
              else
                ()
          | C(choices, old, _notify, _) ->
              let low = 0
              and high = Array.length choices
              in
              let c = get_cur () in
              if low <= c + delta && c + delta < high then
                set_cur (c + delta)
              else
                ()
          | _ -> ()

        method right = self#movement 1
        method left = self#movement (-1)

        method up =
          match path with
          | [] -> ()
          | (node, choice) :: rest ->
              path <- rest;
              current_node <- node;
              current_choice <- choice

        method enter =
          let (_, menu) = current_node in
          match menu with
          | I(_,_,old,_,_,_) | C(_,old,_,_) -> set_cur (get old)
          | _ -> ()

        method down =
          let (t, menu) = current_node in
          match menu with
          | A(_, Ignore,_) -> ()
          | A(_, Notify u,_) -> Printf.printf "Notify %s\n%!" u
          | D _ -> ()
          | I(_,_,old,_,_,_) | IF(_,_,old,_,_,_,_) | C(_,old,_,_) -> set old (get_cur ())
          | M sa|MN(sa, _, _) ->
            let menu' = sa.(current_choice) in
            path <- (current_node, current_choice) :: path;
            current_node <- menu';
            self#enter;
            current_choice <- 0
      end
    ;;
  end
;;

let test () =
  let d = new Display.display ~bindings:!bindings ~root stdin stdout in
  d#run
;;

let fp = Printf.fprintf;;
let bf = Printf.bprintf;;

let bcomma oc =
  let first = ref true in
  fun () ->
    if !first then
      first := false
    else
      bf oc ", "
;;

let bprint_array f b a =
  let v = bcomma b in
  Array.iter (fun x -> v (); f b x) a
;;

let generate ?(prefix="foo") ~name ~dir root =
  let dirify x = Filename.concat dir x in
  let fnc = name^".c" in
  let fnh = name^".h" in
  let namec = String.uppercase name in
  let occ = open_out (dirify fnc) in
  let och = open_out (dirify fnh) in
  let header oc name =
    fp oc "/* %s\n" name;
    fp oc " *\n";
    fp oc " * Copyright (C)2007 Therodox R&D\n";
    fp oc " * All rights reserved\n";
    fp oc " */\n";
    fp oc "\n";
  in
  header occ fnc;
  header och fnh;
  fp och "#ifndef %s_H\n" namec;
  fp och "#define %s_H\n" namec;
  fp och "\n";
  fp och "#include <ui.h>\n";
  fp och "#include <settings.h>\n";
  fp och "#include <menu.h>\n";
  fp och "\n";
  fp occ "#include <%s>\n" fnh;
  fp occ "\n";

  let gensym =
    let k = ref 0 in
    fun kind ->
      incr k;
      Printf.sprintf "%s_%s_%d" prefix kind !k
  in
  let headers = ref [] in
  let menus = ref [] in
  let bf = Printf.bprintf in
  let emit_label_array a =
    let name = gensym "labels" in
    let b = Buffer.create 256 in
    bf b "static const char * const %s[] = {\n" name;
    Array.iter (fun n -> bf b "  %S,\n" n) a;
    bf b "};\n\n";
    let u = Buffer.contents b in
    menus := u :: !menus;
    name
  in
  let emit_menu_array a =
    let name = gensym "array" in
    headers := (sf "static const menu_t * const %s[];\n" name) :: !headers;
    let b = Buffer.create 256 in
    bf b "static const menu_t * const %s[] = {\n" name;
    Array.iter (fun n -> bf b "  &%s,\n" n) a;
    bf b "};\n\n";
    let u = Buffer.contents b in
    menus := u :: !menus;
    name
  in
  let emit_int = function
    | L name ->
        menus := (sf "int %s;\n" name) :: !menus;
        fp och "extern int %s;\n" name
    | G _ -> ()
  in
  let bf_var b = function
    | L name|G name -> bf b "%s" name
  in
  let rec emit_selection ?(root=false) ?parent ?name () (title, menu) =
    let b = Buffer.create 256 in
    let name =
      match name with
      | None -> gensym "menu"
      | Some u -> u
    in
    let st = if root then "" else "static " in
    headers := (sf "%sconst menu_t %s;\n" st name) :: !headers;
    bf b "%sconst menu_t %s = {\n" st name;
    bf b "  .m_up = %s,\n" (match parent with None -> "0" | Some p -> "&"^p);
    bf b "  .m_label = %S,\n" title;
    bf b "  .m_selection = {\n";
    let notifier b = function
      | Ignore -> bf b "0"
      | Notify u ->
          (*fp och "void %s(void * );\n" u;*)
          bf b "%s" u
    in
    begin
      let add_notify name = function
        | None -> ()
        | Some notify -> bf b "                  .%s = %a,\n" name notifier notify
      in
      let do_menu ?notify1 ?notify2 a =
        let a' = Array.map (emit_selection ~parent:name ()) a in
        bf b "    .s_type = SEL_MENU,\n";
        bf b "    .s_content = {\n";
        bf b "      .s_menu = { .sm_count = %d,\n" (Array.length a');
        add_notify "sm_notify_enter" notify1;
        add_notify "sm_notify_exit" notify2;
        bf b "                  .sm_entries = %s } },\n" (emit_menu_array a');
      in
      let bf_flags b fl =
        if fl = [] then
          bf b "0"
        else
          bf b "%s" (String.concat " | " (List.map string_of_flag fl))
      in
      let do_integer ?fmt low def current high notify flags =
        bf b "    .s_type = SEL_INT,\n";
        bf b "    .s_flags = %a,\n" bf_flags flags;
        bf b "    .s_content = {\n";
        bf b "      .s_int = { .si_low = %d,\n" low;
        bf b "                 .si_default = %d,\n" def;
        bf b "                 .si_high = %d,\n" high;
        bf b "                 .si_notify = %a,\n" notifier notify;
        bf b "                 .si_formatter = %s,\n" (match fmt with | None -> "0" | Some f -> f);
        emit_int current;
        bf b "                 .si_current = &%a } },\n" bf_var current
      in
      match menu with
      | M a -> do_menu a
      | A(prompt, notify, flags) ->
          bf b "    .s_type = SEL_ACTION,\n";
          bf b "    .s_flags = %a,\n" bf_flags flags;
          bf b "    .s_content = {\n";
          bf b "      .s_action = { .sa_prompt = %S,\n" prompt;
          bf b "                    .sa_notify = %a } },\n" notifier notify
      | MN(a, notify1, notify2) -> do_menu ~notify1 ~notify2 a
      | D(u, flags) ->
          bf b "    .s_type = SEL_DISPLAY,\n";
          bf b "    .s_flags = %a,\n" bf_flags flags;
          bf b "    .s_content = {\n";
          bf b "      .s_display = { .sd_label = %S } },\n" u
      | I(low, def, current, high, notify, flags) -> do_integer low def current high notify flags
      | IF(low, def, current, high, notify, flags, fmt) ->
          do_integer ~fmt low def current high notify flags

      | C(ca, current, notify, flags) ->
          emit_int current;
          bf b "    .s_type = SEL_CHOICE,\n";
          bf b "    .s_flags = %a,\n" bf_flags flags;
          bf b "    .s_content = {\n";
          bf b "      .s_choice = { .sc_count = %d,\n" (Array.length ca);
          bf b "                    .sc_current = &%a,\n" bf_var current;
          bf b "                    .sc_labels = %s,\n" (emit_label_array ca);
          bf b "                    .sc_notify = %a } },\n" notifier notify;
    end;
    bf b "  }\n";
    bf b "};\n\n";
    let u = Buffer.contents b in
    menus := u :: !menus;
    name
  in
  ignore (emit_selection ~root:true ~name:(prefix^"_root") () root);
  List.iter (output_string och) (List.rev !headers);
  List.iter (output_string occ) (List.rev !menus);
  close_out occ;

  fp och "\n";
  fp och "#endif\n";
  close_out och
;;

let _ =
  let a = Array.to_list Sys.argv in
  match List.tl a with
  | ["test"] -> test ()
  | ["generate";prefix;dir;name] -> generate ~prefix ~dir ~name root
  | _ -> invalid_arg "wtf?"
;;
