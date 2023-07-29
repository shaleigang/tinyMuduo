//
// Created by slg on 7/8/23.
//

#ifndef TINYMUDUO_RPCSERVER_H
#define TINYMUDUO_RPCSERVER_H

#include <map>
#include <google/protobuf/service.h>

#include <tinyMuduo/net/TcpServer.h>

namespace tmuduo {
namespace net {

class RpcServer {
public:
    RpcServer(EventLoop* loop, const InetAddress &listenAddr);

    void setThreadNum(int n);

    void registerService(google::protobuf::Service* service);

    void start();

private:
    void onConnection(const TcpConnectionPtr& conn);

    TcpServer server_;
    std::map<string, google::protobuf::Service*> services_;
};

}
}
#endif //TINYMUDUO_RPCSERVER_H
