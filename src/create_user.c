#include "create_user.h" 
#include "general_utils.h"

// Example function to parse JSON
int parse_login_admin(const char *json_string, char **access_token, char **refresh_token) {
    cJSON *json = cJSON_Parse(json_string);
    if (json == NULL) {
        printf("Error parsing JSON\n");
        return 1;
    }

    //Parse access_token
    cJSON *a_token = cJSON_GetObjectItem(json, "access_token");
    if (cJSON_IsString(a_token) && (a_token->valuestring != NULL)) {
        *access_token = malloc(strlen(a_token->valuestring) + 1);
        if (*access_token == NULL) {
            printf("Memory allocation failed for access_token\n");
            cJSON_Delete(json);
            return 1;
        }
        strcpy(*access_token, a_token->valuestring);
    }


    //Parse refresh_token
    cJSON *r_token = cJSON_GetObjectItem(json, "refresh_token");
    if (cJSON_IsString(r_token) && (r_token->valuestring != NULL)) {
        *refresh_token = malloc(strlen(r_token->valuestring) + 1);
        if (*refresh_token == NULL) {
            printf("Memory allocation failed for refresh_token\n");
            cJSON_Delete(json);
            return 1;
        }
        strcpy(*refresh_token, r_token->valuestring);
    }

    cJSON_Delete(json);
    return 0;
}

// Example function to parse JSON
int parse_uuid(const char *json_string, char **uuid) {
    cJSON *json = cJSON_Parse(json_string);
    if (json == NULL) {
        printf("Error parsing JSON\n");
        return 1;
    }

    //Parse uuid
    cJSON *uuid_json = cJSON_GetObjectItem(json, "uuid");
    if (cJSON_IsString(uuid_json) && (uuid_json->valuestring != NULL)) {
        *uuid = malloc(strlen(uuid_json->valuestring) + 1);
        if (*uuid == NULL) {
            printf("Memory allocation failed for uuid\n");
            cJSON_Delete(json);
            return 1;
        }
        strcpy(*uuid, uuid_json->valuestring);
        return 0;
    }

    cJSON_Delete(json);
    return 1;
}

int login_admin_curl(char **access_token, char **refresh_token){
    //CURL es la biblioteca que permite conectarse a internet
    CURL *curl;
    //RES es donde almacenamos el estado de la peticion
    CURLcode res;
    
    struct MemoryStruct chunk; /* Creamos una memoria de ese tipo llamado chunk */
    
    chunk.memory = malloc(1);  /* Iremos aumentando la memoria conforme necesitemos espacio */
    chunk.size = 0;
    
    //Iniciamos curl con el metodo de la biblioteca.
    curl = curl_easy_init();

    if(curl) {
        curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "POST");
        curl_easy_setopt(curl, CURLOPT_URL, "https://admin:password123@register:8084/login");
        curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
        curl_easy_setopt(curl, CURLOPT_DEFAULT_PROTOCOL, "https");

        //Le decimos que queremos que todo lo que devuelve la petición pase por la funcion WriteMemoryCallback

            curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteMemoryCallback);
            //Y los datos a guardar los tiene que escribir en nuestra estructura de memoria, llamada chunk, por ello se la pasamos por referencia (&), por tal de que pueda modificarla.
            curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *)&chunk);


        // Set the path to the certificate file
        curl_easy_setopt(curl, CURLOPT_CAINFO, "certificates/ca_register.crt");

        // Ensure libcurl verifies the peer's SSL certificate
        curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 1L);


        struct curl_slist *headers = NULL;
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
        res = curl_easy_perform(curl);
        curl_easy_cleanup(curl);
  
    
        //Si ha ido mal...
        if(res != CURLE_OK){
            //Mostramos un mensaje diciendo por qué ha ido mal
            fprintf(stderr, "curl_easy_perform() falló: %s\n",
            curl_easy_strerror(res));
            return 1;
        }else{
            //Si todo ha ido bien, podemos hacer cosas con lo que hay en nuestra memoria. En este caso, mostrarla por pantalla:
            if(!parse_login_admin(chunk.memory, access_token, refresh_token)){
                free(chunk.memory);
                return 0;
            }
        }
            
        free(chunk.memory);
    }else{
        return 1;
    }
    return 0;
}

int create_user_curl(char *access_token, char* username, char* password, char **uuid){

    CURL *curl;
    CURLcode res;

    curl = curl_easy_init();

    struct MemoryStruct chunk; /* Creamos una memoria de ese tipo llamado chunk */
    chunk.memory = malloc(1);  /* Iremos aumentando la memoria conforme necesitemos espacio */
    chunk.size = 0;
    
    if(curl) {
        curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "POST");
        curl_easy_setopt(curl, CURLOPT_URL, "https://register:8084/createUser");
        curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
        curl_easy_setopt(curl, CURLOPT_DEFAULT_PROTOCOL, "https");
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteMemoryCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *)&chunk);


        // Set the path to the certificate file
        curl_easy_setopt(curl, CURLOPT_CAINFO, "certificates/ca_register.crt");

        // Ensure libcurl verifies the peer's SSL certificate
        curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 1L);

        struct curl_slist *headers = NULL;
        
        //Añadimos el token de acceso a la cabecera
        char *authorization = malloc(strlen("Authorization: Bearer ") + strlen(access_token) + 1);
        if (authorization == NULL) {
            printf("Memory allocation failed for authorization\n");
            return 1;
        }
        sprintf(authorization, "Authorization: Bearer %s", access_token);
        headers = curl_slist_append(headers, authorization);

        //Añadimos el tipo de contenido a la cabecera y el JSON con los datos del usuario
        headers = curl_slist_append(headers, "Content-Type: application/json");

        char* data = NULL;
        read_file("json/create_user.json", &data);
        replace_placeholder(data, "rp_username", username, &data);
        replace_placeholder(data, "rp_password", password, &data);

        
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, data);
        
        
        //Unir los headers a la petición y realizar la petición
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
        res = curl_easy_perform(curl);
    
        if(res != CURLE_OK){
            //Mostramos un mensaje diciendo por qué ha ido mal
            fprintf(stderr, "curl_easy_perform() falló: %s\n",
                    curl_easy_strerror(res));
        }else{
            //Si todo ha ido bien, podemos hacer cosas con lo que hay en nuestra memoria. En este caso, mostrarla por pantalla:
            if(!parse_uuid(chunk.memory, uuid)){
                free(chunk.memory);
                return 0;
            }else{
                printf("User creation failed\n");
                printf("Message: %s\n", chunk.memory);
                return 1;
            }
        }

        curl_slist_free_all(headers);

    curl_easy_cleanup(curl);
    free(chunk.memory);
    free(authorization);
    free(data);
    }
    else{
        printf("Error initializing curl\n");
        return 1;
    }
  return 0;
}
