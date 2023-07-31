//
// Created by slg on 7/31/23.
//

#ifndef TINYMUDUO_TCPCLIENT_H
#define TINYMUDUO_TCPCLIENT_H

#include <tinyMuduo/net/TcpConnection.h>
#include <tinyMuduo/base/types.h>

namespace tmuduo {
namespace net {

class Connector;
typedef std::shared_ptr<Connector> ConnectorPtr;

class TcpClient : noncopyable {
public:
    TcpClient(EventLoop* loop,
              const InetAddress& serverAddr,
              const string& nameArg);
    ~TcpClient();

    void connect();
    void disconnect();
    void stop();

    TcpConnectionPtr connection() const {
        std::unique_lock<mutex> lock(mutex_);
        return connection_;;
    }

    EventLoop* getLoop() const { return loop_; }
    bool retry() const { return retry_; }
    const string& name() const { return name_; }

    void enableRetry() { retry_ = true; }
    void setConnectionCallback(ConnectionCallback cb) {
        connectionCallback_ = std::move(cb);
    }
    void setMessageCallback(MessageCallback cb) {
        messageCallback_ = std::move(cb);
    }
    void setWriteCompleteCallback(WriteCompleteCallback cb) {
        writeCompleteCallback_ = std::move(cb);
    }

private:
    void newConnection(int sockfd);
    void removeConnection(const TcpConnectionPtr& conn);

    EventLoop* loop_;
    ConnectorPtr connector_;
    const string name_;
    ConnectionCallback connectionCallback_;
    MessageCallback messageCallback_;
    WriteCompleteCallback writeCompleteCallback_;
    bool retry_;
    bool connect_;

    int nextConnId_;
    mutable mutex mutex_;
    TcpConnectionPtr connection_;
};

} // net
} // tmuduo


#endif //TINYMUDUO_TCPCLIENT_H
