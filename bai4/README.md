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
- Sửa file extlinux.conf
``` bash
label beaglebone-buildroot
  kernel /zImage
  fdt /am335x-boneblack.dtb
  append console=ttyS0,115200n8 root=/dev/mmcblk0p2 rw rootfstype=ext4 rootwait
```

