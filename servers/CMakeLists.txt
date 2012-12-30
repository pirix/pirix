set(CMAKE_C_FLAGS "-mcpu=${CPU} -mtune=${CPU} -mfpu=fpa -mfloat-abi=softfp -Wall -std=gnu99 -iquote ${CMAKE_SOURCE_DIR}/include")

link_directories(${CMAKE_BINARY_DIR}/lib)
set_directory_properties(PROPERTIES INCLUDE_DIRECTORIES "")
include_directories(${CMAKE_CURRENT_SOURCE_DIR})

user_executable(vfs
  vfs/vfs.c
  vfs/vnode.c
)

user_executable(devmgr
  devmgr/devmgr.c
  devmgr/devfs.c
)