aux_source_directory(. RPC_SRCS)
add_library(tmuduo_rpc SHARED ${RPC_SRCS})
install(TARGETS tmuduo_rpc
        LIBRARY DESTINATION ${CMAKE_INSTALL_PREFIX}/lib)