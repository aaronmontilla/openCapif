#ifndef PUBLISH_API_H
#define PUBLISH_API_H

#include <curl/curl.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "cJSON.h"
#include "opencapif_process.h"


// Function declarations and other definitions can go here
int publish_api(char* aefid, char* apfid);


#endif // PUBLISH_API_H