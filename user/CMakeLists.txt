set(CMAKE_ASM_COMPILER arm-elf-ix-as)
set(CMAKE_ASM_FLAGS "-march=armv6 -mfpu=fpa -mfloat-abi=hard")

set(CMAKE_C_COMPILER arm-elf-ix-gcc)
set(CMAKE_C_FLAGS "-mtune=arm1176jzf-s -mfpu=fpa -mfloat-abi=hard -march=armv6 -ffreestanding -nostartfiles -nostdinc -nodefaultlibs -Wall -Wextra -fno-stack-protector -std=gnu99")

add_executable(init
  init.c
)