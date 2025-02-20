#ifndef RETRIEVE_APIS_H
#define RETRIEVE_APIS_H

#include <curl/curl.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "cJSON.h"
#include "opencapif_process.h"


// Function declarations and other definitions can go here
int retrieve_apis( char* username, char* provider_id);


#endif // RETRIEVE_APIS