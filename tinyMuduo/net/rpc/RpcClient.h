//
// Created by slg on 8/6/23.
//

#ifndef TINYMUDUO_RPCCLIENT_H
#define TINYMUDUO_RPCCLIENT_H

#include <tinyMuduo/base/Logger.h>
#include <tinyMuduo/net/EventLoop.h>
#include <tinyMuduo/net/InetAddress.h>
#include <tinyMuduo/net/TcpClient.h>
#include <tinyMuduo/net/rpc/RpcChannel.h>
#include <tinyMuduo/net/Callback.h>
#include <tinyMuduo/base/BlockingQueue.h>

#include <google/protobuf/stubs/callback.h>

namespace tmuduo {
namespace net {

class RpcClient : noncopyable {
public:
//    typedef std::function<void(google::protobuf::Message*)> RpcResponseCallback;
//    typedef std::function<void()> taskType;
//
//    typedef std::function<void(google::protobuf::RpcChannel* controller,
//                               google::protobuf::Message* request,
//                               google::protobuf::Message* response,
//                               const RpcResponseCallback& cb)> ServiceCallMethodFunc;

    RpcClient(EventLoop* loop, const InetAddress& serverAddr)
            : loop_(loop),
              client_(loop, serverAddr, "RpcClient") {
        client_.setConnectionCallback(
                std::bind(&RpcClient::onConnection, this, _1));

    }

    void setNewConnectionCallback(ConnectionCallback cb) { newConnectionCallback_ = cb; }

    void connect() {
        client_.connect();
    }

    void disconnect() {
        client_.disconnect();
    }

private:
    void onConnection(const TcpConnectionPtr& conn);
    EventLoop* loop_;
    TcpClient client_;
    RpcChannelPtr channel_;
    ConnectionCallback newConnectionCallback_;
};

}
}


#endif //TINYMUDUO_RPCCLIENT_H
