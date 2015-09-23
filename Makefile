ARCH ?= i386

AS = as --32 #$(ARCH)-elf-pirix-as

LD = ld -melf_i386 #$(ARCH)-elf-pirix-ld
LDFLAGS = -z max-page-size=0x1000 --gc-sections -T arch/$(ARCH)/link.ld

RUSTC = rustc
RUSTFLAGS = -g -C opt-level=0 --target=arch/$(ARCH)/target.json

ARCHOBJS = arch/i386/asm/init.o \
           arch/i386/asm/gdt.o \
           arch/i386/asm/irq.o

all: build/kernel.elf

build/kernel.elf: build/kernel.o $(ARCHOBJS) build/libcore.rlib
	$(LD) $(LDFLAGS) -o $@ $^

build/libcore.rlib: libcore/lib.rs
	mkdir -p build
	$(RUSTC) $(RUSTFLAGS) --out-dir build/ --crate-type=lib --emit=link,dep-info $<

build/kernel.o: kernel/kernel.rs build/libcore.rlib
	mkdir -p build
	$(RUSTC) $(RUSTFLAGS) --out-dir build/ --emit=obj,dep-info kernel/kernel.rs \
	                      --extern core=build/libcore.rlib

.S.o:
	$(AS) -o $@ $<

.SUFFIXES: .o .S

clean:
	rm -f $(ARCHOBJS) build/kernel.elf build/kernel.o build/pirix.iso

doc:
	rustdoc -o build/doc kernel/kernel.rs

-include build/kernel.d build/libcore.d

#
# QEMU
#

qemu: iso
	qemu-system-i386 -monitor stdio -cdrom build/pirix.iso

qemu-serial: iso
	qemu-system-i386 -serial stdio -cdrom build/pirix.iso

qemu-log: iso
	qemu-system-i386 -serial stdio -monitor stdio -cdrom build/pirix.iso -no-reboot -d in_asm

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

.PHONY: boot/grub.cfg doc clean all
