include(FetchContent)
FetchContent_Declare(
  jsoncons
  GIT_REPOSITORY https://github.com/danielaparker/jsoncons
  GIT_TAG        v0.168.7
)

FetchContent_MakeAvailable(jsoncons)

# for inculde.
target_set_warnings(${jsoncons_jsoncons} DISABLE ALL Annoying) # No warnings.
# target_link_libraries(${APP_NAME} PRIVATE jsoncons)
target_include_directories(${APP_NAME} SYSTEM PRIVATE ${jsoncons_SOURCE_DIR}/include)