//
// Created by slg on 7/25/23.
//

#include <vector>
#include <cstring>
#include <arpa/inet.h>

#include <tinyMuduo/base/Logger.h>
#include <tinyMuduo/net/rpc/MyPB_coder.h>

using namespace tmuduo::net;

void MyPBCoder::encode(std::vector<AbstractProtocol::s_ptr> &messages, tmuduo::net::Buffer *out_buf) {
    for(auto &i : messages) {
        std::shared_ptr<MyPBProtocol> msg = std::dynamic_pointer_cast<MyPBProtocol>(i);
        int len = 0;
        const char* buf = encodeMyPB(msg, len);
        if (buf != nullptr && len != 0) {
            out_buf->append(buf, len);
        }
        if (buf) {
            free((void*)buf);
            buf = nullptr;
        }
    }
}

void MyPBCoder::decode(std::vector<AbstractProtocol::s_ptr> &out_messages, tmuduo::net::Buffer *buf) {
    while(1) {
        int32_t pk_len = buf->peekInt32();
        if(pk_len > buf->readableBytes()) {
            break;
        }
        std::shared_ptr<MyPBProtocol> message = std::make_shared<MyPBProtocol>();

        message->m_pk_len = pk_len;
        buf->retrieveInt32();

        message->m_msg_id_len = buf->peekInt32();
        buf->retrieveInt32();
        message->m_msg_id = buf->retrieveAsString(message->m_msg_id_len);

        message->m_method_name_len = buf->peekInt32();
        buf->retrieveInt32();
        message->m_method_name = buf->retrieveAsString(message->m_method_name_len);

        message->m_err_code = buf->peekInt32();
        buf->retrieveInt32();

        message->m_err_info_len = buf->peekInt32();
        buf->retrieveInt32();
        message->m_err_info = buf->retrieveAsString(message->m_err_info_len);

        int32_t pb_data_len = message->m_pk_len - message->m_method_name_len - message->m_msg_id_len - message->m_err_info_len - 24;
        message->m_pb_data = buf->retrieveAsString(pb_data_len);

        message->m_check_sum = buf->peekInt32();
        buf->retrieveInt32();
        message->parse_success = true;

        out_messages.push_back(message);
    }
}

const char* MyPBCoder::encodeMyPB(std::shared_ptr<MyPBProtocol> message, int &len) {
    if (message->m_msg_id.empty()) {
        message->m_msg_id = "123456789";
    }
    LOG_DEBUG("msg_id = %s", message->m_msg_id.c_str());
    int pk_len = 24 + message->m_msg_id.length() + message->m_method_name.length() + message->m_err_info.length() +
                 message->m_pb_data.length();
    LOG_DEBUG("pk_len = %", pk_len);

    char *buf = reinterpret_cast<char *>(malloc(pk_len));
    char *tmp = buf;

//    *tmp = MyPBProtocol::PB_START;
//    ++tmp;

    int32_t pk_len_net = htonl(pk_len);
    memcpy(tmp, &pk_len_net, sizeof(pk_len_net));
    tmp += sizeof(pk_len_net);

    int msg_id_len = message->m_msg_id.length();
    int32_t msg_id_len_net = htonl(msg_id_len);
    memcpy(tmp, &msg_id_len_net, sizeof(msg_id_len_net));
    tmp += sizeof(msg_id_len_net);

    if (!message->m_msg_id.empty()) {
        memcpy(tmp, &(message->m_msg_id[0]), msg_id_len);
        tmp += msg_id_len;
    }
    int method_name_len = message->m_method_name.length();
    int32_t method_name_len_net = htonl(method_name_len);
    memcpy(tmp, &method_name_len_net, sizeof(method_name_len_net));
    tmp += sizeof(method_name_len_net);

    if (!message->m_method_name.empty()) {
        memcpy(tmp, &(message->m_method_name[0]), method_name_len);
        tmp += method_name_len;
    }

    int32_t err_code_net = htonl(message->m_err_code);
    memcpy(tmp, &err_code_net, sizeof(err_code_net));
    tmp += sizeof(err_code_net);

    int err_info_len = message->m_err_info.length();
    int32_t err_info_len_net = htonl(err_info_len);
    memcpy(tmp, &err_info_len_net, sizeof(err_info_len_net));
    tmp += sizeof(err_info_len_net);

    if (!message->m_err_info.empty()) {
        memcpy(tmp, &(message->m_err_info[0]), err_info_len);
        tmp += err_info_len;
    }

    if (!message->m_pb_data.empty()) {
        memcpy(tmp, &(message->m_pb_data[0]), message->m_pb_data.length());
        tmp += message->m_pb_data.length();
    }

    int32_t check_sum_net = htonl(1);
    memcpy(tmp, &check_sum_net, sizeof(check_sum_net));
//    tmp += sizeof(check_sum_net);
//
//    *tmp = MyPBProtocol::PB_END;

    message->m_pk_len = pk_len;
    message->m_msg_id_len = msg_id_len;
    message->m_method_name_len = method_name_len;
    message->m_err_info_len = err_info_len;
    message->parse_success = true;
    len = pk_len;

    LOG_DEBUG("encode message[%s] success", message->m_msg_id.c_str());

    return buf;
}