cmake_minimum_required(VERSION 3.10)

# Enable Hot Reload for MSVC compilers if supported.
if (POLICY CMP0141)
  
endif()
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} /INCREMENTAL:YES" )
cmake_policy(SET CMP0141 NEW)
set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} /MDd /ZI /Ob0 /Od /RTC1")

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/$<CONFIGURATION>")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/$<CONFIGURATION>")

#Vendored projects go here

add_subdirectory(vendored/SDL EXCLUDE_FROM_ALL)
add_subdirectory(vendored/SDL_mixer EXCLUDE_FROM_ALL)
set(SDL_INSTALL OFF CACHE BOOL "" FORCE) 

#add_subdirectory(vendored/angelscript/angelscript/projects/cmake)
add_subdirectory(vendored/miniz)

#My projects go here
add_subdirectory(core)
add_subdirectory(MGUI)
add_subdirectory(Magea)
add_subdirectory(Game)
set(DEBUG true)
if(CMAKE_BUILD_TYPE STREQUAL "RelWithDebInfo")
    set(DEBUG false)
endif()
if(DEBUG)
    add_subdirectory(tools)
endif()
if(EMSCRIPTEN)
  set(CMAKE_EXECUTABLE_SUFFIX ".html" CACHE INTERNAL "")
endif()

project(MageaGame)

add_executable(MageaGame main.cpp )

set_property(TARGET MageaGame core MGUI Game Magea MageaDevTools PackedFileUtil PROPERTY CXX_STANDARD 23)
#target_compile_features(MageaGame core MGUI Game Magea MageaDevTools PRIVATE cxx_std_23)

#if (CMAKE_VERSION VERSION_GREATER 3.12)
#endif()

#Uses SDL
target_link_libraries(MageaGame PRIVATE core)
#Uses core & SDL
target_link_libraries(MageaGame PRIVATE MGUI)
#Uses everything but Game
target_link_libraries(MageaGame PRIVATE Magea)
#Uses nothing
target_link_libraries(MageaGame PRIVATE SDL3::SDL3)
#Uses MGUI & SDL?
target_link_libraries(MageaGame PRIVATE Game)

target_link_libraries(MageaGame PRIVATE angelscript)


if(DEBUG)
    target_link_libraries(MageaGame PRIVATE MageaDevTools)
endif()
# Post-build step to copy DLLs
#add_custom_command(TARGET MageaGame POST_BUILD
#    COMMAND ${CMAKE_COMMAND} -E copy_if_different
#        $<TARGET_RUNTIME_DLLS:MageaGame>
#        $<TARGET_FILE_DIR:MageaGame>
#    COMMAND_EXPAND_LISTS
#)

#file(RENAME "C:/source/MageaWhole/out/build/x64-Debug/Magea/Magea.dll" "C:/source/MageaWhole/out/build/x64-Debug/Magea.dll")
#file(RENAME "C:/source/MageaWhole/out/build/x64-Debug/MGUI/MGUI.dll" "C:/source/MageaWhole/out/build/x64-Debug/MGUI.dll")
#file(RENAME "C:/source/MageaWhole/out/build/x64-Debug/core/core.dll" "C:/source/MageaWhole/out/build/x64-Debug/core.dll")
#[===[
if(WIN32)
    add_custom_command(
        TARGET MageaGame POST_BUILD
        COMMAND "${CMAKE_COMMAND}" -E copy $<TARGET_FILE:core> $<TARGET_FILE_DIR:MageaGame>
        COMMAND "${CMAKE_COMMAND}" -E copy $<TARGET_FILE:MGUI> $<TARGET_FILE_DIR:MageaGame>
        COMMAND "${CMAKE_COMMAND}" -E copy $<TARGET_FILE:Magea> $<TARGET_FILE_DIR:MageaGame>
        VERBATIM
    )
endif()
]===]