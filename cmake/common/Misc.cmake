# Embedding Version (using CMake configure)
configure_file (
  "${PROJECT_SOURCE_DIR}/include/ProjectVersion.h.in"
  "${PROJECT_BINARY_DIR}/ProjectVersion.h"
)
include_directories("${PROJECT_BINARY_DIR}")

# Ask CMake to output a compile_commands.json file for use with things like Vim YCM.
set(CMAKE_EXPORT_COMPILE_COMMANDS 1)
