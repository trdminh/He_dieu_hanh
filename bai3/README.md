# Hệ Điều Hành Nhúng

### Bài 3

##### 1. Cài đặt cho thẻ microSD 
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

##### 2. Tải kernel và Root File System
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

##### 3. Boot với SDcard

Nhấn và giữ nút BOOT S2 để disconnect và cắm thẻ nhớ và cấp lại nguồn sau đó thả nút BOOT để boot từ SD card và bbb

##### 4. Connect tới BBB
- Kết nối đến bbb qua screen và uart
``` bash
sudo screen /dev/ttyUSB0 115200
```

- Đăng nhập vào bbb
``` bash
arm login: debian
Password: 

The programs included with the Debian GNU/Linux system are free software;
the exact distribution terms for each program are described in the
individual files in /usr/share/doc/*/copyright.

Debian GNU/Linux comes with ABSOLUTELY NO WARRANTY, to the extent
permitted by applicable law.
Last login: Thu Oct  6 20:19:40 UTC 2022 on ttyS0
[  151.622109] systemd-journald[142]: File /var/log/journal/5d7acfce99624d61b2b259cb6c0cf8a2/user-1000.journal corrupted or uncleanly shut down, renaming and replacing.
```

- Chạy các lệnh trong bbb
``` bash
debian@arm:~$ ls
debian@arm:~$ echo

debian@arm:~$ echo "minh"
minh
debian@arm:~$ 
```

