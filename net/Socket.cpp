//
// Created by sha on 6/20/23.
//

#include "Socket.h"
#include "InetAddress.h"
#include "SocketOps.h"
#include "../base/Logger.h"

#include <sys/socket.h>
#include <cstring>
#include <netinet/in.h>
#include <netinet/tcp.h>

using namespace tmuduo::net;

Socket::~Socket() {
    sockets::close(sockfd_);
}

void Socket::bindAddress(const tmuduo::net::InetAddress &localaddr) {
    sockets::bindOrDie(sockfd_, localaddr.getSockAddr());
}

void Socket::listen() {
    sockets::listenOrDie(sockfd_);
}

int Socket::accept(tmuduo::net::InetAddress *peeraddr) {
    struct sockaddr_in addr;
    memset(&addr, 0, sizeof(addr));
    socklen_t addrlen = static_cast<socklen_t>(sizeof addr);
    int connfd = sockets::accept(sockfd_, &addr);
    if(connfd >= 0){
        peeraddr->setSockAddr(addr);
    }
    return connfd;
}

void Socket::shutdownWrite() {
    sockets::shutdownWrite(sockfd_);
}

void Socket::setTcpNoDelay(bool on) {
    int optval = on ? 1 : 0;
    ::setsockopt(sockfd_, IPPROTO_TCP, TCP_NODELAY,
                 &optval, static_cast<socklen_t>(sizeof optval));
}

void Socket::setReuseAddr(bool on) {
    int optval = on ? 1 : 0;
    ::setsockopt(sockfd_, SOL_SOCKET, SO_REUSEADDR,
                 &optval, static_cast<socklen_t>(sizeof optval));
}

void Socket::setReusePort(bool on) {
    int optval = on ? 1 : 0;
    int ret = ::setsockopt(sockfd_, SOL_SOCKET, SO_REUSEPORT,
                           &optval, static_cast<socklen_t>(sizeof optval));
    if (ret < 0 && on)
    {
        LOG_ERROR("SO_REUSEPORT failed.");
    }
}

void Socket::setKeepAlive(bool on) {
    int optval = on ? 1 : 0;
    ::setsockopt(sockfd_, SOL_SOCKET, SO_KEEPALIVE,
                 &optval, static_cast<socklen_t>(sizeof optval));
}


