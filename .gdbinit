set osabi none
set arch i386:x86-64

define connect
  target remote localhost:1234
end

define kernel
  file build/kernel.elf
end
