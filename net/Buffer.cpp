//
// Created by slg on 6/26/23.
//

#include "Buffer.h"
#include "../base/Logger.h"

#include <cstring>
#include <cassert>
#include <cerrno>
#include <sys/uio.h>

using namespace tmuduo;
using namespace tmuduo::net;

const char Buffer::kCRLF[] = "\r\n";

void Buffer::makeSpace(size_t len) {
    if(writableBytes() + prependableBytes() < len + kCheapPrepend) {
        buffer_.resize(writerIndex_ + len);
    }
    else {
        assert(kCheapPrepend < readerIndex_);
        size_t readable = readableBytes();
        std::copy(begin() + readerIndex_,
                  begin() + writerIndex_,
                  begin() + kCheapPrepend);
        readerIndex_ = kCheapPrepend;
        writerIndex_ = readerIndex_ + readable;
        assert(readable == readableBytes());
    }
}

const char *Buffer::findCRLF() const {
    const char* crlf = std::search(peek(), beginWrite(), kCRLF, kCRLF+2);
    return crlf == beginWrite() ? NULL : crlf;
}

const char *Buffer::findCRLF(const char *start) const {
    const char* crlf = std::search(start, beginWrite(), kCRLF, kCRLF+2);
    return crlf == beginWrite() ? NULL : crlf;
}

const char *Buffer::findEOL() const {
    const void* eol = memchr(peek(), '\n', readableBytes());
    return static_cast<const char*>(eol);
}

const char *Buffer::findEOL(const char *start) const {
    const void* eol = memchr(start, '\n', readableBytes());
    return static_cast<const char*>(eol);
}

void Buffer::retrieve(size_t len) {
    assert(len <= readableBytes());
    if(len < readableBytes()) {
        readerIndex_ += len;
    }
    else {
        retrieveAll();
    }
}

void Buffer::retrieveAll() {
    readerIndex_ = kCheapPrepend;
    writerIndex_ = kCheapPrepend;
}

string Buffer::retrieveAsString(size_t len) {
    assert(len <= readableBytes());
    string result(peek(), len);
    retrieve(len);
    return result;
}

string Buffer::retrieveAllAsString() {
    return retrieveAsString(readableBytes());
}

void Buffer::ensureWritableBytes(size_t len) {
    if(writableBytes() < len){
        makeSpace(len);
    }
    assert(writableBytes() >= len);
}

void Buffer::append(const char *data, size_t len) {
    ensureWritableBytes(len);
    std::copy(data, data+len, beginWrite());
    writerIndex_ += len;
}

ssize_t Buffer::readFd(int fd, int *savedErrno) {
    char extrabuf[65536];
    struct iovec vec[2];
    const size_t writable = writableBytes();
    vec[0].iov_base = begin() + writerIndex_;
    vec[0].iov_len = writable;
    vec[1].iov_base = extrabuf;
    vec[1].iov_len = sizeof extrabuf;
    const int iovcnt = (writable < sizeof extrabuf) ? 2 : 1;
    const ssize_t n = ::readv(fd, vec, iovcnt);
    if(n < 0){
        *savedErrno = errno;
    }
    else if(static_cast<size_t>(n) <= writable){
        writerIndex_ += n;
    }
    else{
        writerIndex_ = buffer_.size();
        append(extrabuf, n - writable);
    }
    return n;
}