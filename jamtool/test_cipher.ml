let _ =
  for i = 1 to 100 do
    let rnd _ = Random.int32 Int32.max_int in
    let key = Array.init 4 rnd in
    let m = 4 * (1 + Random.int 1000) in
    let plain = Array.init m rnd in
    let cipher = Array.copy plain in
    Xtea.encipher_cbc cipher key;
    let cipher' = Data.string_of_int32_array cipher in
    let cipher'' = Data.int32_array_of_string cipher' in
    let plain' = Array.copy cipher'' in
    Xtea.decipher_cbc plain' key;
    if plain = plain' then
      Talk.debug "OK"
    else
      begin
        Talk.debug "Plain:";
        Hexdump.hexdump stdout plain;
        Talk.debug "Decrypted:";
        Hexdump.hexdump stdout plain'
      end
  done
;;
