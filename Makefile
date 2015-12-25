ARCH ?= x86_64
TARGET = $(ARCH)-unknown-linux-gnu

AS = as --64

LD = ld -melf_$(ARCH)
LDFLAGS = -n --gc-sections -T arch/$(ARCH)/link.ld

RUSTCFLAGS = -g -Z no-landing-pads -C no-stack-check -C no-redzone=on
CARGOFLAGS = --target $(TARGET)

ARCHOBJS = arch/$(ARCH)/asm/init.o \
           arch/$(ARCH)/asm/irq.o

all: build/kernel.elf

build/kernel.elf: $(ARCHOBJS) target/$(TARGET)/debug/libkernel.a
	$(LD) $(LDFLAGS) -o $@ $^

target/$(TARGET)/debug/libkernel.a:
	cargo rustc $(CARGOFLAGS) -- $(RUSTCFLAGS)

.PHONY: target/$(TARGET)/debug/libkernel.a

.S.o:
	$(AS) -o $@ $<

.SUFFIXES: .o .S

clean:
	cargo clean
	rm -f $(ARCHOBJS)  build/kernel.elf build/pirix.iso

doc:
	rustdoc -o build/doc kernel/kernel.rs

-include build/kernel.d build/libcore.d

#
# QEMU
#

qemu: iso
	qemu-system-$(ARCH) -monitor stdio -cdrom build/pirix.iso

qemu-serial: iso
	qemu-system-$(ARCH) -serial stdio -cdrom build/pirix.iso

qemu-log: iso
	qemu-system-$(ARCH) -serial stdio -monitor stdio -cdrom build/pirix.iso -no-reboot -d in_asm

debug: iso
	tmux new-session -d -s pirix "qemu-system-$(ARCH) -monitor stdio \
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
  multiboot2 /boot/kernel.elf
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
                                                  multiboot2 normal ls cat help
	cat /usr/lib/grub/i386-pc/cdboot.img boot/tmp_core.img > boot/eltorito.img
	rm -f boot/tmp_core.img

iso: build/kernel.elf boot/eltorito.img boot/grub.cfg
	mkisofs -graft-points -R -b boot/eltorito.img -no-emul-boot \
        -boot-load-size 4 -boot-info-table -o build/pirix.iso \
         boot/=boot/ \
         boot/kernel.elf=build/kernel.elf

.PHONY: boot/grub.cfg doc clean all
