//
// Created by slg on 7/25/23.
//

#include "order.pb.h"
#include "../../base/Logger.h"
#include "../../base/Timestamp.h"
#include "../../net/rpc/RpcServer.h"
#include <google/protobuf/message.h>


using namespace tmuduo;
using namespace tmuduo::net;

class OrderImp : public Order {
public:
    void makeOrder(google::protobuf::RpcController* controller,
                           const ::makeOrderRequest* request,
                           ::makeOrderResponse* response,
                           ::google::protobuf::Closure* done) override {
        LOG_INFO("Order::makeOrder");
        response->set_ret_code(123);
        response->set_res_info(Timestamp::now().toString());
        response->set_order_id("123456");
        done->Run();
    }
};

int main() {
    EventLoop loop;
    InetAddress listenAddr(9981);
    OrderImp impl;
    RpcServer server(&loop, listenAddr);
    server.registerService(&impl);
    LOG_INFO("RpcServer UP.");
    server.start();
    loop.loop();
    google::protobuf::ShutdownProtobufLibrary();
}