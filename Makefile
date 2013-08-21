define GRUB_CFG
set timeout=0
set default=1
menuentry "pirix" {
  multiboot /boot/kernel
  module /boot/system
  module /boot/login
  boot
}
endef
export GRUB_CFG

all: i386

i386:
	scons ARCH=i386 DEBUG=1 -Q

rbpi:
	scons ARCH=arm PLATFORM=RBPI DEBUG=1 -Q

icp:
	scons ARCH=arm PLATFORM=ICP DEBUG=1 -Q

clean:
	scons -c

qemu: iso
	qemu-system-i386 -cpu core2duo -cdrom pirix.iso

debug: iso
	qemu-system-i386 -S -s -cdrom pirix.iso

boot:
	mkdir -p boot

boot/grub.cfg: boot
	echo "$$GRUB_CFG" > boot/grub.cfg

boot/eltorito.img: boot
	grub-mkimage -p /boot -o boot/tmp_core.img -O i386-pc biosdisk iso9660 multiboot normal ls cat help
	cat /usr/lib/grub/i386-pc/cdboot.img boot/tmp_core.img > boot/eltorito.img
	rm -f boot/tmp_core.img

iso: i386 boot/eltorito.img boot/grub.cfg
	mkisofs -graft-points -R -b boot/eltorito.img -no-emul-boot \
        -boot-load-size 4 -boot-info-table -o pirix.iso \
         boot/=boot/ \
         boot/kernel=build/i386/kernel/kernel \
         boot/system=build/i386/servers/system/system \
         boot/login=build/i386/servers/login/login

.PHONY: boot/grub.cfg
