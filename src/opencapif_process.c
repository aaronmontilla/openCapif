
#include "opencapif_process.h" 
#include "create_user.h" 
#include "getauth.h" 
#include "register_provider.h"
#include "publish_api.h"

int register_provider_process(char* username, char* password){
    //Inicializamos los JSONs del admin login
    char *access_token = NULL;
    char *refresh_token = NULL;

    //Inicializamos parámetros para la creación de usuario
    char *uuid = NULL;

    //Inicializamos parámetros para get_auth
    struct AuthCreds *auth_creds = malloc(sizeof(struct AuthCreds*));
    if (auth_creds == NULL) {
        printf("Memory allocation failed for authorization\n");
    }

    char *url = malloc(strlen("https://") + strlen(username) + strlen(":") + strlen(password) + strlen("@register:8084/getauth") + 1);
        if (url == NULL) {
            printf("Memory allocation failed for authorization\n");
            return 1;
        }

    sprintf(url,"https://%s:%s@register:8084/getauth", username, password);
    //printf("URL: %s\n", url);

    

    //Login admin

    if(login_admin_curl(&access_token, &refresh_token)){
        printf("Admin login failed\n");
        return 1;
    }
    printf("Admin logged in\n");
    printf("Access token: %s\n", access_token);
    printf("Refresh token: %s\n", refresh_token);

    //Create user
    if(create_user_curl(access_token, &uuid)){
        printf("User creation failed\n");
    }
    //printf("User created, uuid: %s\n", uuid);
   
    //Get Auth
    printf("Getting auth\n");
    if(get_auth_curl(url, auth_creds)){
        printf("Error getting auth\n");
        return 1;
    }
   
    //Register provider
    printf("Registering provider\n");
    if(register_provider(username, auth_creds->access_token)){
        printf("Provider registration failed\n");
        return 1;
    }
    return 0;
}

int main(){


    system("clear");

    printf("Welcome to the OpenCAPIF provider client\n");
    printf("This client will help you to publish your API in the OpenCAPIF\n");
    printf("Please, introduce your credentials\n");

    char *username = malloc(50);
    char *password = malloc(50);

    printf("Username: ");
    scanf("%s", username);
    printf("Password: ");
    scanf("%s", password);

    int option;

    while(option != 5){
    printf("Choose one of the following options:\n");
    printf("1. Register provider\n");
    printf("2. Delete provider\n");
    printf("3. Publish API\n");
    printf("4. Delete API\n");
    printf("5. Exit\n");
    
    scanf("%d", &option);
    
        switch (option)
            {
            case 1:
                printf ("Register provider\n");
                if(register_provider_process(username, password)){
                    printf("Error creating user\n");
                }
                break;
            case 2:
                printf ("Delete provider\n");
                printf("Please provide provider ID:");
                char *provider_id = malloc(50);
                scanf("%s", provider_id);
                /*
                if(delete_provider(provider_id)){
                    printf("Error deleting provider\n");
                }*/
                break;

            case 3:
                printf ("Publish API\n");
                printf("Please provide APF ID:");
                char *apf_id = malloc(50);
                scanf("%s", apf_id);
                printf("Please provide AEF ID:");
                char *aef_id = malloc(50);
                scanf("%s", aef_id);
                publish_api(aef_id, apf_id);
                break;

            case 4:
                printf ("Delete API\n");
                printf("Please provide API ID:");
                char *api_id = malloc(50);
                scanf("%s", api_id);
                /*
                if(delete_api(api_id)){
                    printf("Error deleting API\n");
                }*/
                break;
            
            case 5:
                printf ("Thanks foor using OpenCAPIF provider client\n");
                break;
            
            default:
                printf("Invalid option\n");
                break;
            }
    }

}
