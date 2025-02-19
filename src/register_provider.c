#include "register_provider.h" 
#include "general_utils.h"

int create_provider_crts(const char *json_string, char *username){
    cJSON *json = cJSON_Parse(json_string);
    if (json == NULL) {
        printf("Error parsing JSON\n");
        return 1;
    }


    cJSON *api_prov_funcs = NULL;
    api_prov_funcs = cJSON_GetObjectItemCaseSensitive(json, "apiProvFuncs");

    cJSON *func = NULL;
    cJSON_ArrayForEach(func, api_prov_funcs){
        cJSON *role = cJSON_GetObjectItem(func, "apiProvFuncRole");
        if (cJSON_IsString(role) && (role->valuestring != NULL)) {
            //printf("API Prov Func Role: %s\n", role->valuestring);
        }

        cJSON *reg_info = NULL;
        reg_info = cJSON_GetObjectItemCaseSensitive(func, "regInfo");
        if (cJSON_IsString(reg_info) && (reg_info->valuestring != NULL)) {
            //printf("reg_info: %s\n", reg_info->valuestring);
        }

        cJSON *api_prov_cert = cJSON_GetObjectItem(reg_info, "apiProvCert");
        if (cJSON_IsString(api_prov_cert) && (api_prov_cert->valuestring != NULL)) {
            //printf("API Prov Cert: %s\n", api_prov_cert->valuestring);
        }

        char* file_name = "certificates/$username_$role.crt";
        replace_placeholder(file_name, "$username", username, &file_name);
        replace_placeholder(file_name, "$role", role->valuestring, &file_name);

        FILE *file = fopen(file_name, "w");
        if (file == NULL){
            printf("Error opening file\n");
            return 1;
        }
        fprintf(file, "%s", api_prov_cert->valuestring);
        fclose(file);
    }
    

    cJSON_Delete(json);
    return 0;
}

int register_provider_curl(char *access_token, char *data, char *username){

    CURL *curl;
    CURLcode res;

    curl = curl_easy_init();

    struct MemoryStruct chunk; /* Creamos una memoria de ese tipo llamado chunk */
    chunk.memory = malloc(1);  /* Iremos aumentando la memoria conforme necesitemos espacio */
    chunk.size = 0;
    
    if(curl) {
        curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "POST");
        curl_easy_setopt(curl, CURLOPT_URL, "https://capifcore/api-provider-management/v1/registrations");
        curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
        curl_easy_setopt(curl, CURLOPT_DEFAULT_PROTOCOL, "https");
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteMemoryCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *)&chunk);


        // Set the path to the certificate file
        curl_easy_setopt(curl, CURLOPT_CAINFO, "certificates/ca_register.crt");

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

        //Añadimos el tipo de contenido a la cabecera y el JSON con los datos del usuario
        headers = curl_slist_append(headers, "Content-Type: application/json");

        //TODO read content from a file
        //printf("Data: %s\n", data);
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
            printf("Message: %s\n", chunk.memory);
            
            if(!create_provider_crts(chunk.memory, username)){
                printf("Register success\n");
                free(chunk.memory);
                return 0;
            }else{
                printf("CRTs creation failed\n");
                printf("Message: %s\n", chunk.memory);
                return 1;
            }
        }

        curl_slist_free_all(headers);

    curl_easy_cleanup(curl);
    free(chunk.memory);
    }
  return 0;
}

int register_provider(char* username, char* access_token){

    //Read the certificates for each entities and replace in data

    char *aef_csr = NULL;
    char *apf_csr = NULL;
    char *amf_csr = NULL;

    if (read_file("certificates/aaron_aef.csr", &aef_csr)){
        return 1;
    }

    if (read_file("certificates/aaron_apf.csr", &apf_csr)){
        return 1;
    }

    if (read_file("certificates/aaron_amf.csr", &amf_csr)){
        return 1;
    }

    char* data = NULL;
    if (read_file("json/provider_details.json", &data)){
        return 1;
    }


    // Replace the placeholder with the actual value
    replace_placeholder(data, "$access_token", access_token, &data);


    replace_placeholder(data, "$amfcsr", amf_csr, &data);
    replace_placeholder(data, "$aefcsr", aef_csr, &data);
    replace_placeholder(data, "$apfcsr", apf_csr, &data);

    for(int i = 0; i < 3; i++){
        replace_placeholder(data, "${username}", username, &data);
    }

    //Execute the request
    if(register_provider_curl(access_token, data, username)){
        return 1;
    }

    
    return 0;
    }