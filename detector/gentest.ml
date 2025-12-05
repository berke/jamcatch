let draw_gaussian =
  let spare = ref None in
  fun () ->
    match !spare with
    | None ->
      let rec loop () =
        let a = Random.float 2.0 -. 1.0
        and b = Random.float 2.0 -. 1.0
        in
        let r = a *. a +. b *. b in
        if r = 0.0 or r > 1.0 then loop ()
        else
          begin
            let f = sqrt (-2.0 *. log r /. r) in
            spare := Some(a *. f);
            b *. f
          end
      in
      loop ()
    | Some x ->
        spare := None;
        x
;;

let gaussian mu sigma = mu +. sigma *. draw_gaussian ();;

let pi = 3.14159265358979323846264338327950288419716939937510;;

let db_to_amplitude x = 10.0 ** (x /. 20.0);;

let seq = [|
  1.0, 2.0e-3;
  0.0, 5.0e-3;
  1.0, 2.0e-3;
  0.0, 25.0e-3;
  1.0, 2.0e-3;
  0.0, 25.0e-3
|];;

let _ =
  let t_total = 90.0 in
  let f_r = 18000.0 in
  let f_c = 58000.0 in
  let f_mod = 61000.0 in
  let f_diff = f_mod -. f_c in
  let t_p = Array.fold_left (fun tot (_,t) -> tot +. t) 0.0 seq in
  let noise_amplitude = db_to_amplitude (-30.0) in
  let m = int_of_float (t_total *. f_r) in
  let jammer_events =
    [(20.0, 0.3, 0.1, 57996.0, 4.0);
     (30.0, 0.3, 0.1, 57996.0, 4.0);
     (40.0, 0.3, 0.1, 57996.0, 4.0);
     (45.0, 0.3, 0.1, 57996.0, 4.0);
     (50.0, 0.3, 0.1, 57996.0, 4.0);
     (60.0, 0.3, 0.1, 57996.0, 4.0);
     (70.0, 0.3, 0.1, 57996.0, 4.0);
     (75.0, 0.3, 0.1, 57996.0, 4.0)]
  in
  let last_i = ref 0 in
  let last_draw = ref true in
  for i = 0 to m - 1 do
    let t = (float i) /. f_r in
    let easx =
      let phi = mod_float t t_p in
      let rec amplitude i tot =
        let (a, phi') = seq.(i) in
        let tot' = tot +. phi' in
        if i + 1 = Array.length seq or phi < tot' then
          begin
            if i <> !last_i then
              begin
                last_draw := Random.float 1.0 < 1.75;
                last_i := i
              end;
            if !last_draw then a else 0.0
          end
        else
          amplitude (i + 1) tot'
      in
      let a = amplitude 0 0.0 in
      a *. cos(2.0 *. pi *. f_diff *. t)
    in
    let jx =
      List.fold_left
        begin fun jx (j_t0, j_sigma, j_amplitude, j_f, j_exp) ->
          let dt = (t -. j_t0) in
          let a = exp (-. dt *. dt /. j_sigma) in
          let a = a ** j_exp in
          jx +. j_amplitude *. a *. sin (2.0 *. pi *. (f_mod -. j_f) *. t)
        end
        0.0
        jammer_events
    in
    let noise = gaussian 0.0 noise_amplitude in
    let x = noise +. easx +. jx in
    Printf.printf "%f %f\n" t x
  done
;;
