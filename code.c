#include <stdio.h>
#include <stdlib.h>
#include <curl/curl.h>
#include <string.h>
#include <json-c/json.h>

//function to handle curl response
size_t write_callback(char *ptr, size_t size, size_t nmemb, void *userdata)
{
    strcat(userdata, ptr);
    return size * nmemb;
}

int main()
{
    CURL *curl;
    CURLcode res;
    char url[100] = "http://api.weatherapi.com/v1/current.json?key=cf21e941c7174c76854114118231804&q=";
    char city[20];
    char final_url[150];
    char response[1000];
    struct json_object *json, *temp_c, *humidity, *description;
    response[0] = '\0';
    printf("Enter city name: ");
    scanf("%s",city);
    strcat(url, city);

    curl = curl_easy_init();
    if(curl) {
        strcpy(final_url, url);
        curl_easy_setopt(curl, CURLOPT_URL, final_url);
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_callback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, response);
        res = curl_easy_perform(curl);

        if(res != CURLE_OK) {
            printf("Error: %s\n", curl_easy_strerror(res));
            curl_easy_cleanup(curl);
            return 1;
        }
        // printf("%s\n",response);
    
        json = json_tokener_parse(response);
        json_object_object_get_ex(json,"current",&json);
        json_object_object_get_ex(json,"temp_c",&temp_c);
        json_object_object_get_ex(json,"humidity",&humidity);
        json_object_object_get_ex(json,"condition",&description);
        json_object_object_get_ex(description,"text",&description);



        //displaying weather information
        printf("\nWeather information for %s:\n", city);
        printf("Temperature: %s C\n",json_object_get_string(temp_c));
        printf("Humidity: %s %%\n",json_object_get_string(humidity));
        printf("Description: %s\n",json_object_get_string(description));


        curl_easy_cleanup(curl);
    }
    return 0;
}
