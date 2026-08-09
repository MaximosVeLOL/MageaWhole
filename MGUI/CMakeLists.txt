cmake_minimum_required(VERSION 3.10)
project(MGUI)


add_library(MGUI STATIC
	"source/widget.cpp"
	"source/handler.cpp"
	"source/style.cpp")

target_include_directories(MGUI PUBLIC include)

target_link_libraries(MGUI PRIVATE SDL3::SDL3)
target_link_libraries(MGUI PRIVATE SDL3_mixer::SDL3_mixer)
target_link_libraries(MGUI PUBLIC core)

