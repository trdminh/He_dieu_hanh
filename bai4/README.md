# Hệ Điều Hành Nhúng

### Bài 4

##### 1. Build với buildroot

- Cài buildroot
``` bash
git clone https://github.com/buildroot/buildroot.git && cd buildroot
```

- Cấu hình cho buildroot
``` bash
make beaglebone_defconfig
make menuconfig
```

- Chạy lệnh build
``` bash
make -j$(nproc)
```

- Đưa vào thẻ nhớ
``` bash
sudo dd if=output/images/sdcard.img of=/dev/sdX bs=4M status=progress
sync
```
- Sửa file extlinux.conf trong phân vùng boot
``` bash
label beaglebone-buildroot
  kernel /zImage
  fdt /am335x-boneblack.dtb
  append console=ttyS0,115200n8 root=/dev/mmcblk0p2 rw rootfstype=ext4 rootwait
```

##### 2. Sử dụng Toolchain từ Buildroot

- Tạo file code 
``` bash
nano ~/Desktop/bai2/main.c
```

- Tạo đoạn code 
``` bash
#include "stdio.h"

int main()
{
    printf("Hello world!");
    return 0;
}
```

- Build với toolchain của buildroot
``` bash
~/Desktop/buildroot/output/host/bin/arm-buildroot-uclibcgnueabihf-gcc -static main.c -o hello
```

- Copy file 
``` bash
sudo cp hello /media/minh/rootfs/usr/bin
```

##### 3. Thêm Package vào Buildroot
- Tạo folder cho package mới
``` bash
mkdir -p ~/Desktop/buildroot/package/hello/src

cd ~/Desktop/buildroot/package/hello
```

- Tạo các file hello.c
``` bash
nano src/hello.c
```

``` bash
#include "stdio.h"

int main()
{
    printf("Hello world!");
    return 0;
}
```

- Tạo Makefile
``` bash
nano hello.mk
```

``` bash
HELLO_VERSION = 1.0
HELLO_SITE = $(TOPDIR)/package/hello/src
HELLO_SITE_METHOD = local

define HELLO_BUILD_CMDS
  $(TARGET_CC) -static $(@D)/hello.c -o $(@D)hello
endef

define HELLO_INSTALL_TARGET_CMDS
  $(INSTALL) -D -m 0755 $(@D)/hello $(TARGET_DIR)/usr/bin/hello
endef

$(eval $(generic-package))
```
- Tạo file Config.in
``` bash
nano Config.in
```

``` bash
config BR2_PACKAGE_HELLO
  bool "hello"
  help
    Simple Hello World program
```
- Thêm config của package 
``` bash
nano ~/Desktop/buildroot/package/Config.in

source "package/hello/Config.in"
```

- Build lại 
``` bash
make -j$(nproc)
```