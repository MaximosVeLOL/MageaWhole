cmake_minimum_required(VERSION 3.10)
project(core)


add_library(core STATIC
	#"common.hpp" #"common.cpp"
	#"renderer.hpp" "renderer.cpp"
	"source/renderer.cpp"
	"source/common.cpp"
	"source/input.cpp"
	"source/audio.cpp"
	"source/math.cpp" "source/file.cpp")


#if (CMAKE_VERSION VERSION_GREATER 3.12)
#  set_property(TARGET core PROPERTY CXX_STANDARD 20)
#endif()


target_include_directories(core PUBLIC include)

target_link_libraries(core
	PUBLIC SDL3::SDL3
	PUBLIC SDL3_mixer::SDL3_mixer
)

