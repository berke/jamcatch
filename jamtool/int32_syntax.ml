(* Int32_syntax
* *)

open Int32;;

let ( ||| ) = logor
let ( +++ ) = add
let ( --- ) = sub
and ( <<< ) = shift_left
and ( >>> ) = shift_right_logical
and ( &&& ) = logand
and ( ~~~ ) = lognot
and ( ^^^ ) = logxor
and ( !!! ) = of_int
and ( ??? ) = to_int
;;

