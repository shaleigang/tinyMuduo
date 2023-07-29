//
// Created by slg on 6/26/23.
//

#include <tinyMuduo/net/Acceptor.h>
#include <tinyMuduo/net/SocketOps.h>
#include <tinyMuduo/base/Logger.h>

#include <sys/socket.h>
#include <fcntl.h>
#include <unistd.h>

using namespace tmuduo;
using namespace tmuduo::net;

Acceptor::Acceptor(EventLoop *loop, const InetAddress &listenAddr, bool reuseport)
  : loop_(loop),
    acceptSocket_(sockets::createNonblockingOrDie()),
    acceptChannel_(loop, acceptSocket_.fd()),
    listening_(false),
    idleFd_(::open("/dev/null", O_RDONLY | O_CLOEXEC)){

//    LOG_DEBUG("Acceptor fd: %d", acceptChannel_.fd());
    acceptSocket_.setReusePort(reuseport);
    acceptSocket_.setReuseAddr(true);
    acceptSocket_.bindAddress(listenAddr);
    acceptChannel_.setReadCallback(std::bind(&Acceptor::handleRead, this));
}

Acceptor::~Acceptor() {
    acceptChannel_.disableAll();
    acceptChannel_.remove();
    ::close(idleFd_);
}

void Acceptor::listen() {
    loop_->assertInLoopThread();
    listening_ = true;
    acceptSocket_.listen();
    acceptChannel_.enableReading();
    LOG_DEBUG("accepter listening...");
}

void Acceptor::handleRead() {
    loop_->assertInLoopThread();
    InetAddress peerAddr;
    int connfd = acceptSocket_.accept(&peerAddr);
    if(connfd >= 0) {
        if(newConnectionCallback_) {
            newConnectionCallback_(connfd, peerAddr);
        }
        else {
            sockets::close(connfd);
        }
    }
    else {
        LOG_ERROR("in Acceptor::handleRead");
        if (errno == EMFILE) {
            ::close(idleFd_);
            idleFd_ = ::accept(acceptSocket_.fd(), NULL, NULL);
            ::close(idleFd_);
            idleFd_ = ::open("/dev/null", O_RDONLY | O_CLOEXEC);
        }
    }
}