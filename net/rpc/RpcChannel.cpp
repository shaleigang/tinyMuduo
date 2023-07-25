//
// Created by slg on 7/8/23.
//

#include "RpcChannel.h"

using namespace tmuduo;
using namespace tmuduo::net;

RpcChannel::RpcChannel(const tmuduo::net::TcpConnectionPtr &conn)
  : conn_(conn),
    services_(nullptr) {
    LOG_INFO("RpcChannel ctor.");
}

RpcChannel::~RpcChannel() {
    LOG_INFO("RpcChannel dtor.");
}

void RpcChannel::onMessage(const tmuduo::net::TcpConnectionPtr &conn, tmuduo::net::Buffer *buf,
                           tmuduo::Timestamp receiveTime) {
    coder_.decode(messages_, buf);
    for(auto &m : messages_) {
        std::shared_ptr<MyPBProtocol> message = std::dynamic_pointer_cast<MyPBProtocol>(m);
        const auto it = services_->find(message->m_method_name);
        if(it == services_->end()) {
            LOG_INFO("can not find service %s.", message->m_method_name.c_str());
            continue;
        }
        else {
            google::protobuf::Service* service = it->second;
            const google::protobuf::ServiceDescriptor* desc = service->GetDescriptor();
            size_t i = message->m_method_name.find_first_of('.');
            string service_name = message->m_method_name.substr(0, i);
            string method_name = message->m_method_name.substr(i + 1, message->m_method_name.length() - i - 1);
            const google::protobuf::MethodDescriptor* method = desc->FindMethodByName(method_name);
            if(method) {
                google::protobuf::Message* request(service->GetRequestPrototype(method).New());
                if (request->ParseFromString(message->m_pb_data)) {
                    google::protobuf::Message* response = service->GetResponsePrototype(method).New();
                    service->CallMethod(method, nullptr, request, response, nullptr);
                    doneCallback(response, message->m_msg_id);
                }
            }
        }
    }
}

void RpcChannel::doneCallback(::google::protobuf::Message *response, std::string &id) {
    std::unique_ptr<google::protobuf::Message> d(response);
    std::shared_ptr<MyPBProtocol> message = std::make_shared<MyPBProtocol>();
    message->m_msg_id = id;
    message->m_pb_data = (response->SerializeAsString());
    std::vector<AbstractProtocol::s_ptr> messages;
    Buffer buf;
    messages.push_back(message);
    coder_.encode(messages, &buf);
    conn_->send(&buf);
}

void RpcChannel::CallMethod(const google::protobuf::MethodDescriptor *method,
                            google::protobuf::RpcController *controller, const google::protobuf::Message *request,
                            google::protobuf::Message *response, google::protobuf::Closure *done) {
    std::shared_ptr<MyPBProtocol> message = std::make_shared<MyPBProtocol>();
    message->m_msg_id = "123456789";
    message->m_method_name = method->full_name();
    message->m_pb_data = request->SerializeAsString();
    std::vector<AbstractProtocol::s_ptr> messages;
    Buffer buf;
    messages.push_back(message);
    coder_.encode(messages, &buf);
    conn_->send(&buf);
}