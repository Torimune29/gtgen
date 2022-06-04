include(FetchContent)
FetchContent_Declare(
  cppast
  GIT_REPOSITORY https://github.com/foonathan/cppast
  GIT_TAG        v0.1.0
)

FetchContent_MakeAvailable(cppast)

# for inculde.
target_set_warnings(cppast DISABLE ALL Annoying) # No warnings.
target_include_directories(cppast SYSTEM INTERFACE ${cppast_SOURCE_DIR}/cppast/include)