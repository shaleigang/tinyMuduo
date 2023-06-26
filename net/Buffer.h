//
// Created by slg on 6/26/23.
//

#ifndef TINYMUDUO_BUFFER_H
#define TINYMUDUO_BUFFER_H

#include "../base/copyable.h"
#include "../base/types.h"

#include <algorithm>
#include <vector>

namespace tmuduo {
namespace net {

class Buffer : public tmuduo::copyable {
public:
    static const size_t kCheapPrepend = 8;
    static const size_t kInitSize = 1024;

    explicit Buffer(size_t initSize = kInitSize)
      : buffer_(kCheapPrepend + initSize),
        readerIndex_(kCheapPrepend),
        writerIndex_(kCheapPrepend) {

    }

    void swap(Buffer& rhs) {
        buffer_.swap(rhs.buffer_);
        std::swap(readerIndex_, rhs.readerIndex_);
        std::swap(writerIndex_, rhs.writerIndex_);
    }

    size_t readableBytes() const { return writerIndex_ - readerIndex_; }
    size_t writableBytes() const { return buffer_.size() - writerIndex_; }
    size_t prependableBytes() const { return readerIndex_; }

    char* beginWrite() { return begin() + writerIndex_; }
    const char* beginWrite() const { return begin() + writerIndex_; }

    const char* peek() const { return begin() + readerIndex_; }
    const char* findCRLF() const;
    const char* findCRLF(const char* start) const;
    const char* findEOL() const;
    const char* findEOL(const char* start) const;
    void retrieve(size_t len);
    void retrieveAll();
    string retrieveAsString(size_t len);
    string retrieveAllAsString();
    void ensureWritableBytes(size_t len);
    void append(const char* data, size_t len);

    size_t readFd(int fd, int* savedErrno);

private:
    char* begin() { return &*buffer_.begin(); }
    const char* begin() const { return &*buffer_.begin(); }
    void makeSpace(size_t len);

    std::vector<char> buffer_;
    size_t readerIndex_;
    size_t writerIndex_;

    static const char kCRLF[];
};

} // namespace net
} // namespace tmuduo
#endif //TINYMUDUO_BUFFER_H
