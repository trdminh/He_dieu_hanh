# Hệ Điều Hành Nhúng

### Bài 2

##### 1. Tải packages

``` bash
sudo apt-get update
sudo apt-get upgrade -y
sudo apt-get install -y bc lz4 vim git make build-essential cpio bison lzop flex libssl-dev wget u-boot-tools libncurses5-dev libncursesw5-dev xz-utils lsb-release man-db gettext pkg-config libmpc-dev zstd libdw-dev 
```

##### 2. Tải ARM Cross Compiler - GCC

- Tải GCC compiler
``` bash
mkdir toolchain
cd toolchain

wget -c https://releases.linaro.org/components/toolchain/binaries/7.5-2019.12/arm-linux-gnueabihf/gcc-linaro-7.5.0-2019.12-x86_64_arm-linux-gnueabihf.tar.xz
tar -xf gcc-linaro-7.5.0-2019.12-x86_64_arm-linux-gnueabihf.tar.xz
export CC=`pwd`/gcc-linaro-7.5.0-2019.12-x86_64_arm-linux-gnueabihf/bin/arm-linux-gnueabihf-
```

- Kiểm tra GCC

``` bash
${CC}gcc --version
```

- Kết quả trả về sau khi kiểm tra 

``` bash
gcc (Ubuntu 9.4.0-1ubuntu1~20.04.2) 9.4.0
Copyright (C) 2019 Free Software Foundation, Inc.
This is free software; see the source for copying conditions.  There is NO
warranty; not even for MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
```

##### 3. Tải Uboot

- Tải uboot về từ git

``` bash
git clone -b v2022.04 https://github.com/u-boot/u-boot --depth=1
cd u-boot
```

- Lựa chọn patch
``` bash
git pull --no-edit https://git.beagleboard.org/beagleboard/u-boot.git v2022.04-bbb.io-am335x-am57xx
```

- Kết quả sau khi chọn patch
``` bash
From https://git.beagleboard.org/beagleboard/u-boot
 * branch              v2022.04-bbb.io-am335x-am57xx -> FETCH_HEAD
Updating e4b6ebd3..c6f4cf7d
Fast-forward
 .gitlab-ci.yml                                     | 418 +-------------------------------------------------------------
 arch/arm/dts/am335x-osd335x-common.dtsi            |   4 +
 arch/arm/include/asm/arch-am33xx/hardware_am33xx.h |   1 +
 arch/arm/include/asm/arch-am33xx/sys_proto.h       |   1 +
 arch/arm/mach-omap2/am33xx/board.c                 |  22 ++--
 arch/arm/mach-omap2/hwinit-common.c                |  10 ++
 board/ti/am335x/board.c                            | 591 +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++--
 board/ti/am335x/board.h                            |  39 +++++-
 board/ti/am335x/hash-string.h                      |  59 +++++++++
 board/ti/am335x/mux.c                              |  13 +-
 board/ti/am57xx/board.c                            | 150 +++++++++++++++++++++-
 board/ti/common/board_detect.c                     |  12 ++
 build-am335x.sh                                    |  39 ++++++
 build-am57xx.sh                                    |  39 ++++++
 configs/am335x_evm_defconfig                       |  46 +++----
 configs/am57xx_evm_defconfig                       |  23 ++--
 include/configs/am335x_evm.h                       | 100 +++++++++++++--
 include/configs/ti_armv7_common.h                  | 696 +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
 include/configs/ti_omap5_common.h                  |  19 +--
 include/environment/ti/mmc.h                       |  80 ++++++++++--
 install-am335x-am57xx.sh                           |  43 +++++++
 21 files changed, 1909 insertions(+), 496 deletions(-)
 create mode 100644 board/ti/am335x/hash-string.h
 create mode 100755 build-am335x.sh
 create mode 100755 build-am57xx.sh
 create mode 100755 install-am335x-am57xx.sh
```

- Config và build cho uboot

``` bash
make ARCH=arm CROSS_COMPILE=${CC} distclean
make ARCH=arm CROSS_COMPILE=${CC} am335x_evm_defconfig
make ARCH=arm CROSS_COMPILE=${CC}
```
##### 4. Build linux kernel

- Tải linux kernel
``` bash
git clone https://github.com/RobertCNelson/bb-kernel ./kernelbuildscripts
cd kernelbuildscripts/
git checkout origin/am33x-v5.4 -b tmp
```

