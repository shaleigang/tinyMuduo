//
// Created by sha on 6/20/23.
//

#include "Socket.h"
#include "InetAddress.h"

#include <unistd.h>
#include <sys/socket.h>
#include <cstring>

using namespace tmuduo::net;

Socket::~Socket() {
    if(::close(sockfd_) < 0){
        // LOG
    }
}

void Socket::bindAddress(const tmuduo::net::InetAddress &localaddr) {
    int ret = ::bind(sockfd_, localaddr.getSockAddr(), sizeof(struct sockaddr_in));
    if(ret < 0){
        // LOG
    }
}

void Socket::listen() {
    int ret = ::listen(sockfd_, SOMAXCONN);
    if(ret < 0){
        // LOG
    }
}

int Socket::accept(tmuduo::net::InetAddress *peeraddr) {
    struct sockaddr_in addr;
    memset(&addr, 0, sizeof(addr));
    socklen_t addrlen = static_cast<socklen_t>(sizeof addr);
    int connfd = ::accept4(sockfd_, (struct sockaddr *)&addr, &addrlen, SOCK_NONBLOCK | SOCK_CLOEXEC);
    if(connfd >= 0){
        peeraddr->setSockAddr(addr);
    }
    return connfd;
}

void Socket::shutdownWrite() {
    if(::shutdown(sockfd_, SHUT_WR)){
        // LOG
    }
}


