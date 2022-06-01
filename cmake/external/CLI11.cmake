include(FetchContent)
FetchContent_Declare(
  cli11
  GIT_REPOSITORY https://github.com/CLIUtils/CLI11
  GIT_TAG        v2.2.0
)

FetchContent_MakeAvailable(cli11)

# for inculde.
add_library(cli11 INTERFACE)
target_include_directories(cli11 SYSTEM INTERFACE ${cli11_SOURCE_DIR}/include)
