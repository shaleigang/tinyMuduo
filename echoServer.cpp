//
// Created by slg on 6/25/23.
//

#include "base/Logger.h"
#include "base/Timestamp.h"
#include "net/TcpServer.h"
#include "net/EventLoop.h"
#include "net/TcpConnection.h"

using namespace tmuduo;
using namespace tmuduo::net;

void onConnection(const TcpConnectionPtr &conn) {
    LOG_INFO("EchoServer - %s -> %s is %s",
             conn->peerAddress().toIpPort().c_str(),
             conn->localAddress().toIpPort().c_str(),
             (conn->connected() ? "UP" : "DOWN"));
}

void onMessage(const TcpConnectionPtr &conn,
               Buffer *buf,
               Timestamp time) {
    tmuduo::string msg(buf->retrieveAllAsString());
    LOG_INFO("%s echo %d bytes, data received at", conn->name().c_str(), msg.size(), time.toString().c_str());
    conn->send(msg);
}

int main(int argc, const char *argv[])
{
    EventLoop loop;
    InetAddress addr("127.0.0.1", 8988);
    TcpServer server(&loop, addr, "echo");
    server.setConnectionCallback(&onConnection);
    server.setMessageCallback(&onMessage);
    server.start();
    loop.loop();
    return 0;
}
