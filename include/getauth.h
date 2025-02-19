#ifndef GETAUTH_H
#define GETAUTH_H

#include <curl/curl.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "cJSON.h"
#include "opencapif_process.h"


int parse_auth_creds(const char *json_string, struct AuthCreds *auth_creds);
int get_auth_curl(char* url, struct AuthCreds *auth_cred);


// Function declarations and other definitions can go here

#endif // GETAUTH_H