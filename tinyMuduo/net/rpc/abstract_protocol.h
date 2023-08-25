//
// Created by slg on 7/25/23.
//

#ifndef TINYMUDUO_ABSTRACT_PROTOCOL_H
#define TINYMUDUO_ABSTRACT_PROTOCOL_H

#include <memory>
#include <string>

namespace tmuduo {
namespace net {

class AbstractProtocol : public std::enable_shared_from_this<AbstractProtocol> {
public:
    typedef std::shared_ptr<AbstractProtocol> s_ptr;

    virtual ~AbstractProtocol() {}
public:
    std::string m_msg_id;
};

}
}

#endif //TINYMUDUO_ABSTRACT_PROTOCOL_H
