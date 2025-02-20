#ifndef DELETE_PROVIDER_H
#define DELETE_PROVIDER_H

#include <curl/curl.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "cJSON.h"
#include "opencapif_process.h"


// Function declarations and other definitions can go here
int delete_provider( char* username, char* provider_id);


#endif // DELETE_PROVIDER_H