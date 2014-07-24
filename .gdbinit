set osabi none

define connect
  target remote localhost:1234
end

define kernel
  file build/kernel.elf
end
