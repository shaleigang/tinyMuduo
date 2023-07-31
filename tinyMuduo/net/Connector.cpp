//
// Created by slg on 7/30/23.
//

#include <tinyMuduo/net/Connector.h>
#include <tinyMuduo/base/Logger.h>
#include <tinyMuduo/net/SocketOps.h>

#include <cassert>

using namespace tmuduo;
using namespace tmuduo::net;

const int Connector::kMaxRetryDelayMs;

Connector::Connector(tmuduo::net::EventLoop *loop, const tmuduo::net::InetAddress &serverAddr)
  : loop_(loop),
    serverAddr_(serverAddr),
    connect_(false),
    state_(kDisconnected),
    retryDelayMs_(kInitRetryDelayMs){
    LOG_DEBUG("ctor Connector.");
}

Connector::~Connector() {
    LOG_DEBUG("dtor Connector.");
    assert(!channel_);
}

void Connector::start() {
    connect_ = true;
    loop_->runInLoop(std::bind(&Connector::startInLoop, this));
}

void Connector::startInLoop() {
    loop_->assertInLoopThread();
    assert(state_ == kDisconnected);
    if (connect_) {
        connect();
    }
    else {
        LOG_DEBUG(" do not connect.");
    }
}

void Connector::connect() {
    int sockfd = sockets::createNonblockingOrDie();
    int ret = sockets::connect(sockfd, serverAddr_.getSockAddr());
    int savedErrno = (ret == 0) ? 0 : errno;
    switch(savedErrno) {
        case 0:
        case EINPROGRESS:
        case EINTR:
        case EISCONN:
            connecting(sockfd);
            break;
        case EAGAIN:
        case EADDRINUSE:
        case EADDRNOTAVAIL:
        case ECONNREFUSED:
        case ENETUNREACH:
            retry(sockfd);
            break;
        case EACCES:
        case EPERM:
        case EAFNOSUPPORT:
        case EALREADY:
        case EBADF:
        case EFAULT:
        case ENOTSOCK:
            LOG_ERROR("connect error in Connector::startInLoop. Errno: %d", savedErrno);
            sockets::close(sockfd);
            break;
        default:
            LOG_ERROR("connect error in Connector::startInLoop. Errno: %d", savedErrno);
            sockets::close(sockfd);
            break;
    }
}

void Connector::connecting(int sockfd) {
    setState(kConnecting);
    assert(!channel_);
    channel_.reset(new Channel(loop_, sockfd));
    channel_->setWriteCallback(std::bind(&Connector::handleWrite, this));
    channel_->setErrorCallback(std::bind(&Connector::handleError, this));

    channel_->enableWriting();
}

void Connector::handleWrite() {
    if(state_ == kConnecting) {
        int sockfd = removeAndResetChannel();
        int err = sockets::getSocketError(sockfd);
        if(err) {
            LOG_DEBUG("Connector::handleWrite - SO_ERROR = %d", err);
            retry(sockfd);
        }
        else if(sockets::isSelfConnect(sockfd)) {
            LOG_DEBUG("COnnector::handleWrite - self connect");
            retry(sockfd);
        }
        else {
            setState(kConnected);
            if(connect_) {
                newConnectionCallback_(sockfd);
            }
            else{
                sockets::close(sockfd);
            }
        }
    }
    else {
        assert(state_ == kDisconnected);
    }
}

void Connector::handleError() {
    LOG_ERROR("Connector::handle error stat=%d", state_);
    if(state_ == kConnecting) {
        int sockfd = removeAndResetChannel();
        int err = sockets::getSocketError(sockfd);
        retry(sockfd);
    }
}

int Connector::removeAndResetChannel() {
    channel_->disableAll();
    channel_->remove();
    int sockfd = channel_->fd();
    // Can't reset channel_ here, because we are inside Channel::handleEvent
    loop_->queueInLoop(std::bind(&Connector::resetChannel, this)); // FIXME: unsafe
    return sockfd;
}

void Connector::resetChannel() {
    channel_.reset();
}

void Connector::retry(int sockfd) {
    sockets::close(sockfd);
    setState(kDisconnected);
    if (connect_){
        LOG_INFO ( "Connector::retry");
        loop_->runAfter(retryDelayMs_/1000.0, std::bind(&Connector::startInLoop, shared_from_this()));
        retryDelayMs_ = std::min(retryDelayMs_ * 2, kMaxRetryDelayMs);
    }
    else{
        LOG_DEBUG("do not connect");
    }
}

void Connector::stop() {
    connect_ = false;
    loop_->queueInLoop(std::bind(&Connector::stopInLoop, this));
}

void Connector::stopInLoop() {
    loop_->assertInLoopThread();
    if (state_ == kConnecting) {
        setState(kDisconnected);
        int sockfd = removeAndResetChannel();
        retry(sockfd);
    }
}

void Connector::restart() {
    loop_->assertInLoopThread();
    setState(kDisconnected);
    retryDelayMs_ = kInitRetryDelayMs;
    connect_ = true;
    startInLoop();
}




















