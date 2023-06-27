//
// Created by slg on 6/26/23.
//

#include "SocketOps.h"
#include "../base/Logger.h"
#include "../base/types.h"

#include <cerrno>
#include <fcntl.h>
#include <cstdio>
#include <sys/socket.h>
#include <sys/uio.h>
#include <unistd.h>
#include <cstring>

using namespace tmuduo;
using namespace tmuduo::net;

int sockets::createNonblockingOrDie() {
    int sockfd = ::socket(AF_INET, SOCK_STREAM | SOCK_NONBLOCK | SOCK_CLOEXEC, IPPROTO_TCP);
    if (sockfd < 0)
    {
        LOG_FATAL("sockets::createNonblockingOrDie");
    }
    return sockfd;
}

void sockets::bindOrDie(int sockfd, const struct sockaddr* addr) {
    int ret = ::bind(sockfd, addr, static_cast<socklen_t>(sizeof(struct sockaddr_in)));
    if (ret < 0)
    {
        LOG_FATAL("sockets::bindOrDie");
    }
}

void sockets::listenOrDie(int sockfd) {
    int ret = ::listen(sockfd, SOMAXCONN);
    if (ret < 0)
    {
        LOG_FATAL("sockets::listenOrDie");
    }
}

int sockets::accept(int sockfd, struct sockaddr_in* addr)
{
    socklen_t addrlen = static_cast<socklen_t>(sizeof *addr);
    int connfd = ::accept4(sockfd, (struct sockaddr*)addr,
                           &addrlen, SOCK_NONBLOCK | SOCK_CLOEXEC);
    if (connfd < 0)
    {
        int savedErrno = errno;
        LOG_ERROR("Socket::accept");
        switch (savedErrno)
        {
            case EAGAIN:
            case ECONNABORTED:
            case EINTR:
            case EPROTO:
            case EPERM:
            case EMFILE:
                errno = savedErrno;
                break;
            case EBADF:
            case EFAULT:
            case EINVAL:
            case ENFILE:
            case ENOBUFS:
            case ENOMEM:
            case ENOTSOCK:
            case EOPNOTSUPP:
                // unexpected errors
                LOG_FATAL("unexpected error of ::accept %d", savedErrno);
                break;
            default:
                LOG_FATAL("unknown error of ::accept %d", savedErrno);
                break;
        }
    }
    return connfd;
}

int sockets::connect(int sockfd, const struct sockaddr* addr)
{
    return ::connect(sockfd, addr, static_cast<socklen_t>(sizeof(struct sockaddr_in)));
}

ssize_t sockets::read(int sockfd, void *buf, size_t count)
{
    return ::read(sockfd, buf, count);
}

ssize_t sockets::readv(int sockfd, const struct iovec *iov, int iovcnt)
{
    return ::readv(sockfd, iov, iovcnt);
}

ssize_t sockets::write(int sockfd, const void *buf, size_t count)
{
    return ::write(sockfd, buf, count);
}

void sockets::close(int sockfd)
{
    if (::close(sockfd) < 0)
    {
        LOG_ERROR("sockets::close");
    }
}

void sockets::shutdownWrite(int sockfd)
{
    if (::shutdown(sockfd, SHUT_WR) < 0)
    {
        LOG_ERROR("sockets::shutdownWrite");
    }
}

int sockets::getSocketError(int sockfd) {
    int optval;
    socklen_t optlen = static_cast<socklen_t>(sizeof optval);

    if (::getsockopt(sockfd, SOL_SOCKET, SO_ERROR, &optval, &optlen) < 0) {
        return errno;
    }
    else {
        return optval;
    }
}

struct sockaddr_in sockets::getLocalAddr(int sockfd) {
    struct sockaddr_in localaddr;
    memset(&localaddr, 0,  sizeof localaddr);
    socklen_t addrlen = static_cast<socklen_t>(sizeof localaddr);
    if (::getsockname(sockfd, (struct sockaddr *)(&localaddr), &addrlen) < 0) {
        LOG_ERROR("sockets::getLocalAddr");
    }
    return localaddr;
}

struct sockaddr_in sockets::getPeerAddr(int sockfd) {
    struct sockaddr_in peeraddr;
    memset(&peeraddr, 0, sizeof peeraddr);
    socklen_t addrlen = static_cast<socklen_t>(sizeof peeraddr);
    if (::getpeername(sockfd, (struct sockaddr *)(&peeraddr), &addrlen) < 0) {
        LOG_ERROR("sockets::getPeerAddr");
    }
    return peeraddr;
}