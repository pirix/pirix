add_library(util STATIC
  string.c
  div.c
)

add_library(system STATIC
  system/syscall.S
)

set_source_files_properties(system/syscall.S PROPERTIES LANGUAGE C)