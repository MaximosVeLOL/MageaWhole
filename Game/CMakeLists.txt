cmake_minimum_required(VERSION 3.10)
#This is an example of a shared library that uses Magea
project(Game)

add_library(Game SHARED
	"source/objectDefs.cpp"
	"source/game.cpp"
	"source/engine/api.cpp" )

target_include_directories(Game PRIVATE include)

#target_link_libraries(Game PRIVATE SDL3::SDL3)

set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)

#target_link_libraries(Game PRIVATE Magea)