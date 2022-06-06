include(FetchContent)
FetchContent_Declare(
  cli11
  GIT_REPOSITORY https://github.com/CLIUtils/CLI11
  GIT_TAG        v2.2.0
)

FetchContent_MakeAvailable(cli11)

# for inculde.
# target_link_libraries(${APP_NAME} PRIVATE cli11)
target_include_directories(${APP_NAME} SYSTEM PRIVATE ${cli11_SOURCE_DIR}/include)
