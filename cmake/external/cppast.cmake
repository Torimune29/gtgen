include(FetchContent)
FetchContent_Declare(
  cppast
  GIT_REPOSITORY https://github.com/foonathan/cppast
  GIT_TAG        v0.1.0
)

FetchContent_MakeAvailable(cppast)

set(cppast_BUILD_TESTING OFF)

# for inculde.
target_set_warnings(cppast DISABLE ALL DISABLE Annoying) # No warnings.
target_include_directories(${LIBRARY_NAME} SYSTEM PRIVATE ${cppast_SOURCE_DIR}/cppast/include)
target_link_libraries(${LIBRARY_NAME} PRIVATE cppast)
target_compile_options(${LIBRARY_NAME} PRIVATE "-Wno-weak-vtables")  # for cppast virtual on headerfile inline.
