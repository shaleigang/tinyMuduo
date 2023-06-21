//
// Created by sha on 6/20/23.
//

#ifndef TINYMUDUO_INETADDRESS_H
#define TINYMUDUO_INETADDRESS_H

#include "../base/copyable.h"
#include "../base/types.h"

#include <netinet/in.h>
#include <string>
#include <cstring>

namespace tmuduo {
namespace net {

class InetAddress : public tmuduo::copyable {
public:
    InetAddress() {
        ::memset(&addr_, 0, sizeof(addr_));
    };
    explicit InetAddress(uint16_t port);
    InetAddress(string ip, uint16_t port);
    explicit InetAddress(const struct sockaddr_in& addr)
            : addr_(addr)
    { }

    string toIp() const;
    string toIpPort() const;
    uint16_t port() const;

    const struct sockaddr* getSockAddr() const {
        return (const struct sockaddr*)(&addr_);
    }
    void setSockAddr(const struct sockaddr_in &addr) {
        addr_ = addr;
    }

private:
    struct sockaddr_in addr_;
};

} // namespace net
} // namespace tmuduo

#endif //TINYMUDUO_INETADDRESS_H
