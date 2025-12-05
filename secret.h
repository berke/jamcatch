/* secret.h
 *
 * Copyright (C)2007 Therodox R&D
 * All rights reserved
 */

#ifndef SECRET_H
#define SECRET_H

#include "types.h"

#define E_DEVELOPER 1
#define E_CLIENT 2
#define E_INTEGRATOR 4
#define E_MAINTAINER 8
#define E_USER 16
#define E_STRANGER 32

#define AT_LEAST(x) (2 * (x) - 1)
#define EVERYBODY AT_LEAST(E_STRANGER)

#if BETA
#define IF_BETA(x,y) x
#else
#define IF_BETA(x,y) y
#endif

#define INITIAL_PRIVILEGE_LEVEL IF_BETA(E_DEVELOPER, E_STRANGER)

typedef struct {
  string pw_user;
  u32 pw_hash[2];
  u32 pw_privileges;
} password_t;

char *secret_hash_password(char *password, u32 length, u32 *hash);
void secret_delay(void);
char *secret_attempt_login(char *user, u32 user_length, char *password, u32 password_length, u32 *privileges);

#endif
