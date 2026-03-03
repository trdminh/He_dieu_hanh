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

- Copy dữ liệu
``` bash
sudo mount /dev/mmcblk0p2 /mnt

sudo cp -a _install/* /mnt/
sync

sudo umount /mnt
```

##### 3. Boot với SDcard

Nhấn và giữ nút BOOT S2 để disconnect và cắm thẻ nhớ và cấp lại nguồn sau đó thả nút BOOT để boot từ SD card và bbb





