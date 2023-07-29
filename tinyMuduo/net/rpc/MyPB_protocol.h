//
// Created by slg on 7/25/23.
//

#ifndef TINYMUDUO_MY_PROTOCOL_H
#define TINYMUDUO_MY_PROTOCOL_H

#include <tinyMuduo/net/rpc/abstract_protocol.h>

namespace tmuduo {
namespace net {

class MyPBProtocol : public AbstractProtocol {
public:
    MyPBProtocol() {}
    ~MyPBProtocol() {}

//    static char PB_START;
//    static char PB_END;

    int32_t m_pk_len {0};
    int32_t m_msg_id_len {0};

    int32_t m_method_name_len {0};
    std::string m_method_name;
    int32_t m_err_code {0};
    int32_t m_err_info_len {0};
    std::string m_err_info;

    std::string m_pb_data;
    int32_t m_check_sum {0};

    bool parse_success {false};
};

}
}


#endif //TINYMUDUO_MY_PROTOCOL_H
