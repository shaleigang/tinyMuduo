//
// Created by sha on 6/20/23.
//

#ifndef TINYMUDUO_SOCKET_H
#define TINYMUDUO_SOCKET_H

#include "../base/noncopyable.h"

namespace tmuduo {

namespace net {

class InetAddress;

class Socket : tmuduo::noncopyable {
public:
    explicit Socket(int sockfd)
            : sockfd_(sockfd) {}

    // 析构函数负责关闭文件描述符
    ~Socket();

    int fd() const { return sockfd_; }

    void bindAddress(const InetAddress &localaddr);
    void listen();
    int accept(InetAddress* peeraddr);
    void shutdownWrite();

    void setTcpNoDelay(bool on);
    void setReuseAddr(bool on);
    void setReusePort(bool on);
    void setKeepAlive(bool on);

private:
    const int sockfd_;
};

} // namespace net
} // namespace tmuduo

#endif //TINYMUDUO_SOCKET_H
