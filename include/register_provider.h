#ifndef REGISTER_PROVIDER_H
#define REGISTER_PROVIDER_H

#include <curl/curl.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "cJSON.h"
#include "opencapif_process.h"


// Function declarations and other definitions can go here
int register_provider(char* username, char* access_token);


#endif // REGISTER_PROVIDER_H