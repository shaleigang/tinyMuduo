cmake_minimum_required(VERSION 3.10)

project(tinyMuduo)
set(CMAKE_CXX_STANDARD 17)

include_directories(${PROJECT_SOURCE_DIR})

add_subdirectory(tinyMuduo/base)
add_subdirectory(tinyMuduo/net)
add_subdirectory(examples)

file(GLOB tmuduo_base_h_file tinyMuduo/base/*.h)
file(GLOB tmuduo_net_h_file tinyMuduo/net/*.h)
file(GLOB tmuduo_rpc_h_file tinyMuduo/net/rpc/*.h)

install(FILES ${tmuduo_base_h_file} DESTINATION ${CMAKE_INSTALL_PREFIX}/include/tinyMuduo/base)
install(FILES ${tmuduo_net_h_file} DESTINATION ${CMAKE_INSTALL_PREFIX}/include/tinyMuduo/net)
install(FILES ${tmuduo_rpc_h_file} DESTINATION ${CMAKE_INSTALL_PREFIX}/include/tinyMuduo/net/rpc)