-include build/kernel.d

ARCH ?= i386
TARGET = i686-unknown-linux-gnu

AS = $(ARCH)-elf-pirix-as
LD = $(ARCH)-elf-pirix-ld
CLANG = clang
CLANGFLAGS = -ffreestanding -target $(TARGET)
RUSTC = rustc
RUSTFLAGS = -Z no-landing-pads -Z lto --target=$(TARGET) --out-dir=build/

ARCHOBJS = arch/i386/asm/init.o \
           arch/i386/asm/gdt.o \
           arch/i386/asm/irq.o

.SUFFIXES: .o .S

all: build/kernel.elf

build/kernel.elf: build/kernel.o $(ARCHOBJS)
	$(LD) -T arch/$(ARCH)/link.ld -o $@ $^

build/kernel.o: build/kernel.bc
	$(CLANG) $(CLANGFLAGS) -c -o $@ $<

build/kernel.bc: kernel/kernel.rs build
	$(RUSTC) $(RUSTFLAGS) --dep-info --emit bc $< -o $@

build:
	mkdir build

.S.o:
	$(AS) -o $@ $<

clean:
	rm -f $(ARCHOBJS) build/kernel.bc build/kernel.o build/pirix.iso

#
# QEMU
#

qemu: iso
	qemu-system-i386 -cdrom build/pirix.iso

debug: iso
	qemu-system-i386 -monitor stdio -S -s -cdrom build/pirix.iso

#
# GRUB
#

define GRUB_CFG
set timeout=5
set default=1
menuentry "pirix" {
  multiboot /boot/kernel
  boot
}
endef
export GRUB_CFG

boot:
	mkdir -p boot

boot/grub.cfg: boot
	echo "$$GRUB_CFG" > boot/grub.cfg

boot/eltorito.img: boot
	grub-mkimage -p /boot -o boot/tmp_core.img -O i386-pc biosdisk iso9660 multiboot normal ls cat help
	cat /usr/lib/grub/i386-pc/cdboot.img boot/tmp_core.img > boot/eltorito.img
	rm -f boot/tmp_core.img

iso: build/kernel.elf boot/eltorito.img boot/grub.cfg
	mkisofs -graft-points -R -b boot/eltorito.img -no-emul-boot \
        -boot-load-size 4 -boot-info-table -o build/pirix.iso \
         boot/=boot/ \
         boot/kernel=build/kernel.elf

.PHONY: boot/grub.cfg
