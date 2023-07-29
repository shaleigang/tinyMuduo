//
// Created by slg on 6/26/23.
//

#ifndef TINYMUDUO_BUFFER_H
#define TINYMUDUO_BUFFER_H

#include <tinyMuduo/base/types.h>
#include <tinyMuduo/base/copyable.h>

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

    // 返回第一个可写位置的指针
    char* beginWrite() { return begin() + writerIndex_; }
    const char* beginWrite() const { return begin() + writerIndex_; }

    // 返回指向可读消息头部的指针
    const char* peek() const { return begin() + readerIndex_; }
    int32_t peekInt32() const;
    const char* findCRLF() const;
    const char* findCRLF(const char* start) const;
    const char* findEOL() const;
    const char* findEOL(const char* start) const;
    int32_t readInt32();
    // 消费Buffer中的消息
    void retrieve(size_t len);
    void retrieveAll();
    string retrieveAsString(size_t len);
    string retrieveAllAsString();
    void retrieveInt32();
    // 如果剩余容量不满足要求，尝试double容量
    void ensureWritableBytes(size_t len);
    void prepend(const void* /*restrict*/ data, size_t len);
    void prependInt32(int32_t x);
    // 向Buffer中写入数据
    void append(const char* data, size_t len);
    void append(const void* /*restrict*/ data, size_t len);
    void appendInt32(int32_t x);

    ssize_t readFd(int fd, int* savedErrno);

private:
    char* begin() { return &*buffer_.begin(); }
    const char* begin() const { return &*buffer_.begin(); }
    // 扩展vector容量，首先将所有信息移动至vector前端，看看实际剩余容量再决定下一步操作
    void makeSpace(size_t len);

    std::vector<char> buffer_;
    size_t readerIndex_;
    size_t writerIndex_;

    static const char kCRLF[];
};

} // namespace net
} // namespace tmuduo
#endif //TINYMUDUO_BUFFER_H
