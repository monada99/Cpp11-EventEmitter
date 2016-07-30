cmake_minimum_required( VERSION 2.6 )

set( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11" )

message( ${CMAKE_CURRENT_SOURCE_DIR} )

include_directories( ${CMAKE_CURRENT_SOURCE_DIR} )

add_executable( "example" "example.cpp")
