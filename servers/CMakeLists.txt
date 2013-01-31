set(CMAKE_C_FLAGS "-mcpu=${CPU} -mtune=${CPU} -mfpu=fpa -mfloat-abi=softfp -Wall -std=gnu99")

link_directories(${CMAKE_BINARY_DIR}/lib)

user_executable(vfs
  vfs/vfs.c
  vfs/vnode.c
)

user_executable(devmgr
  devmgr/devmgr.c
  devmgr/devfs.c
)

install(TARGETS vfs devmgr RUNTIME DESTINATION bin)