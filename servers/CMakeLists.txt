set(CMAKE_LINKER arm-elf-pirix-ld)
set(CMAKE_C_COMPILER arm-elf-pirix-gcc)
set(CMAKE_C_FLAGS "-mcpu=${CPU} -mtune=${CPU} -mfpu=fpa -mfloat-abi=softfp -Wall -Wextra -std=gnu99 -iquote ${CMAKE_SOURCE_DIR}/include")

set_directory_properties(PROPERTIES INCLUDE_DIRECTORIES "")

user_executable(vfs
  vfs/vfs.c
  vfs/node.c
)

user_executable(devmgr
  devmgr/devmgr.c
)