//
// Created by slg on 7/8/23.
//

#ifndef TINYMUDUO_RPCCHANNEL_H
#define TINYMUDUO_RPCCHANNEL_H

#include <memory>
#include <map>
#include <google/protobuf/service.h>
#include <google/protobuf/descriptor.h>
#include <google/protobuf/message.h>
#include <vector>

#include "MyPB_protocol.h"
#include "MyPB_coder.h"
#include "../TcpConnection.h"
#include "../../base/Logger.h"

namespace tmuduo {
namespace net {

class RpcChannel : public google::protobuf::RpcChannel {
public:
//    RpcChannel();
    explicit RpcChannel(const TcpConnectionPtr& conn);
    ~RpcChannel() override;

    void setConnection(const TcpConnectionPtr& conn) {
        conn_ = conn;
    }

    void setServices(const std::map<string, google::protobuf::Service*>* services) {
        services_ = services;
    }

    void CallMethod(const google::protobuf::MethodDescriptor* method,
                    google::protobuf::RpcController* controller, const google::protobuf::Message* request,
                    google::protobuf::Message* response, google::protobuf::Closure* done) override;

    void onMessage(const TcpConnectionPtr& conn,
                   Buffer* buf,
                   Timestamp receiveTime);
private:
    void doneCallback(::google::protobuf::Message* response, string &id);

    TcpConnectionPtr conn_;
    MyPBCoder coder_;
//    std::vector<std::shared_ptr<MyPBProtocol>> messages_;
    std::vector<AbstractProtocol::s_ptr> messages_;
    const std::map<string, google::protobuf::Service*>* services_;
};

typedef std::shared_ptr<RpcChannel> RpcChannelPtr;

}
}
#endif //TINYMUDUO_RPCCHANNEL_H
