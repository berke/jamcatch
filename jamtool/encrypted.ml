(* Encrypted *)

open Device;;
open Int32_syntax;;

exception Invalid_data;;

class encrypted_device ~key ~device ?(big=false) () =
  let device_block_size = device#block_size in
  let m = (device_block_size / 4) in
  let signature_index = m - 2
  and number_index = m - 1
  in
  let signature = 0x31415926l
  in
  let salt () = Random.int32 (Int32.max_int) in
  object(self)
    method block_size = device_block_size - 16

    method get block =
      let b0 = device#get block in
      let c0 = Data.int32_array_of_string ~big b0 in
      Xtea.decipher_cbc c0 key;
      if c0.(signature_index) = signature &&
         c0.(number_index) = !!! block then
        begin
          let p0 = Array.sub c0 2 (Array.length c0 - 4) in
          p0
        end
      else
        begin
          raise Invalid_data
        end

    method put block data =
      let p0 = Array.concat
        [
          [| salt (); salt () |];
          Data.pad_to (m - 4) data;
          [| signature;
             !!! block |] ]
      in
      Xtea.encipher_cbc p0 key;
      let c0 = Data.string_of_int32_array ~big p0 in
      device#put block c0;
      ()
  end
;;
