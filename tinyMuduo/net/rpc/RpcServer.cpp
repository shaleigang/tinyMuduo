//
// Created by slg on 7/8/23.
//

#include <google/protobuf/descriptor.h>

#include <memory>

#include <tinyMuduo/base/Logger.h>
#include <tinyMuduo/net/rpc/RpcChannel.h>
#include <tinyMuduo/net/rpc/RpcServer.h>

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

void RpcServer::onConnection(const TcpConnectionPtr& conn)
{
    LOG_INFO("RpcServer UP");
    if (conn->connected())
    {
        RpcChannelPtr channel(new RpcChannel(conn));
        channel->setServices(&services_);
        conn->setMessageCallback(
                std::bind(&RpcChannel::onMessage, channel.get(), _1, _2, _3));
    }
}