- Build 
``` bash
./build_kernel.sh
```
- Khi build hoàn tất sẽ trả lại kết quả
``` bash
-----------------------------
'arch/arm/boot/zImage' -> '/home/backup/kernelbuildscripts/deploy/5.4.288-bone69.zImage'
'.config' -> '/home/backup/kernelbuildscripts/deploy/config-5.4.288-bone69'
-rwxr-xr-x 1 root root 5.6M Jan 14 14:38 /home/backup/kernelbuildscripts/deploy/5.4.288-bone69.zImage
-----------------------------
Building modules archive...
Warning: 'make modules_install' requires depmod. Please install it.
This is probably in the kmod package.
Compressing 5.4.288-bone69-modules.tar.gz...
-rw-r--r-- 1 root root 21M Jan 14 14:38 /home/backup/kernelbuildscripts/deploy/5.4.288-bone69-modules.tar.gz
-----------------------------
Building dtbs archive...
Compressing 5.4.288-bone69-dtbs.tar.gz...
-rw-r--r-- 1 root root 669K Jan 14 14:38 /home/backup/kernelbuildscripts/deploy/5.4.288-bone69-dtbs.tar.gz
-----------------------------
Script Complete
eewiki.net: [user@localhost:~$ export kernel_version=5.4.288-bone69]
-----------------------------
```
- Tải Root File System 

``` bash
wget -c https://rcn-ee.com/rootfs/eewiki/minfs/debian-11.5-minimal-armhf-2022-10-06.tar.xz

sha256sum debian-11.5-minimal-armhf-2022-10-06.tar.xz

tar xf debian-11.5-minimal-armhf-2022-10-06.tar.xz
```

##### 5. Cài đặt cho thẻ microSD 
- Format thẻ nhớ
``` bash
sudo mkfs.vfat -I /dev/sda
export DISK=/dev/sda
sudo dd if=/dev/zero of=${DISK} bs=1M count=10
```

- Tải bootloader
``` bash
sudo dd if=./u-boot/MLO of=${DISK} count=2 seek=1 bs=128k
sudo dd if=./u-boot/u-boot-dtb.img of=${DISK} count=4 seek=1 bs=384k
```

- Tạo partition layout
``` bash

sudo sfdisk ${DISK} <<-__EOF__
4M,,L,*
__EOF__
 
sudo mkfs.ext4 -L rootfs -O ^metadata_csum,^64bit ${DISK}1
```

- Mount Partition
``` bash
sudo mkdir -p /media/rootfs/
sudo mount ${DISK}1 /media/rootfs/
```

- Backup Bootloader
``` bash
sudo mkdir -p /media/rootfs/opt/backup/uboot/
sudo cp -v ./u-boot/MLO /media/rootfs/opt/backup/uboot/
sudo cp -v ./u-boot/u-boot-dtb.img /media/rootfs/opt/backup/uboot/
```

##### 6. Tải kernel và Root File System
``` bash
export kernel_version=5.4.288-bone69
```

- Copy Root File System
``` bash
sudo tar xfvp ./debian-*-*-armhf-*/armhf-rootfs-*.tar -C /media/rootfs/
```

- Set uname_r cho /boot/uEnv.txt
``` bash
sudo sh -c "echo 'uname_r=${kernel_version}' >> /media/rootfs/boot/uEnv.txt"
```

- Copy Kernel Image
``` bash

sudo cp -v ./kernelbuildscripts/deploy/${kernel_version}.zImage /media/rootfs/boot/vmlinuz-${kernel_version}
```
- Copy Kernel Device Tree Binaries
``` bash
sudo mkdir -p /media/rootfs/boot/dtbs/${kernel_version}/
sudo tar xfv ./kernelbuildscripts/deploy/${kernel_version}-dtbs.tar.gz -C /media/rootfs/boot/dtbs/${kernel_version}/
```

- Copy Kernel Modules
``` bash
sudo tar xfv ./kernelbuildscripts/deploy/${kernel_version}-modules.tar.gz -C /media/rootfs/
```

- File Systems Table (/etc/fstab)
``` bash
sudo sh -c "echo '/dev/mmcblk0p1  /  auto  errors=remount-ro  0  1' >> /media/rootfs/etc/fstab"
```

- Remove microSD card
``` bash
sync
sudo umount /media/rootfs
```

##### 7. Boot với SDcard

Nhấn và giữ nút BOOT S2 để disconnect và cắm thẻ nhớ và cấp lại nguồn sau đó thả nút BOOT để boot từ SD card và bbb
