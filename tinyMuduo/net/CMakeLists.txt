aux_source_directory(. NET_SRCS)
add_library(tmuduo_net SHARED ${NET_SRCS})
install(TARGETS tmuduo_net
        LIBRARY DESTINATION ${CMAKE_INSTALL_PREFIX}/lib)

add_subdirectory(rpc)