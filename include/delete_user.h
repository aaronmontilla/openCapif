#ifndef DELETE_USER_H
#define DELETE_USER_H

#include <curl/curl.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "cJSON.h"
#include "opencapif_process.h"


// Function declarations and other definitions can go here
int delete_user( char* uuid, char* access_token);


#endif // DELETE_USER_H