//
// Created by slg on 6/27/23.
//

#ifndef TINYMUDUO_TCPCPNNECTION_H
#define TINYMUDUO_TCPCPNNECTION_H

#include <tinyMuduo/base/noncopyable.h>
#include <tinyMuduo/base/types.h>
#include <tinyMuduo/base/Timestamp.h>
#include <tinyMuduo/net/EventLoop.h>
#include <tinyMuduo/net/Socket.h>
#include <tinyMuduo/net/InetAddress.h>
#include <tinyMuduo/net/Buffer.h>
#include <tinyMuduo/net/Channel.h>
#include <tinyMuduo/net/Callback.h>

#include <any>

namespace tmuduo {
namespace net {

class TcpConnection : noncopyable, public std::enable_shared_from_this<TcpConnection>{
public:
    TcpConnection(EventLoop* loop,
                  const string& name,
                  int sockfd,
                  const InetAddress& localAddr,
                  const InetAddress& peerAddr);
    ~TcpConnection();

    EventLoop* getLoop() const { return loop_; }
    const string& name() const { return name_; }
    const InetAddress& localAddress() const { return localAddr_; }
    const InetAddress& peerAddress() const { return peerAddr_; }
    bool connected() const { return state_ == kConnected; }
    bool disconnected() const { return state_ == kDisconnected; }

    void setContext(const std::any& context) { context_ = context; }

    const std::any& getContext() const { return context_; }

    std::any* getMutableContext() { return &context_; }

    void send(const string& message);
    void send(Buffer* message);
    void shutdown();
    void forceClose();
    void forceCloseInLoop();

    void setConnectionCallback(const ConnectionCallback& cb) { connectionCallback_ = cb; }
    void setMessageCallback(const MessageCallback& cb) { messageCallback_ = cb; }
    void setWriteCompleteCallback(const WriteCompleteCallback& cb) { writeCompleteCallback_ = cb; }
    void setHighWaterMarkCallback(const HighWaterMarkCallback& cb, size_t highWaterMark) {
        highWaterMarkCallback_ = cb; highWaterMark_ = highWaterMark;
    }
    void setCloseCallback(const CloseCallback &cb) { closeCallback_ = cb; }

    void connectEstablished();
    void connectDestroyed();

private:
    enum StateE { kDisconnected, kConnecting, kConnected, kDisconnecting };
    void setState(StateE s) { state_ = s; }

    void handleRead(Timestamp receiveTime);
    void handleWrite();
    void handleClose();
    void handleError();

    void sendInLoop(const void* message, ssize_t len);
    void shutdownInLoop();

    EventLoop* loop_;
    const string name_;
    StateE state_;
    bool reading_;

    std::unique_ptr<Socket> socket_;
    std::unique_ptr<Channel> channel_;
    const InetAddress localAddr_;
    const InetAddress peerAddr_;
    Buffer inputBuffer_;
    Buffer outputBuffer_;

    ConnectionCallback connectionCallback_;
    MessageCallback messageCallback_;
    WriteCompleteCallback writeCompleteCallback_;
    HighWaterMarkCallback highWaterMarkCallback_;
    CloseCallback closeCallback_;
    size_t highWaterMark_;

    std::any context_;
};

} // namespace net
} // namespace tmuduo
#endif //TINYMUDUO_TCPCPNNECTION_H
