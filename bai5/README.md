# Hệ Điều Hành Nhúng

### Bài 5

##### 1. Biên dịch ứng dụng với thư viện đã có

- Thêm thư viện cJson trong menuconfig
``` bash
make menuconfig
```

``` bash
Target packages
   Libraries
      JSON/XML
         [*] cJSON
```

- Build lại buildroot

``` bash
make -j$(nproc)
```

- Viết 1 chương trình helloJson.c
``` bash
#include <stdio.h>
#include <cjson/cJSON.h>

int main() {

    const char *json_string = "{\"name\":\"Minh\",\"age\":22,\"city\":\"Hanoi\"}";

    cJSON *json = cJSON_Parse(json_string);

    if (json == NULL) {
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
```

- Build bằng toolchain của buildroot
``` bash
buildroot/output/host/bin/arm-buildroot-linux-gnueabihf-gcc helloJson.c \
-Ibuildroot/output/host/arm-buildroot-linux-gnueabihf/sysroot/usr/include \
-Lbuildroot/output/host/arm-buildroot-linux-gnueabihf/sysroot/usr/lib \
-o hellojson -lcjson
```

- Copy file 
``` bash
sudo cp hellojson /media/minh/rootfs/usr/bin
```

##### 2.  Tự tạo thư viện cá nhân

- Tạo folder chứa thư viện 
``` bash
mkdir mylib & cd mylib
```
- Tạo file .c và .h

``` bash
nano mymath.c

#include "mymath.h"

int add(int a, int b){
    return a + b;
}
```

``` bash
nano mymath.h
#ifndef MYMATH_H
#define MYMATH_H

int add(int a, int b);

#endif
```

- Cross compile
``` bash
~/Desktop/buildroot/output/host/bin/arm-buildroot-linux-gnueabihf-gcc -c mymath.c
```

- Tạo static library
``` bash
~/Desktop/buildroot/output/host/bin/arm-buildroot-linux-gnueabihf-ar rcs libmymath.a mymath.o
```

- Tạo dynamic library
``` bash
~/Desktop/buildroot/output/host/bin/arm-buildroot-linux-gnueabihf-gcc -fPIC -c mymath.c

~/Desktop/buildroot/output/host/bin/arm-buildroot-linux-gnueabihf-gcc -shared -o libmymath.so mymath.o
```

- Copy thư viện vào sysroot
``` bash
cp libmymath.a ~/Desktop/buildroot/output/host/arm-buildroot-linux-gnueabihf/sysroot/usr/lib

cp libmymath.so ~/Desktop/buildroot/output/host/arm-buildroot-linux-gnueabihf/sysroot/usr/lib

cp mymath.h ~/Desktop/buildroot/output/host/arm-buildroot-linux-gnueabihf/sysroot/usr/include
```

- Tạo file test
``` bash
nano test_mymath.c

#include <stdio.h>
#include "mymath.h"

int main(){

    int result = add(3,5);

    printf("Result = %d\n", result);

    return 0;
}
```

- Compile file test với static library
``` bash
~/Desktop/buildroot/output/host/bin/arm-buildroot-linux-gnueabihf-gcc \
test_mymath.c \
-Ibuildroot/output/host/arm-buildroot-linux-gnueabihf/sysroot/usr/include \
-Lbuildroot/output/host/arm-buildroot-linux-gnueabihf/sysroot/usr/lib \
-static -o test_static -lmymath
```

- Compile file test với dynamic library
``` bash
~/Desktop/buildroot/output/host/bin/arm-buildroot-linux-gnueabihf-gcc \
test_mymath.c \
-Ibuildroot/output/host/arm-buildroot-linux-gnueabihf/sysroot/usr/include \
-Lbuildroot/output/host/arm-buildroot-linux-gnueabihf/sysroot/usr/lib \
-o test_dynamic -lmymath
```
- Copy file
``` bash
sudo cp test_static /media/minh/rootfs/usr/bin

sudo cp test_dynamic /media/minh/rootfs/usr/bin
```

- Kết quả test trên bbb


##### 3. Tích hợp ứng dụng và thư viện và Buildroot
- Tạo package trong buildroot
``` bash
mkdir ~/Desktop/buildroot/package/helloapp & cd ~/Desktop/buildroot/package/helloapp
```

- Tạo makefile và Config.in
``` bash
nano helloapp.mk

HELLOAPP_VERSION = 1.0
HELLOAPP_SITE = $(TOPDIR)/package/helloapp/src
HELLOAPP_SITE_METHOD = local

HELLOAPP_DEPENDENCIES = cjson

define HELLOAPP_BUILD_CMDS
	$(TARGET_CC) $(@D)/helloapp.c $(@D)/mymath.c \
	-I$(@D) -o $(@D)/helloapp -lcjson
endef

define HELLOAPP_INSTALL_TARGET_CMDS
	$(INSTALL) -D -m 0755 $(@D)/helloapp \
	$(TARGET_DIR)/usr/bin/helloapp
endef

$(eval $(generic-package))
```

``` bash
nano Config.in

config BR2_PACKAGE_HELLOAPP
    bool "helloapp"
    depends on BR2_PACKAGE_CJSON
    help
      Example app using cJSON and mymath library
```

- Tạo file code và thư viện
 + Tạo thư viện:
    ``` bash
    nano mymath.c

    #include "mymath.h"

    int add(int a, int b){
        return a + b;
    }
    ```

    ``` bash
    nano mymath.h
    #ifndef MYMATH_H
    #define MYMATH_H

    int add(int a, int b);

    #endif
    ```
 + Tạo file helloapp.c
    ``` bash
    #include <stdio.h>
    #include <cjson/cJSON.h>
    #include "mymath.h"

    int main() {

        const char *json_string = "{\"a\":5,\"b\":7}";

        cJSON *json = cJSON_Parse(json_string);

        if(json == NULL){
            printf("JSON parse error\n");
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
    ```
- Thêm package vào package/Config.in
``` bash
nano ~Desktop/buildroot/package/Config.in

source "package/helloapp/Config.in"
```

- Chạy menuconfig để thêm package mới 
``` bash
make menuconfig

Target package
    [*] helloapp
```
- Build lại
``` bash
make -j$(nproc)
```


- Đưa vào thẻ nhớ
``` bash
sudo dd if=output/images/sdcard.img of=/dev/sdb bs=4M status=progress
sync
```

- Kết quả
