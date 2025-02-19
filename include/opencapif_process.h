#ifndef OPENCAPIF_PROCESS_H
#define OPENCAPIF_PROCESS_H

#include <curl/curl.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "cJSON.h"

/* Este bloque define una estructura preparada para almacenar una memoria, que contiene caracteres, y un tamaño. La usaremos para guardar la peticion */

struct AuthCreds {
    char *access_token;
    char *ca_root;
    char *message;
    char *ccf_api_onboarding_url;
    char *ccf_discover_url;
    char *ccf_onboarding_url;
    char *ccf_publish_url;
    char *ccf_security_url;
};



// Function declarations and other definitions can go here

#endif // OPENCAPIF_PROCESS_H