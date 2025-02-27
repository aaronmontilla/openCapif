#include "retrieve_apis.h" 
#include "general_utils.h"

int retrieve_apis(char* username, char *provider_id){

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
      curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "GET");

      char* url = "https://capifcore/published-apis/v1/$provider_id/service-apis";
      replace_placeholder(url, "$provider_id", provider_id, &url);

      curl_easy_setopt(curl, CURLOPT_URL, url);
      curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
      curl_easy_setopt(curl, CURLOPT_DEFAULT_PROTOCOL, "https");
      curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteMemoryCallback);
      curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *)&chunk);


      // Set the path to the certificate file
      curl_easy_setopt(curl, CURLOPT_CAINFO, "certificates/ca_capifcore.crt");

      // Ensure libcurl verifies the peer's SSL certificate
      curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L);

      // Set the client certificate
      char* crt_path = "certificates/$username_APF.crt";
      replace_placeholder(crt_path, "$username", username, &crt_path);
      curl_easy_setopt(curl, CURLOPT_SSLCERT, crt_path);


      // Set the client private key
      char* key_path = "certificates/$username_apf.key";
      replace_placeholder(key_path, "$username", username, &key_path);
      curl_easy_setopt(curl, CURLOPT_SSLKEY, key_path);

      struct curl_slist *headers = NULL;

      //Unir los headers a la petición y realizar la petición
      curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);

      //curl_easy_setopt(curl, CURLOPT_VERBOSE, 1L);
      res = curl_easy_perform(curl);
  
      if(res != CURLE_OK){
          //Mostramos un mensaje diciendo por qué ha ido mal
          fprintf(stderr, "curl_easy_perform() falló: %s\n",
                  curl_easy_strerror(res));
      }else{
              //Check the response code
              long response_code;
              curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &response_code);
              if(response_code == 204){
                printf("The user has been removed\n");
              }else {
                printf("Message: %s\n", chunk.memory);
              }
              
      }

    curl_slist_free_all(headers);
    curl_easy_cleanup(curl);
    free(chunk.memory);
    free(url);
    free(crt_path);
    free(key_path);
    }
    else{
      free(chunk.memory);
      return 1;
    }
  return 0;
}

