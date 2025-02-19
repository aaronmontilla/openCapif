#include "general_utils.h"

/* Esta es la funcion que salta cuando hemos terminado de recibir el contenido de la web y estructura nuestra memoria conforme necesitemos alojar más datos */
size_t WriteMemoryCallback(void *contents, size_t size, size_t nmemb, void *userp)
{
    size_t realsize = size * nmemb;
    struct MemoryStruct *mem = (struct MemoryStruct *)userp;
    
    mem->memory = realloc(mem->memory, mem->size + realsize + 1);
    if(mem->memory == NULL) {
        /* out of memory! */
        printf("not enough memory (realloc returned NULL)\n");
        return 0;
    }
    
    memcpy(&(mem->memory[mem->size]), contents, realsize);
    mem->size += realsize;
    mem->memory[mem->size] = 0;
    
    return realsize;
}

int read_file(const char *filename, char **file_contents){

    //Read from file
    FILE *file = fopen(filename, "r");
    if (file == NULL){
        printf("Error opening file\n");
        return 1;
    }

    //Get the size of the file
    fseek(file, 0, SEEK_END);
    size_t file_size = ftell(file);
    fseek(file, 0, SEEK_SET);

    //Allocate memory for the file
    *file_contents = malloc(file_size + 1);
    if (*file_contents == NULL){
        printf("Memory allocation failed for file_contents\n");
        fclose(file);
        return 1;
    }

    //Read the file
    size_t read_size = fread(*file_contents, 1, file_size, file);
    if (read_size != file_size){
        printf("Error reading file\n");
        free(*file_contents);
        fclose(file);
        return 1;
    }

    

    // Null-terminate the file contents
    (*file_contents)[file_size] = '\0';

    //Close the file
    fclose(file);
    return 0;
}

void replace_placeholder(char *template, const char *placeholder, const char *replace, char **result) {

    size_t new_size = strlen(template) + strlen(replace) - strlen(placeholder) + 1;
    *result = malloc(new_size);
    if (*result == NULL) {
        perror("Memory allocation failed");
        return;
    }

    char *pos = strstr(template, placeholder);
    if (pos == NULL) {
        strcpy(*result, template);
        return;
    }

    size_t before_len = pos - template;
    strncpy(*result, template, before_len);
    (*result)[before_len] = '\0';
    strcat(*result, replace);
    strcat(*result, pos + strlen(placeholder));
}