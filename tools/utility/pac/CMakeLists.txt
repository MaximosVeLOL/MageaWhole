cmake_minimum_required(VERSION 3.10)


project("PackedFileUtil")

add_executable(PackedFileUtil "packedfile.cpp")

target_link_libraries(PackedFileUtil PRIVATE core)