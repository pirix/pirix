set(CMAKE_LINKER i386-elf-pirix-ld)
set(CMAKE_ASM_COMPILER i386-elf-pirix-gcc)
set(CMAKE_C_COMPILER i386-elf-pirix-gcc)
set(CMAKE_OBJCOPY i386-elf-pirix-objcopy)

set(CMAKE_C_FLAGS "-ffreestanding -nostartfiles -nodefaultlibs -Wall -fno-stack-protector -std=gnu99")

add_custom_target(debug
  DEPENDS image
  COMMAND qemu-system-i386 -kernel pirix.img -nographic -S -s
  WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
)

add_custom_target(qemu
  DEPENDS image
  COMMAND qemu-system-i386 -kernel pirix.img -nographic
  WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
)