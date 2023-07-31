//
// Created by slg on 7/31/23.
//

#include "order.pb.h"

#include <tinyMuduo/base/Logger.h>
#include <tinyMuduo/net/EventLoop.h>
#include <tinyMuduo/net/InetAddress.h>
#include <tinyMuduo/net/TcpClient.h>
#include <tinyMuduo/net/rpc/RpcChannel.h>

using namespace tmuduo;
using namespace tmuduo::net;

class RpcClient : noncopyable {
public:
    RpcClient(EventLoop* loop, const InetAddress& serverAddr)
            : loop_(loop),
              client_(loop, serverAddr, "RpcClient"),
              channel_(new RpcChannel),
              stub_(channel_.get()) {
        client_.setConnectionCallback(
                std::bind(&RpcClient::onConnection, this, _1));
        client_.setMessageCallback(
                std::bind(&RpcChannel::onMessage, channel_.get(), _1, _2, _3));
        // client_.enableRetry();
    }

    void connect() {
        client_.connect();
    }
private:
    void onConnection(const TcpConnectionPtr& conn) {
        if (conn->connected()) {
            channel_->setConnection(conn);
            makeOrderRequest request;
            makeOrderResponse* response = new makeOrderResponse;
            stub_.makeOrder(nullptr, &request, response, NewCallback(this, &RpcClient::getResp, response));
        }
        else {
            loop_->quit();
        }
    }
    void getResp(makeOrderResponse* resp) {
        LOG_INFO("getResponse2 %s", resp->res_info().c_str());
        client_.disconnect();
    }

    EventLoop* loop_;
    TcpClient client_;
    RpcChannelPtr channel_;
    Order::Stub stub_;

};

int main(int argc, char* argv[]) {
    EventLoop loop;
    InetAddress serverAddr("127.0.0.1", 9981);

    RpcClient rpcClient(&loop, serverAddr);
    rpcClient.connect();
    loop.loop();
    google::protobuf::ShutdownProtobufLibrary();
}