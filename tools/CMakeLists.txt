include_directories(${CMAKE_SOURCE_DIR}/include/kernel)
set(CMAKE_C_FLAGS "-std=gnu99")

add_executable(pimg pimg.c)
target_link_libraries(pimg elf)