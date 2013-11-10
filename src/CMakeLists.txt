cmake_minimum_required (VERSION 2.0)

project (BsnI)

find_package (ZLIB REQUIRED)
include_directories (${ZLIB_INCLUDE_DIR})

include_directories (${CMAKE_CURRENT_SOURCE_DIR})

add_executable (extract extract_bsni.cpp zlib.cpp)
target_link_libraries (extract ${ZLIB_LIBRARIES})
add_executable (append append_bsni.cpp zlib.cpp)
target_link_libraries (append ${ZLIB_LIBRARIES})
