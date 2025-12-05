(* Findbaud *)

let _ =
  let f_clk = 14.7456e6 in
  let m = 4.0 in
  let p_clk = m *. f_clk in
  let baud = 115200.0 in
  let best_params = ref (0,0,0) in
  let best_baud = ref 0.0 in
  let best_d = ref (-1.0) in
  while true do
    let d = Random.int 65536 in
    let m = 1 + Random.int 15 in
    let a = Random.int 16 in

    let d' = float d in
    let m' = float m in
    let a' = float a in

    let rate = p_clk /.(16.0 *. d') *. (m' /. (m' +. a')) in
    let dist = abs_float (rate -. baud) in
    if !best_d < 0.0 or dist < !best_d then
      begin
        best_d := dist;
        best_params := (d,m,a);
        best_baud := rate;
        Printf.printf "PCLK=%f U0DL=%d MulVal=%d DivAddVal=%d BAUD=%f\n%!" p_clk d m a rate
      end
  done
;;
