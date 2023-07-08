//
// Created by slg on 7/8/23.
//

#include <google/protobuf/descriptor.h>

#include "RpcServer.h"

using namespace tmuduo;
using namespace tmuduo::net;

RpcServer::RpcServer(EventLoop *loop, const InetAddress &listenAddr)
  : server_(loop, listenAddr, "rpcServer"){
    server_.setConnectionCallback(
            std::bind(&RpcServer::onConnection, this, _1));
}

void RpcServer::setThreadNum(int n) {
    server_.setThreadNum(n);
}

void RpcServer::registerService(google::protobuf::Service *service) {
    const google::protobuf::ServiceDescriptor* desc = service->GetDescriptor();
    services_[desc->full_name()] = service;
}

void RpcServer::start() {
    server_.start();
}
