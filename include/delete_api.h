#ifndef DELETE_API_H
#define DELETE_API_H

#include <curl/curl.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "cJSON.h"
#include "opencapif_process.h"


// Function declarations and other definitions can go here
int delete_api( char* username, char* provider_id, char* api_id);


#endif // DELETE_API_H