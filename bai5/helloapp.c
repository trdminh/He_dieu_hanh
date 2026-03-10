#include <stdio.h>
#include <cjson/cJSON.h>
#include "mymath.h"

int main() {

    const char *json_string = "{\"a\":5,\"b\":7}";

    cJSON *json = cJSON_Parse(json_string);

    if(json == NULL){
        return -1;
    }

    cJSON *a = cJSON_GetObjectItem(json, "a");
    cJSON *b = cJSON_GetObjectItem(json, "b");

    int result = add(a->valueint, b->valueint);

    printf("a = %d\n", a->valueint);
    printf("b = %d\n", b->valueint);
    printf("a + b = %d\n", result);

    cJSON_Delete(json);

    return 0;
}