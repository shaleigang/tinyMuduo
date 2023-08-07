//
// Created by slg on 8/6/23.
//

#include "RpcClient.h"
//#include <memory>

using namespace tmuduo;
using namespace tmuduo::net;

void RpcClient::onConnection(const tmuduo::net::TcpConnectionPtr &conn) {
    if (conn->connected()) {
        channel_ = std::make_shared<RpcChannel>(conn);
        conn->setMessageCallback(
                std::bind(&RpcChannel::onMessage, channel_.get(), _1, _2, _3));
        conn->setContext(channel_);
        newConnectionCallback_(conn);
    }
    else {
        conn->setContext(channel_);
    }
}

