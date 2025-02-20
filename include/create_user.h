#ifndef CREATE_USER_H
#define CREATE_USER_H

#include <curl/curl.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "cJSON.h"

/* Este bloque define una estructura preparada para almacenar una memoria, que contiene caracteres, y un tamaño. La usaremos para guardar la peticion */


int parse_login_admin(const char *json_string, char **access_token, char **refresh_token);
int parse_uuid(const char *json_string, char **uuid);
int login_admin_curl(char **access_token, char **refresh_token);
int create_user_curl(char *access_token, char* username, char* password, char **uuid);

#endif // CREATE_USER_H