#!/bin/bash
echo "make bzImage"
make -j4 bzImage

echo "make modules"
make -j4 modules

echo "make modules_install"
make -j4 modules_install

echo "make install"
make -j4 install

echo "install image & reboot"
cd ../../../../../boot/
mkinitramfs -o initrd.img-3.13.11-ckt27 3.13.11-ckt27-cs470p2
../../sbin/reboot
