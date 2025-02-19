#ifndef GENERAL_UTILS_H
#define GENERAL_UTILS_H


#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "cJSON.h"

struct MemoryStruct {
    char *memory;
    size_t size;
};

size_t WriteMemoryCallback(void *contents, size_t size, size_t nmemb, void *userp);
int read_file(const char *filename, char **file_contents);
void replace_placeholder(char *data, const char *placeholder, const char *replace, char **result);

#endif // GENERAL_UTILS_H