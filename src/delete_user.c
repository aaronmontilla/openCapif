#include "delete_user.h" 
#include "general_utils.h"

int delete_user(char *uuid, char* access_token){

    CURL *curl;
    CURLcode res;

    curl_global_init(CURL_GLOBAL_DEFAULT);
    curl = curl_easy_init();

    struct MemoryStruct chunk; /* Creamos una memoria de ese tipo llamado chunk */
    chunk.memory = malloc(1);  /* Iremos aumentando la memoria conforme necesitemos espacio */
    //inicializamos memoria a null
    chunk.memory[0] = '\0';
    chunk.size = 0;
    
    if(curl) {
        curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "DELETE");

        char* url = "https://register:8084/deleteUser/$uuid";
        replace_placeholder(url, "$uuid", uuid, &url);

        curl_easy_setopt(curl, CURLOPT_URL, url);
        curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
        curl_easy_setopt(curl, CURLOPT_DEFAULT_PROTOCOL, "https");
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteMemoryCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *)&chunk);


        // Set the path to the certificate file
        curl_easy_setopt(curl, CURLOPT_CAINFO, "certificates/ca_capifcore.crt");

        // Ensure libcurl verifies the peer's SSL certificate
        curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L);

        struct curl_slist *headers = NULL;

        //Añadimos el token de acceso a la cabecera
        char *authorization = malloc(strlen("Authorization: Bearer ") + strlen(access_token) + 1);
        if (authorization == NULL) {
            printf("Memory allocation failed for authorization\n");
            return 1;
        }
        sprintf(authorization, "Authorization: Bearer %s", access_token);
        headers = curl_slist_append(headers, authorization);

        //Unir los headers a la petición y realizar la petición
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);

        curl_easy_setopt(curl, CURLOPT_VERBOSE, 1L);
        res = curl_easy_perform(curl);
    
        if(res != CURLE_OK){
            //Mostramos un mensaje diciendo por qué ha ido mal
            fprintf(stderr, "curl_easy_perform() falló: %s\n",
                    curl_easy_strerror(res));
        }else{
            
                printf("Message: %s\n", chunk.memory);
        }

        curl_slist_free_all(headers);

    curl_easy_cleanup(curl);
    free(chunk.memory);
    free(authorization);
    free(url);
    }
    else{
        printf("Error initializing curl\n");
        return 1;
    }
  return 0;
}

