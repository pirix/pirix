ARCH ?= x86_64

AS = as --64 #$(ARCH)-elf-pirix-as

LD = ld -melf_x86_64 #$(ARCH)-elf-pirix-ld
LDFLAGS = -z max-page-size=0x1000 --gc-sections -T arch/$(ARCH)/link.ld

RUSTC = rustc
RUSTFLAGS = -g -L build -C opt-level=0 --target=arch/$(ARCH)/target.json --out-dir build/

RUSTSRC ?= ./rust/src

ARCHOBJS = arch/$(ARCH)/asm/init.o \
           arch/$(ARCH)/asm/irq.o

all: build/kernel.elf

build/kernel.elf: build/kernel.o $(ARCHOBJS) build/libcore.rlib \
	                build/liballoc.rlib build/liballoc_system.rlib build/libcollections.rlib
	$(LD) $(LDFLAGS) -o $@ $^

build/libcore.rlib: $(RUSTSRC)/libcore/lib.rs
	mkdir -p build
	$(RUSTC) $(RUSTFLAGS) --emit=link,dep-info $<

build/librustc_unicode.rlib: $(RUSTSRC)/librustc_unicode/lib.rs build/libcore.rlib
	$(RUSTC) $(RUSTFLAGS) --emit=link,dep-info $<

build/libcollections.rlib: $(RUSTSRC)/libcollections/lib.rs build/libcore.rlib \
	                         build/liballoc.rlib build/librustc_unicode.rlib
	$(RUSTC) $(RUSTFLAGS) --emit=link,dep-info $<

build/liballoc.rlib: $(RUSTSRC)/liballoc/lib.rs build/libcore.rlib
	$(RUSTC) $(RUSTFLAGS) --emit=link,dep-info $<

build/liballoc_system.rlib: lib/alloc_system/lib.rs build/libcore.rlib
	$(RUSTC) $(RUSTFLAGS) --emit=link,dep-info $<

build/kernel.o: kernel/kernel.rs build/liballoc_system.rlib build/libcore.rlib \
	              build/liballoc.rlib build/libcollections.rlib
	$(RUSTC) $(RUSTFLAGS) --emit=obj,dep-info kernel/kernel.rs

.S.o:
	$(AS) -o $@ $<

.SUFFIXES: .o .S

clean:
	rm -f $(ARCHOBJS)  build/kernel.elf build/kernel.o build/pirix.iso

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
  multiboot2 /boot/kernel
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
         boot/kernel=build/kernel.elf

.PHONY: boot/grub.cfg doc clean all
