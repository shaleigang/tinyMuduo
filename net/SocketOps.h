//
// Created by slg on 6/26/23.
//

#ifndef TINYMUDUO_SOCKETOPS_H
#define TINYMUDUO_SOCKETOPS_H

#include <arpa/inet.h>

namespace tmuduo {
namespace net {
namespace sockets {

int createNonblockingOrDie();
int  connect(int sockfd, const struct sockaddr* addr);
void bindOrDie(int sockfd, const struct sockaddr* addr);
void listenOrDie(int sockfd);
int  accept(int sockfd, struct sockaddr_in* addr);
ssize_t read(int sockfd, void *buf, size_t count);
ssize_t readv(int sockfd, const struct iovec *iov, int iovcnt);
ssize_t write(int sockfd, const void *buf, size_t count);
void close(int sockfd);
void shutdownWrite(int sockfd);

} // namespace sockets
} // namespace net
} // namespace tmuduo
#endif //TINYMUDUO_SOCKETOPS_H
