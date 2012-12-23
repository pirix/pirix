project(pirix C)
set(VERSION 0.1)
cmake_minimum_required(VERSION 2.8)

###### general

set(ARCH armv6)
set(CPU arm1176jzf-s)

if(NOT PLATFORM)
  set(PLATFORM RBPI)
endif(NOT PLATFORM)

string(TOLOWER ${PLATFORM} PLATFORM_LOWER)

if(NOT CMAKE_BUILD_TYPE)
   set(CMAKE_BUILD_TYPE Debug)
endif(NOT CMAKE_BUILD_TYPE)

configure_file(
  ${CMAKE_SOURCE_DIR}/include/config.h.in
  ${CMAKE_SOURCE_DIR}/include/config.h
)

add_subdirectory(tools)

###### compiler options

set(CMAKE_SYSTEM_NAME Generic)
set(CMAKE_CROSSCOMPILING TRUE)

set(CMAKE_LINKER arm-elf-ld)

set(CMAKE_ASM_COMPILER arm-elf-gcc)
set(CMAKE_ASM_FLAGS "-march=${ARCH} -mfpu=fpa -mcpu=${CPU} -march=armv6")
enable_language(ASM)

set(CMAKE_C_COMPILER arm-elf-gcc)

set(CMAKE_C_FLAGS "-mcpu=${CPU} -mtune=${CPU} -mfpu=fpa -mfloat-abi=softfp -ffreestanding -nostartfiles -nostdinc -nodefaultlibs -Wall -fno-stack-protector -std=gnu99")

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
  target_link_libraries(${name} system)
endmacro()

###### subdirectories

add_subdirectory(kernel)
add_subdirectory(servers)
add_subdirectory(lib)

###### IMAGES

add_custom_target(image
  DEPENDS kernel
  DEPENDS pimg
  COMMAND ./tools/pimg -k kernel/kernel servers/vfs servers/devmgr -o pirix.img
  WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
)

###### QEMU
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

###### DOXYGEN
find_package(Doxygen)
if(DOXYGEN_FOUND)
    configure_file(${CMAKE_SOURCE_DIR}/doc/Doxyfile.in ${CMAKE_BINARY_DIR}/Doxyfile)
    add_custom_target(doxygen ${DOXYGEN_EXECUTABLE} ${CMAKE_BINARY_DIR}/Doxyfile)
endif(DOXYGEN_FOUND)
