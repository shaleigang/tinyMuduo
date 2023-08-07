//
// Created by slg on 7/31/23.
//

#include "order.pb.h"

#include <tinyMuduo/base/Logger.h>
#include <tinyMuduo/net/EventLoop.h>
#include <tinyMuduo/net/InetAddress.h>
#include <tinyMuduo/net/TcpClient.h>
#include <tinyMuduo/net/rpc/RpcChannel.h>
#include <tinyMuduo/net/rpc/RpcClient.h>

#include <unistd.h>

using namespace tmuduo;
using namespace tmuduo::net;

class myClient {
public:
    myClient(EventLoop* loop, const InetAddress& serverAddr)
      : loop_(loop),
        client_(loop, serverAddr) {
        client_.setNewConnectionCallback(std::bind(&myClient::onConnection, this, _1));
    }

    void orderTask() {
        loop_->runInLoop(std::bind(&myClient::makeOrder, this));
    }

    void connect() {
        client_.connect();
    }

    void disconnect() {
        client_.disconnect();
    }

private:
    void makeOrder() {
        makeOrderRequest request;
        makeOrderResponse* response = new makeOrderResponse;
        stub_->makeOrder(nullptr, &request, response, NewCallback(this, &myClient::getResp, response));
    }

    void getResp(makeOrderResponse* resp) {
        LOG_INFO("getResponse %s", resp->res_info().c_str());
    }

    void onConnection(const TcpConnectionPtr& conn) {
        LOG_DEBUG("stub_ init");
        stub_ = new Order::Stub(std::any_cast<std::shared_ptr<RpcChannel>>(conn->getContext()).get());
    }

    EventLoop* loop_;
    RpcClient client_;
    Order::Stub* stub_;
};

void add(myClient* client) {
    while(1) {
        sleep(1);
        LOG_DEBUG("add task");
        client->orderTask();
    }
}

int main(int argc, char* argv[]) {
    EventLoop loop;
    InetAddress serverAddr("127.0.0.1", 9981);

    myClient rpcClient(&loop, serverAddr);
    rpcClient.connect();
    std::thread t(add, &rpcClient);
    loop.loop();

    google::protobuf::ShutdownProtobufLibrary();
}