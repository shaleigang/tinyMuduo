add_subdirectory(rpc)
add_executable(echo echoServer.cpp)
target_link_libraries(echo tmuduo_net tmuduo_base pthread)