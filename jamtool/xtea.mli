(* Xtea *)

val decipher_block : int32 -> int32 -> int32 array -> int32 * int32
val encipher_cbc : int32 array -> int32 array -> unit
val decipher_cbc : int32 array -> int32 array -> unit
val decipher : int array -> int32 array -> int array
