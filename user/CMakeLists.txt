set(CMAKE_C_COMPILER arm-elf-pirix-gcc)
set(CMAKE_C_FLAGS "-mtune=arm1176jzf-s -march=armv6  -mfpu=fpa -mfloat-abi=hard -Wall -Wextra -std=gnu99")

set_directory_properties(PROPERTIES INCLUDE_DIRECTORIES "")

add_executable(init
  init.c
)
