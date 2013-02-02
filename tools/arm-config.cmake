if(NOT PLATFORM)
  set(PLATFORM RBPI)
endif(NOT PLATFORM)

set(CPU arm1176jzf-s)

set(CMAKE_LINKER arm-elf-pirix-ld)
set(CMAKE_ASM_COMPILER arm-elf-pirix-gcc)
set(CMAKE_C_COMPILER arm-elf-pirix-gcc)
set(CMAKE_OBJCOPY arm-elf-pirix-objcopy)

set(CMAKE_ASM_FLAGS "-march=armv6 -mfpu=fpa -mcpu=${CPU}")
set(CMAKE_C_FLAGS "-mcpu=${CPU} -mtune=${CPU} -mfpu=fpa -mfloat-abi=softfp -ffreestanding -nostartfiles -nodefaultlibs -Wall -fno-stack-protector -std=gnu99")

add_custom_target(debug
  DEPENDS image
  COMMAND qemu-system-arm -cpu arm1176 -kernel pirix.img -nographic -S -s
  WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
)

add_custom_target(qemu
  DEPENDS image
  COMMAND qemu-system-arm -cpu arm1176 -kernel pirix.img -nographic
  WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
)