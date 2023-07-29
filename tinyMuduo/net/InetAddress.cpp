//
// Created by sha on 6/20/23.
//

#include <tinyMuduo/net/InetAddress.h>

#include <arpa/inet.h>

using namespace tmuduo::net;

InetAddress::InetAddress(uint16_t port) {
    ::memset(&addr_, 0, sizeof(addr_));
    addr_.sin_family = AF_INET;
    in_addr_t ip = INADDR_ANY;
    addr_.sin_addr.s_addr = htobe32(ip);
    addr_.sin_port = htobe16(port);
}

InetAddress::InetAddress(string ipArg, uint16_t port) {
    ::memset(&addr_, 0, sizeof(addr_));
    addr_.sin_family = AF_INET;
    in_addr_t ip = inet_addr(ipArg.c_str());
    addr_.sin_addr.s_addr = ip;
    addr_.sin_port = htobe16(port);
}

std::string InetAddress::toIp() const {
    char buf[64] = "";
    ::inet_ntop(AF_INET, &addr_.sin_addr, buf, sizeof(buf));
    return buf;
}

uint16_t InetAddress::port() const {
    return be16toh(addr_.sin_port);
}

std::string InetAddress::toIpPort() const {
    return toIp() + ":" + std::to_string(port());
}