project(pirix C)
set(VERSION 0.1)
cmake_minimum_required(VERSION 2.8)

add_subdirectory(tools)

###### general

if(NOT ARCH)
  set(ARCH arm)
endif(NOT ARCH)

include(tools/${ARCH}-config.cmake)

if(NOT PLATFORM)
  set(PLATFORM ${ARCH})
endif(NOT PLATFORM)

string(TOLOWER ${PLATFORM} PLATFORM_LOWER)

if(NOT CMAKE_BUILD_TYPE)
   set(CMAKE_BUILD_TYPE Debug)
endif(NOT CMAKE_BUILD_TYPE)

configure_file(
  ${CMAKE_SOURCE_DIR}/include/config.h.in
  ${CMAKE_BINARY_DIR}/include/config.h
)

configure_file(
  ${CMAKE_SOURCE_DIR}/include/arch.h.in
  ${CMAKE_BINARY_DIR}/include/arch.h
)

include_directories(${CMAKE_BINARY_DIR}/include/)

###### compiler options

set(CMAKE_SYSTEM_NAME Generic)
set(CMAKE_CROSSCOMPILING TRUE)

enable_language(ASM)

set(CMAKE_C_SKIP_RPATH ON)
set(CMAKE_SHARED_LIBRARY_LINK_C_FLAGS)

include_directories(
  ${CMAKE_SOURCE_DIR}/include
  ${CMAKE_SOURCE_DIR}/servers
)

###### user executable

macro(USER_EXECUTABLE name)
  add_executable(${name} ${ARGN})
  set_target_properties(${name} PROPERTIES
    LINK_FLAGS "-T ${CMAKE_SOURCE_DIR}/tools/user.ld"
    DEPENDS pirix
    DEPENDS crt0
  )
  target_link_libraries(${name} pirix)
endmacro()

###### subdirectories

add_subdirectory(kernel)
add_subdirectory(lib)
add_subdirectory(servers)

###### IMAGES

add_custom_target(image ALL
  DEPENDS kernel
  DEPENDS pimg vfs devmgr
  COMMAND ./tools/pimg -k kernel/kernel servers/vfs servers/devmgr -o pirix.img
  WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
)
install(FILES ${CMAKE_BINARY_DIR}/pirix.img DESTINATION boot)

###### DOXYGEN
find_package(Doxygen)
if(DOXYGEN_FOUND)
    configure_file(${CMAKE_SOURCE_DIR}/doc/Doxyfile.in ${CMAKE_BINARY_DIR}/Doxyfile)
    add_custom_target(doxygen ${DOXYGEN_EXECUTABLE} ${CMAKE_BINARY_DIR}/Doxyfile)
endif(DOXYGEN_FOUND)
