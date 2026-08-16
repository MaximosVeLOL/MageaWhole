cmake_minimum_required(VERSION 3.10)
project(MageaDevTools)


add_library(MageaDevTools SHARED

)
target_include_directories(MageaDevTools private include/)

target_link_libraries(MageaDevTools core)
target_link_libraries(MageaDevTools MGUI)
target_link_libraries(MageaDevTools Magea)
target_link_libraries(MageaDevTools SDL3::SDL3)