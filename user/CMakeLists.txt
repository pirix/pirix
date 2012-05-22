add_executable(init
  init.c
)

set_target_properties(init
  PROPERTIES
  LINK_FLAGS "-T ${CMAKE_CURRENT_SOURCE_DIR}/link.ld"
)