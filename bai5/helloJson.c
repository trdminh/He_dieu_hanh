#include <stdio.h>
#include <cjson/cJSON.h>

int main() {

    const char *json_string = "{\"name\":\"Minh\",\"age\":22,\"city\":\"Hanoi\"}";

    cJSON *json = cJSON_Parse(json_string);

    if (json == NULL) {
        printf("Error parsing JSON\n");
        return -1;
    }

    cJSON *name = cJSON_GetObjectItem(json, "name");
    cJSON *age = cJSON_GetObjectItem(json, "age");
    cJSON *city = cJSON_GetObjectItem(json, "city");

    printf("Name: %s\n", name->valuestring);
    printf("Age: %d\n", age->valueint);
    printf("City: %s\n", city->valuestring);

    cJSON_Delete(json);

    return 0;
}