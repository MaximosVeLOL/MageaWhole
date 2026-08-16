cmake_minimum_required(VERSION 3.10)
project(core)

# Enable Hot Reload for MSVC compilers if supported.
if (POLICY CMP0141)
  cmake_policy(SET CMP0141 NEW)
  set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif()

add_library(core SHARED
	#"common.hpp" #"common.cpp"
	#"renderer.hpp" "renderer.cpp"
	"source/common.cpp"
	"source/comp/renderer.cpp"
	"source/comp/input.cpp"
	"source/comp/audio.cpp"
	"source/math.cpp"
	"source/file.cpp"
)
target_include_directories(core PUBLIC include)


#if (CMAKE_VERSION VERSION_GREATER 3.12)
#  set_property(TARGET core PROPERTY CXX_STANDARD 20)
#endif()

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET core PROPERTY CXX_STANDARD 20)
endif()

#set_target_properties(core PROPERTIES WINDOWS_EXPORT_ALL_SYMBOLS ON)


target_link_libraries(core
	PUBLIC SDL3::SDL3
	PUBLIC SDL3_mixer::SDL3_mixer
)