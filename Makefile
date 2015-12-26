ARCH ?= x86_64
TARGET = $(ARCH)-unknown-linux-gnu

AS = as --64

LD = ld -melf_$(ARCH)
LDFLAGS = -n --gc-sections -T arch/$(ARCH)/link.ld

RUSTC ?= rustc
RUSTCFLAGS = -L build/ -g --target=arch/$(ARCH)/target.json --out-dir build/
RUSTSRC ?= ./rust/src

ARCHOBJS = arch/$(ARCH)/asm/init.o \
           arch/$(ARCH)/asm/irq.o

KERNELDEPS = build/librlibc.rlib \
			 build/libcore.rlib \
	         build/liballoc.rlib \
			 build/liballoc_system.rlib \
			 build/libcollections.rlib \
			 build/librustc_unicode.rlib \
		     build/libspin.rlib

all: build/kernel.elf

build/kernel.elf: build/kernel.o $(KERNELDEPS) $(ARCHOBJS)
	$(LD) $(LDFLAGS) -o $@ $^

build/libcore.rlib: $(RUSTSRC)/libcore/lib.rs
	mkdir -p build
	$(RUSTC) $(RUSTCFLAGS) --emit=link,dep-info $<

build/librustc_unicode.rlib: $(RUSTSRC)/librustc_unicode/lib.rs build/libcore.rlib
	$(RUSTC) $(RUSTCFLAGS) --emit=link,dep-info $<

build/libcollections.rlib: $(RUSTSRC)/libcollections/lib.rs build/libcore.rlib \
	                         build/liballoc.rlib build/librustc_unicode.rlib
	$(RUSTC) $(RUSTCFLAGS) --emit=link,dep-info $<

build/liballoc.rlib: $(RUSTSRC)/liballoc/lib.rs build/libcore.rlib
	$(RUSTC) $(RUSTCFLAGS) --emit=link,dep-info $<

build/liballoc_system.rlib: lib/alloc_system/lib.rs build/libcore.rlib
	$(RUSTC) $(RUSTCFLAGS) --emit=link,dep-info $<

build/librlibc.rlib: lib/rlibc/src/lib.rs build/libcore.rlib
	$(RUSTC) $(RUSTCFLAGS) --emit=link,dep-info --crate-type=lib --crate-name=rlibc $<

build/libspin.rlib: lib/spin/src/lib.rs build/libcore.rlib
	$(RUSTC) $(RUSTCFLAGS) --emit=link,dep-info --crate-type=lib --crate-name=spin $<

build/kernel.o: kernel/kernel.rs $(KERNELDEPS)
	$(RUSTC) $(RUSTCFLAGS) --emit=obj,dep-info kernel/kernel.rs

.S.o:
	$(AS) -o $@ $<

.SUFFIXES: .o .S

clean:
	cargo clean
	rm -f $(ARCHOBJS) $(KERNELDEPS) build/kernel.elf build/pirix.iso

doc:
	rustdoc -o build/doc kernel/kernel.rs

-include build/kernel.d

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
                                 -s -cdrom build/pirix.iso"
	tmux new-window -t pirix:1 "sleep 5; gdb -ex kernel -ex connect"
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
