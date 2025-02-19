#include "getauth.h" 
#include "general_utils.h"



int parse_auth_creds(const char *json_string, struct AuthCreds *auth_creds) {
    cJSON *json = cJSON_Parse(json_string);
    if (json == NULL) {
        printf("Error parsing JSON\n");
        return 1;
    }

    //Parse access_token
    cJSON *access_token_json = cJSON_GetObjectItem(json, "access_token");
    if (cJSON_IsString(access_token_json) && (access_token_json->valuestring != NULL)) {
        auth_creds->access_token = malloc(strlen(access_token_json->valuestring) + 1);
        if (auth_creds->access_token == NULL) {
            printf("Memory allocation failed for access_token\n");
            cJSON_Delete(json);
            return 1;
        }
        strcpy(auth_creds->access_token, access_token_json->valuestring);
    }

    //Parse ca_root
    cJSON *ca_root_json = cJSON_GetObjectItem(json, "ca_root");
    if (cJSON_IsString(ca_root_json) && (ca_root_json->valuestring != NULL)) {
        auth_creds->ca_root = malloc(strlen(ca_root_json->valuestring) + 1);
        if (auth_creds->ca_root == NULL) {
            printf("Memory allocation failed for ca_root\n");
            cJSON_Delete(json);
            return 1;
        }
        strcpy(auth_creds->ca_root, ca_root_json->valuestring);
    }

    //Parse message
    cJSON *message_json = cJSON_GetObjectItem(json, "message");
    if (cJSON_IsString(message_json) && (message_json->valuestring != NULL)) {
        auth_creds->message = malloc(strlen(message_json->valuestring) + 1);
        if (auth_creds->message == NULL) {
            printf("Memory allocation failed for message\n");
            cJSON_Delete(json);
            return 1;
        }
        strcpy(auth_creds->message, message_json->valuestring);
    }

    //Parse ccf_api_onboarding_url
    cJSON *ccf_api_onboarding_url_json = cJSON_GetObjectItem(json, "ccf_api_onboarding_url");
    if (cJSON_IsString(ccf_api_onboarding_url_json) && (ccf_api_onboarding_url_json->valuestring != NULL)) {
        auth_creds->ccf_api_onboarding_url = malloc(strlen(ccf_api_onboarding_url_json->valuestring) + 1);
        if (auth_creds->ccf_api_onboarding_url == NULL) {
            printf("Memory allocation failed for ccf_api_onboarding_url\n");
            cJSON_Delete(json);
            return 1;
        }
        strcpy(auth_creds->ccf_api_onboarding_url, ccf_api_onboarding_url_json->valuestring);
    }

    //Parse ccf_discover_url
    cJSON *ccf_discover_url_json = cJSON_GetObjectItem(json, "ccf_discover_url");
    if (cJSON_IsString(ccf_discover_url_json) && (ccf_discover_url_json->valuestring != NULL)) {
        auth_creds->ccf_discover_url = malloc(strlen(ccf_discover_url_json->valuestring) + 1);
        if (auth_creds->ccf_discover_url == NULL) {
            printf("Memory allocation failed for ccf_discover_url\n");
            cJSON_Delete(json);
            return 1;
        }
        strcpy(auth_creds->ccf_discover_url, ccf_discover_url_json->valuestring);
    }
    
    //Parse ccf_onboarding_url
    cJSON *ccf_onboarding_url_json = cJSON_GetObjectItem(json, "ccf_onboarding_url");
    if (cJSON_IsString(ccf_onboarding_url_json) && (ccf_onboarding_url_json->valuestring != NULL)) {
        auth_creds->ccf_onboarding_url = malloc(strlen(ccf_onboarding_url_json->valuestring) + 1);
        if (auth_creds->ccf_onboarding_url == NULL) {
            printf("Memory allocation failed for ccf_onboarding_url\n");
            cJSON_Delete(json);
            return 1;
        }
        strcpy(auth_creds->ccf_onboarding_url, ccf_onboarding_url_json->valuestring);
    }

    //Parse ccf_publish_url
    cJSON *ccf_publish_url_json = cJSON_GetObjectItem(json, "ccf_publish_url");
    if (cJSON_IsString(ccf_publish_url_json) && (ccf_publish_url_json->valuestring != NULL)) {
        auth_creds->ccf_publish_url = malloc(strlen(ccf_publish_url_json->valuestring) + 1);
        if (auth_creds->ccf_publish_url == NULL) {
            printf("Memory allocation failed for ccf_publish_url\n");
            cJSON_Delete(json);
            return 1;
        }
        strcpy(auth_creds->ccf_publish_url, ccf_publish_url_json->valuestring);
    }

    //Parse ccf_security_url
    cJSON *ccf_security_url_json = cJSON_GetObjectItem(json, "ccf_security_url");
    if (cJSON_IsString(ccf_security_url_json) && (ccf_security_url_json->valuestring != NULL)) {
        auth_creds->ccf_security_url = malloc(strlen(ccf_security_url_json->valuestring) + 1);
        if (auth_creds->ccf_security_url == NULL) {
            printf("Memory allocation failed for ccf_security_url\n");
            cJSON_Delete(json);
            return 1;
        }
        strcpy(auth_creds->ccf_security_url, ccf_security_url_json->valuestring);
    }

    cJSON_Delete(json);
    return 0;
}




int get_auth_curl(char* url, struct AuthCreds *auth_cred){
    CURL *curl;
    CURLcode res;

    curl = curl_easy_init();

    struct MemoryStruct chunk; /* Creamos una memoria de ese tipo llamado chunk */
    
    chunk.memory = malloc(1);  /* Iremos aumentando la memoria conforme necesitemos espacio */
    chunk.size = 0;
    if(curl) {
        curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "GET");
        curl_easy_setopt(curl, CURLOPT_URL, url);
        curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
        curl_easy_setopt(curl, CURLOPT_DEFAULT_PROTOCOL, "https");
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteMemoryCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *)&chunk);

        // Ponemos esta opción a 0 porque usa el -k en la petición del curl
        curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L);

        res = curl_easy_perform(curl);

        if(res != CURLE_OK){
            //Mostramos un mensaje diciendo por qué ha ido mal
            fprintf(stderr, "curl_easy_perform() falló: %s\n",
                    curl_easy_strerror(res));
                    return 1;
        }else{
            if(!parse_auth_creds(chunk.memory, auth_cred)){
            /**/
            printf("Access Token: %s\n", auth_cred->access_token);
            printf("CA Root: %s\n", auth_cred->ca_root);
            printf("Message: %s\n", auth_cred->message);
            printf("CCF API Onboarding URL: %s\n", auth_cred->ccf_api_onboarding_url);
            printf("CCF Discover URL: %s\n", auth_cred->ccf_discover_url);
            printf("CCF Onboarding URL: %s\n", auth_cred->ccf_onboarding_url);
            printf("CCF Publish URL: %s\n", auth_cred->ccf_publish_url);
            printf("CCF Security URL: %s\n", auth_cred->ccf_security_url);
            }
        
        }

        



    curl_easy_cleanup(curl);
    free(chunk.memory);
    }
    return 0;
}
