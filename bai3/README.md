# Hệ Điều Hành Nhúng

### Bài 3

##### 1. Setup busy box
- Tải busy box
``` bash
git clone https://git.busybox.net/busybox && cd busybox
```
- Set up cho busy box
``` bash
export PATH=$HOME/Desktop/backup/toolchain/gcc-linaro-7.5.0-2019.12-x86_64_arm-linux-gnueabihf/bin:$PATH

make ARCH=arm CROSS_COMPILE=arm-linux-gnueabihf- menuconfig

```

- Build cài đặt cho busybox
``` bash
make -j$(nproc)
make install
```

- Tạo rootfs
``` bash
cd _install

mkdir -p \
  etc proc sys dev lib tmp var root \
  etc/init.d

chmod 755 .
chmod 1777 tmp
sudo mknod -m 600 dev/console c 5 1
sudo mknod -m 666 dev/null c 1 3
sudo mknod -m 666 dev/tty c 5 0
```

- Cấu hình cho rootfs
``` bash
cat > etc/inittab << 'EOF'
::sysinit:/etc/init.d/rcS
ttyS0::askfirst:/bin/sh
::ctrlaltdel:/sbin/reboot
::shutdown:/bin/umount -a -r
EOF

cat > etc/init.d/rcS << 'EOF'
#!/bin/sh

mount -t proc none /proc
mount -t sysfs none /sys
mount -t devtmpfs none /dev

echo "System booted successfully"
EOF

chmod +x etc/init.d/rcS

echo "root:x:0:0:root:/root:/bin/sh" > etc/passwd
echo "root:x:0:" > etc/group

cat > etc/fstab << 'EOF'
proc  /proc proc  defaults 0 0
sysfs /sys  sysfs defaults 0 0
EOF
```



##### 2. Cài đặt cho thẻ microSD 
- Format thẻ nhớ
``` bash
sudo mkfs.vfat -I /dev/sda
export DISK=/dev/sda
sudo dd if=/dev/zero of=${DISK} bs=1M count=10
```
- Tải uboot
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

sudo mkdir -p /media/rootfs/
sudo mount ${DISK}1 /media/rootfs/
```
- Backup bootloader
``` bash
sudo mkdir -p /media/rootfs/opt/backup/uboot/
sudo cp -v ./u-boot/MLO /media/rootfs/opt/backup/uboot/
sudo cp -v ./u-boot/u-boot-dtb.img /media/rootfs/opt/backup/uboot/
```
- Setup kernel
``` bash
export kernel_version=5.4.288-bone69
mkdir /media/rootfs/boot/uEnv.txt
sudo sh -c "echo 'uname_r=${kernel_version}' >> /media/rootfs/boot/uEnv.txt"

setenv bootargs console=ttyS0,115200n8 root=/dev/mmcblk0p1 rw rootfstype=ext4
```
- Copy dữ liệu
``` bash

sudo cp -a _install/* /media/rootfs
sync

sudo umount /media/rootfs
```

##### 3. Boot với SDcard

Nhấn và giữ nút BOOT S2 để disconnect và cắm thẻ nhớ và cấp lại nguồn sau đó thả nút BOOT để boot từ SD card và bbb





