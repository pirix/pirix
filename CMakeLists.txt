project(pirix C)
set(VERSION 0.1)
cmake_minimum_required(VERSION 2.8)

###### general

add_subdirectory(tools)

###### compiler options

set(CMAKE_SYSTEM_NAME Generic)
set(CMAKE_CROSSCOMPILING TRUE)
set(ARCH armv6)

set(CMAKE_LINKER arm-elf-ld)

set(CMAKE_ASM_COMPILER arm-elf-as)
set(CMAKE_ASM_FLAGS "-march=${ARCH} -mfpu=vfp -mfloat-abi=softfp")
enable_language(ASM)

set(CMAKE_C_COMPILER arm-elf-gcc)
set(CMAKE_C_FLAGS "-mfpu=vfp -mfloat-abi=softfp -march=${ARCH} -mtune=arm1176jzf-s -ffreestanding -nostartfiles -nostdinc -nodefaultlibs -Wall -Wextra -fno-stack-protector -std=gnu99")

set(CMAKE_OBJCOPY arm-elf-objcopy)

set(CMAKE_C_SKIP_RPATH ON)
set(CMAKE_SHARED_LIBRARY_LINK_C_FLAGS)

include_directories(${CMAKE_SOURCE_DIR}/include)

###### user executable

macro(USER_EXECUTABLE name)
  add_executable(${name} ${ARGN})
  set_target_properties(${name} PROPERTIES
    LINK_FLAGS "-T ${CMAKE_SOURCE_DIR}/tools/user.ld"
  )
endmacro()

###### subdirectories

add_subdirectory(kernel)
add_subdirectory(drivers)
add_subdirectory(servers)
add_subdirectory(lib)

###### IMAGES

add_custom_target(image)
add_custom_command(
  TARGET image
  DEPENDS kernel
  DEPENDS boot
  DEPENDS pimg
  COMMAND ./tools/pimg -k kernel/kernel -o pirix.img
  WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
)

###### QEMU
add_custom_target(debug)
add_custom_command(
  TARGET debug
  DEPENDS image
  COMMAND qemu-system-arm -cpu arm1176 -kernel pirix.img -nographic -S -s
  WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
)

add_custom_target(qemu)
add_custom_command(
  TARGET qemu
  DEPENDS image
  COMMAND qemu-system-arm -cpu arm1176 -kernel pirix.img -nographic
  WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
)

###### DOXYGEN
find_package(Doxygen)
if(DOXYGEN_FOUND)
    configure_file(${CMAKE_SOURCE_DIR}/doc/Doxyfile.in ${CMAKE_BINARY_DIR}/Doxyfile)
    add_custom_target(doxygen ${DOXYGEN_EXECUTABLE} ${CMAKE_BINARY_DIR}/Doxyfile)
endif(DOXYGEN_FOUND)
