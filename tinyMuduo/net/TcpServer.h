//
// Created by slg on 6/27/23.
//

#ifndef TINYMUDUO_TCPSERVER_H
#define TINYMUDUO_TCPSERVER_H

#include <tinyMuduo/base/noncopyable.h>
#include <tinyMuduo/net/Acceptor.h>
#include <tinyMuduo/net/EventLoop.h>
#include <tinyMuduo/net/EventLoopThreadPool.h>
#include <tinyMuduo/net/TcpConnection.h>
#include <tinyMuduo/net/Callback.h>

#include <atomic>
#include <map>

namespace tmuduo {
namespace net {

class TcpServer : noncopyable {
public:
    typedef std::function<void(EventLoop*)> ThreadInitCallback;
    enum Option { kNoReusePort, kReusePort };

    TcpServer(EventLoop *loop,
              const InetAddress &listenAddr,
              const std::string &nameArg,
              Option option = kNoReusePort);
    ~TcpServer();

    const string& ipPort() const { return ipPort_; }
    const string& name() const { return name_; }

    void setThreadInitCallback(const ThreadInitCallback &cb) { threadInitCallback_ = cb; }
    void setConnectionCallback(const ConnectionCallback &cb) { connectionCallback_ = cb; }
    void setMessageCallback(const MessageCallback &cb) { messageCallback_ = cb; }
    void setWriteCompleteCallback(const WriteCompleteCallback &cb) { writeCompleteCallback_ = cb; }

    void setThreadNum(int numThreads);

    void start();

private:
    typedef std::map<string, TcpConnectionPtr> ConnectionMap;

    void newConnection(int sockfd, const InetAddress &peerAddr);
    void removeConnection(const TcpConnectionPtr &conn);
    void removeConnectionInLoop(const TcpConnectionPtr &conn);

    EventLoop* loop_;
    const string ipPort_;
    const string name_;
    std::unique_ptr<Acceptor> acceptor_;
    std::shared_ptr<EventLoopThreadPool> threadPool_;

    ConnectionCallback connectionCallback_;
    MessageCallback messageCallback_;
    WriteCompleteCallback writeCompleteCallback_;

    ThreadInitCallback threadInitCallback_;

    std::atomic_int started_;

    int nextConnId_;
    ConnectionMap connections_;
};

} // namespace net
} // namespace tmuduo
#endif //TINYMUDUO_TCPSERVER_H
