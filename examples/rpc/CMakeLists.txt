find_package(Protobuf REQUIRED)

PROTOBUF_GENERATE_CPP(PROTO_SRCS PROTO_HDRS order.proto)

add_executable(rpc_server rpc_server.cpp ${PROTO_SRCS} ${PROTO_HDRS})
target_link_libraries(rpc_server tmuduo_rpc ${PROTOBUF_LIBRARIES} tmuduo_net tmuduo_base pthread)

add_executable(rpc_client rpc_client.cpp ${PROTO_SRCS} ${PROTO_HDRS})
target_link_libraries(rpc_client tmuduo_rpc ${PROTOBUF_LIBRARIES} tmuduo_net tmuduo_base pthread)