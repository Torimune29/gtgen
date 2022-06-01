
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_CURRENT_SOURCE_DIR}/cmake/common")
message(STATUS "Read Compile Config")
include(ConfigSafeGuards)
include(Colors)
include(Documentation)
include(LTO)
include(Misc)
include(Warnings)
include(Ccache)
message(STATUS "Read Compile Config done")

