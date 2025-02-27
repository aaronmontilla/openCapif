
#include "opencapif_process.h" 
#include "create_user.h" 
#include "delete_user.h"
#include "getauth.h" 
#include "register_provider.h"
#include "publish_api.h"
#include "delete_provider.h"
#include "delete_api.h"
#include "retrieve_apis.h"

int register_provider_process(char* username, char* password){
    //Inicializamos los JSONs del admin login
    char *access_token = NULL;
    char *refresh_token = NULL;

    //Inicializamos parámetros para la creación de usuario
    char *uuid = NULL;

    //Inicializamos parámetros para get_auth
    struct AuthCreds *auth_creds = malloc(sizeof(struct AuthCreds));
    if (auth_creds == NULL) {
        printf("Memory allocation failed for authorization\n");
    }   

    //open a tmp file to store the credentials
    FILE *tmp_file = fopen("tmp", "w");
    if (tmp_file == NULL) {
        printf("Error opening file!\n");
        return 1;
    }
    fprintf(tmp_file, "Username: %s\n Password: %s\n", username, password);

    //Login admin

    if(login_admin_curl(&access_token, &refresh_token)){
        free(auth_creds);
        printf("Admin login failed\n");
        return 1;
    }
    printf("Admin logged in\n");
    printf("Access token: %s\n", access_token);
    printf("Refresh token: %s\n", refresh_token);

    fprintf(tmp_file, "Access token: %s\n Refresh token: %s\n", access_token, refresh_token);

    //Create user
    if(create_user_curl(access_token,  username, password, &uuid)){
        printf("User creation failed\n");
    }

    fprintf(tmp_file, "UUID: %s\n", uuid);

    free(access_token);
    free(refresh_token);
    free(uuid);

    char *url = malloc(strlen("https://") + strlen(username) + strlen(":") + strlen(password) + strlen("@register:8084/getauth") + 1);
        if (url == NULL) {
            printf("Memory allocation failed for authorization\n");
            free(auth_creds);
            return 1;
        }

    sprintf(url,"https://%s:%s@register:8084/getauth", username, password);
   
    //Get Auth
    printf("Getting auth\n");
    if(get_auth_curl(url, auth_creds)){
        printf("Error getting auth\n");
        free(auth_creds);
        free(url);
        return 1;
    }
    free(url);
   
    //Register provider
    printf("Registering provider\n");
    
    if(register_provider(username, auth_creds->access_token)){
        printf("Provider registration failed\n");
        free(auth_creds);
        return 1;
    }
    fprintf(tmp_file, "Acces auth creds: %s\n", auth_creds->access_token);
    fclose(tmp_file);
    
    free(auth_creds);
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
    char *provider_id = NULL;
    char *uuid = NULL;
    char *access_token = NULL;

    while(option != 7){
    printf("Choose one of the following options:\n");
    printf("1. Register provider\n");
    printf("2. Delete user\n");
    printf("3. Delete provider\n");
    printf("4. Publish API\n");
    printf("5. Delete API\n");
    printf("6. Get all APIs from a provider\n");
    printf("7. Exit\n");
    
    scanf("%d", &option);
    
        switch (option)
            {
            case 1:
                printf ("Register provider\n");
                if(register_provider_process(username, password)){
                    printf("Error registering provider\n");
                }
                break;
            case 2:
                printf ("Deleting user\n");
                /**/
                uuid = malloc(50);
                printf("Please provide UUID:");
                scanf("%s", uuid);
                access_token = malloc(200);
                printf("Please provide access_token:");
                scanf("%s", access_token);
                if(delete_user(uuid, access_token)){
                    printf("Error registering provider\n");
                }
                free(uuid);
                free(access_token);
                printf("Not implemented yet\n");
                break;
            case 3:
                printf ("Delete provider\n");
                provider_id = malloc(50);
                printf("Please provide provider ID:");
                scanf("%s", provider_id);
                if(delete_provider(username, provider_id)){
                    printf("Error deleting provider\n");
                }
                free(provider_id);
                break;

            case 4:
                printf ("Publish API\n");
                printf("Please provide APF ID:");
                char *apf_id = malloc(50);
                scanf("%s", apf_id);
                printf("Please provide AEF ID:");
                char *aef_id = malloc(50);
                scanf("%s", aef_id);
                if(publish_api(username, aef_id, apf_id)){
                    printf("Error publishing API\n");
                }
                free(apf_id);
                free(aef_id);
                break;

            case 5:
                printf ("Delete API\n");
                provider_id = malloc(50);
                printf("Please provide publisher ID:");
                scanf("%s", provider_id);
                printf("Please provide API ID:");
                char *api_id = malloc(50);
                scanf("%s", api_id);
                if(delete_api(username, provider_id, api_id)){
                    printf("Error deleting API\n");
                }
                free(provider_id);
                free(api_id);
                break;
            
            case 6:
                printf ("Get all APIs from a provider\n");
                provider_id = malloc(50);
                printf("Please provide publisher ID:");
                scanf("%s", provider_id);
                if(retrieve_apis(username, provider_id)){
                    printf("Error retrieving APIs\n");
                }
                free(provider_id);
                break;
            
            case 7:
                printf ("Thanks for using OpenCAPIF provider client\n");
                free(username);
                free(password);
                break;
            
            default:
                printf("Invalid option\n");
                break;
            }
    }

}
