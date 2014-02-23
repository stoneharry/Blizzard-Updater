cmake_minimum_required(VERSION 2.0)

project(BsnI)

set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake/")

if(WIN32)
  set(Boost_USE_STATIC_LIBS   ON)
  set(Boost_USE_MULTITHREADED ON)
endif(WIN32)

find_package(ZLIB REQUIRED)
include_directories(${ZLIB_INCLUDE_DIR})

find_package(StormLib REQUIRED)
include_directories(${STORMLIB_INCLUDE_DIR})

find_package(Boost COMPONENTS system filesystem program_options REQUIRED)
include_directories(${Boost_INCLUDE_DIR})

include_directories(${CMAKE_CURRENT_SOURCE_DIR})

add_executable(extract extract_bsni.cpp zlib.cpp)
target_link_libraries(extract ${ZLIB_LIBRARIES})
add_executable(append append_bsni.cpp zlib.cpp)
target_link_libraries(append ${ZLIB_LIBRARIES} ${Boost_LIBRARIES})
add_executable(headergenerator headergenerator.cpp)
target_link_libraries(headergenerator ${Boost_LIBRARIES})
add_executable(listgenerator listgenerator.cpp)
target_link_libraries(listgenerator ${Boost_LIBRARIES})
add_executable(mpqcreate mpqcreate.cpp)
target_link_libraries(mpqcreate ${Boost_LIBRARIES} ${STORMLIB_LIBRARIES})
