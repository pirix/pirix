ARCH ?= i386
TARGET = i686-unknown-linux-gnu

AS = $(ARCH)-elf-pirix-as
LD = $(ARCH)-elf-pirix-ld

CLANG = clang
CLANGFLAGS = -g -ffreestanding -target $(TARGET)

RUSTC = rustc
RUSTFLAGS = -g -Z no-landing-pads -Z lto --target=$(TARGET)

ARCHOBJS = arch/i386/asm/init.o \
           arch/i386/asm/gdt.o \
           arch/i386/asm/irq.o

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

.SUFFIXES: .o .S

clean:
	rm -f $(ARCHOBJS) build/kernel.elf build/kernel.bc build/kernel.o build/pirix.iso

-include build/kernel.d

#
# QEMU
#

qemu: iso
	qemu-system-i386 -monitor stdio -cdrom build/pirix.iso

qemu-log: iso
	qemu-system-i386 -monitor stdio -cdrom build/pirix.iso -no-reboot -d in_asm

debug: iso
	tmux new-session -d -s pirix "qemu-system-i386 -monitor stdio \
                                 -S -s -cdrom build/pirix.iso"
	tmux new-window -t pirix:1 "gdb -ex kernel -ex connect"
	tmux a -t pirix
	tmux kill-session -t pirix

#
# GRUB
#

define GRUB_CFG
set timeout=0
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
	grub-mkimage -p /boot -o boot/tmp_core.img -O i386-pc biosdisk iso9660 \
                                                  multiboot normal ls cat help
	cat /usr/lib/grub/i386-pc/cdboot.img boot/tmp_core.img > boot/eltorito.img
	rm -f boot/tmp_core.img

iso: build/kernel.elf boot/eltorito.img boot/grub.cfg
	mkisofs -graft-points -R -b boot/eltorito.img -no-emul-boot \
        -boot-load-size 4 -boot-info-table -o build/pirix.iso \
         boot/=boot/ \
         boot/kernel=build/kernel.elf

.PHONY: boot/grub.cfg
