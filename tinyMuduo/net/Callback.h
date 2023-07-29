//
// Created by slg on 6/27/23.
//

#ifndef TINYMUDUO_CALLBACK_H
#define TINYMUDUO_CALLBACK_H

namespace tmuduo {
namespace net {

class TcpConnection;

using std::placeholders::_1;
using std::placeholders::_2;
using std::placeholders::_3;

typedef std::shared_ptr<TcpConnection> TcpConnectionPtr;
typedef std::function<void(const TcpConnectionPtr &)> ConnectionCallback;
typedef std::function<void(const TcpConnectionPtr &)> CloseCallback;
typedef std::function<void(const TcpConnectionPtr &)> WriteCompleteCallback;
typedef std::function<void(const TcpConnectionPtr &, size_t)> HighWaterMarkCallback;
typedef std::function<void(const TcpConnectionPtr &, Buffer *, Timestamp)> MessageCallback;

void defaultConnectionCallback(const TcpConnectionPtr& conn);
void defaultMessageCallback(const TcpConnectionPtr& conn,
                            Buffer* buffer,
                            Timestamp receiveTime);

} // namespace net
} // namespace tmuduo
#endif //TINYMUDUO_CALLBACK_H
