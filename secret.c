/* secret.c
 *
 * Copyright (C)2007 Therodox R&D
 * All rights reserved
 */

#include <secret.h>
#include <xtea.h>
#include <string.h>
#include <serial.h>
#include <config.h>

const password_t secret_password_table[] = {
  { "user",       { 0x0ef80452, 0xe0818572 }, E_USER       }, /* neptune */
  { "maintainer", { 0xf2ad3802, 0xa6bff51f }, E_MAINTAINER }, /* twinings */
  { "client",     { 0x8ad21105, 0x77a16320 }, E_CLIENT     }, /* malvino */
  { "integrator", { 0xad024a6e, 0x899bbccf }, E_INTEGRATOR }, /* atkins */
  { "root",       { 0x6ad288c3, 0xfb3de511 }, E_DEVELOPER  }, /* fx.. */
};

char *secret_hash_password(char *password, u32 length, u32 *hash)/*{{{*/
{
  u32 u[4]; /* Passwords are 16 characters max */

  u[0] = 0;
  u[1] = 0;
  u[2] = 0;
  u[3] = 0;

  if(length > sizeof(u)) return "Password too long";

  byte_copy((u8 *) password, (u8 *) u, length);
  xtea_hash(u, 1, hash);
  return 0;
}/*}}}*/
void secret_delay(void)/*{{{*/
{
  volatile s32 t;

  t = SECURITY_DELAY;
  for(;;) {
    t -= 4;
    if(t < 0) break;
  }
}/*}}}*/
char *secret_attempt_login(char *user, u32 user_length,/*{{{*/
                           char *password, u32 password_length,
                           u32 *privileges)
{
  int i;
  u32 hash[2];
  u32 result;
  char *error;
  const password_t *pw;

  error = "No such user";
  for(i = 0; i < length(secret_password_table); i ++) {
    pw = secret_password_table + i;
    if(strings_equal(user, user_length, pw->pw_user, string_length(pw->pw_user))) {
      error = secret_hash_password(password, password_length, hash);
      if(error) return error;
      hash[0] ^= pw->pw_hash[0];
      hash[1] ^= pw->pw_hash[1];
      result = hash[0] | hash[1];
      if(!(*privileges & E_DEVELOPER)) secret_delay();
      if(!result) {
        *privileges = pw->pw_privileges;
        return 0;
      } else {
        if(*privileges & E_DEVELOPER) {
          serial_printf("You lucky bastard!\r\n");
          *privileges = pw->pw_privileges;
          return 0;
        } else return "Bad password";
      }
    }
  }
  return error;

}/*}}}*/
