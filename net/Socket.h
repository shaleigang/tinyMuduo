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

    ~Socket();

    int fd() const { return sockfd_; }

    void bindAddress(const InetAddress &localaddr);
    void listen();
    int accept(InetAddress* peeraddr);
    void shutdownWrite();


private:
    const int sockfd_;
};

} // namespace net
} // namespace tmuduo

#endif //TINYMUDUO_SOCKET_H
