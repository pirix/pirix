add_library(pirix STATIC
  pirix/${ARCH}/syscall.S
  pirix/libgloss.c
)
set_source_files_properties(pirix/syscall.S PROPERTIES LANGUAGE C)

add_library(crt0 STATIC
  pirix/${ARCH}/crt0.S
)
set_target_properties(crt0 PROPERTIES PREFIX "" SUFFIX ".o")

set_source_files_properties(pirix/${ARCH}/crt0.S PROPERTIES LANGUAGE C)

install(TARGETS pirix crt0 ARCHIVE DESTINATION lib)