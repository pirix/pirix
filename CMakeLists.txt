project(pirix C)
cmake_minimum_required(VERSION 2.8)

set(CMAKE_LINKER arm-elf-pirix-ld)

set(CMAKE_ASM_COMPILER arm-elf-pirix-as)
set(CMAKE_ASM_FLAGS "-march=armv6 -mfpu=fpa -mfloat-abi=hard")
enable_language(ASM)

set(CMAKE_C_COMPILER arm-elf-pirix-gcc)
set(CMAKE_C_FLAGS "-mtune=arm1176jzf-s -mfpu=fpa -mfloat-abi=hard -march=armv6 -ffreestanding -nostartfiles -nostdinc -nodefaultlibs -Wall -Wextra -fno-stack-protector -std=gnu99")

set(CMAKE_C_SKIP_RPATH ON)
set(CMAKE_SHARED_LIBRARY_LINK_C_FLAGS)

include_directories(${CMAKE_SOURCE_DIR}/include)

add_subdirectory(kernel)
add_subdirectory(drivers)
add_subdirectory(lib)
add_subdirectory(user)

set(INITRD_FILES
  user/init
)

add_custom_command(
  OUTPUT initrd
  COMMAND tar
  ARGS cf initrd ${INITRD_FILES}
  DEPENDS init
  COMMENT "Generating initrd"
  WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
)

add_custom_command(
  OUTPUT ${CMAKE_BINARY_DIR}/initrd.o
  COMMAND arm-elf-objcopy
  ARGS --binary-architecture arm -I binary -O elf32-littlearm
       --rename-section .data=.rodata,alloc,load,readonly,data,contents
       initrd initrd.o
  DEPENDS initrd
  COMMENT "Generating initrd.o"
  WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
)

add_executable(pirix
  $<TARGET_OBJECTS:kernel>
  $<TARGET_OBJECTS:lib>
  $<TARGET_OBJECTS:drivers>
  initrd.o
)

set_target_properties(pirix
  PROPERTIES
  LINK_FLAGS "-T ${CMAKE_SOURCE_DIR}/kernel/link.ld"
  LINKER_LANGUAGE C
)

add_custom_target(debug)
add_custom_command(
  TARGET debug
  DEPENDS pirix
  COMMAND qemu-system-arm -cpu arm1176 -kernel ${CMAKE_BINARY_DIR}/pirix -nographic -S -s
)

add_custom_target(qemu)
add_custom_command(
  TARGET qemu
  DEPENDS pirix
  COMMAND qemu-system-arm -cpu arm1176 -kernel ${CMAKE_BINARY_DIR}/pirix -nographic
)