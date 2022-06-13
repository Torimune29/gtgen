include(FetchContent)
FetchContent_Declare(
  cppgenerate
  GIT_REPOSITORY https://github.com/rm5248/libcppgenerate
  GIT_TAG        cppgenerate-0.2
)

FetchContent_MakeAvailable(cppgenerate)

# for inculde.
# target_set_warnings(libcppgenerate DISABLE ALL DISABLE Annoying) # No warnings.
target_include_directories(${LIBRARY_NAME} SYSTEM PRIVATE ${cppgenerate_SOURCE_DIR})
target_link_libraries(${LIBRARY_NAME} PRIVATE cppgenerate)
# target_compile_options(${LIBRARY_NAME} PRIVATE "-Wno-weak-vtables")  # for cppast virtual on headerfile inline.
