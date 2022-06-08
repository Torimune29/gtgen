include(FetchContent)
FetchContent_Populate(
  jsoncons
  GIT_REPOSITORY https://github.com/danielaparker/jsoncons
  GIT_TAG        v0.168.7
  QUIET
  SUBBUILD_DIR ${FETCHCONTENT_BASE_DIR}/jsoncons-subbuild
  SOURCE_DIR ${FETCHCONTENT_BASE_DIR}/jsoncons-src
  BINARY_DIR ${FETCHCONTENT_BASE_DIR}/jsoncons-build
)

# for inculde.
target_set_warnings(${jsoncons_jsoncons} DISABLE ALL DISABLE Annoying) # No warnings.
target_include_directories(${APP_NAME} SYSTEM PRIVATE ${jsoncons_SOURCE_DIR}/include)