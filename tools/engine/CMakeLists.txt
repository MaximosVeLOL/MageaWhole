cmake_minimum_required(VERSION 3.10)
project(MageaDevTools)

#Developer tools for Magea,
#Ranging from in-engine tools
#To basic command utilities

add_library(MageaDevTools SHARED
	"source/tEditor.cpp"
	"source/tSprite.cpp"
	"source/tDebugger.cpp"
)
target_include_directories(MageaDevTools PRIVATE include)

target_link_libraries(MageaDevTools PRIVATE core MGUI Magea SDL3::SDL3)