//
// Created by slg on 7/8/23.
//

#ifndef TINYMUDUO_RPCCODEC_H
#define TINYMUDUO_RPCCODEC_H

#include <cstdint>
#include <functional>
#include <google/protobuf/service.h>

#include "../TcpConnection.h"

namespace tmuduo {
namespace net {

typedef std::shared_ptr<google::protobuf::Message> MessagePtr;

class RpcCodec {
public:
    const static int kHeaderLen = sizeof(int32_t);
    const static int kChecksumLen = sizeof(int32_t);
    const static int kMaxMessageLen = 64*1024*1024;

    enum ErrorCode {
        kNoError = 0,
        kInvalidLength,
        kCheckSumError,
        kInvalidNameLen,
        kUnknownMessageType,
        kParseError,
    };

    typedef std::function<void (const TcpConnectionPtr&,
                                const MessagePtr&,
                                Timestamp)> ProtobufMessageCallback;
    typedef std::function<void (const TcpConnectionPtr&,
                                Buffer*,
                                Timestamp,
                                ErrorCode)> ErrorCallback;

    void onMessage(const TcpConnectionPtr&,
                      Buffer* buf,
                      Timestamp receiveTime);


private:
    ProtobufMessageCallback messageCallback_;
    ErrorCallback errorCallback_;
};

}
}


#endif //TINYMUDUO_RPCCODEC_H
