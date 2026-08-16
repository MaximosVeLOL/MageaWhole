cmake_minimum_required(VERSION 3.10)
project(MGUI)

# Enable Hot Reload for MSVC compilers if supported.
if (POLICY CMP0141)
  cmake_policy(SET CMP0141 NEW)
  set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif()

add_library(MGUI SHARED
	"source/widget.cpp"
	"source/handler.cpp"
	"source/style.cpp")

target_include_directories(MGUI PUBLIC include)

#set_target_properties(MGUI PROPERTIES WINDOWS_EXPORT_ALL_SYMBOLS ON)


target_link_libraries(MGUI PRIVATE SDL3::SDL3)
target_link_libraries(MGUI PRIVATE SDL3_mixer::SDL3_mixer)
target_link_libraries(MGUI PRIVATE core)
