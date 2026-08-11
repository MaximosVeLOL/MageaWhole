cmake_minimum_required(VERSION 3.10)


# Enable Hot Reload for MSVC compilers if supported.
if (POLICY CMP0141)
  cmake_policy(SET CMP0141 NEW)
  set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif()


add_subdirectory(core)
add_subdirectory(MGUI)
add_subdirectory(Magea)
add_subdirectory(Game)



set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/$<CONFIGURATION>")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/$<CONFIGURATION>")

if(EMSCRIPTEN)
  set(CMAKE_EXECUTABLE_SUFFIX ".html" CACHE INTERNAL "")
endif()





add_subdirectory(vendored/SDL EXCLUDE_FROM_ALL)
add_subdirectory(vendored/SDL_mixer EXCLUDE_FROM_ALL)
set(SDL_INSTALL OFF CACHE BOOL "" FORCE) 

project(MageaGame)

add_executable(MageaGame
	main.cpp
)

#Uses MGUI & SDL?
target_link_libraries(MageaGame PRIVATE Game)
#Uses SDL
target_link_libraries(MageaGame PUBLIC core)
#Uses core & SDL
target_link_libraries(MageaGame PUBLIC MGUI)
#Uses everything but Game
target_link_libraries(MageaGame PUBLIC Magea)
#Uses nothing
target_link_libraries(MageaGame PUBLIC SDL3::SDL3)

