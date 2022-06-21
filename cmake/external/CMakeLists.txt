set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_CURRENT_SOURCE_DIR}")
message(STATUS "Read External Config")
include(CLI11)
include(cppast)
include(jsoncons)
include(cppcodegen)

message(STATUS "Read External Config done")