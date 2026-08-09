cmake_minimum_required(VERSION 3.10)

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

target_link_libraries(MageaGame PUBLIC core)
target_link_libraries(MageaGame PUBLIC MGUI)
target_link_libraries(MageaGame PUBLIC Magea)
#target_link_libraries(MageaGame PUBLIC Game)
target_link_libraries(MageaGame PUBLIC SDL3::SDL3)
