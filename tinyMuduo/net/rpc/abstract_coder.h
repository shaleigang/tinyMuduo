//
// Created by slg on 7/25/23.
//

#ifndef TINYMUDUO_ABSTRACT_CODER_H
#define TINYMUDUO_ABSTRACT_CODER_H

#include <vector>

#include <tinyMuduo/net/Buffer.h>
#include <tinyMuduo/net/rpc/abstract_protocol.h>

namespace tmuduo {
namespace net {

class AbstractCoder {
public:
    virtual void encode(std::vector<AbstractProtocol::s_ptr>& messages, Buffer* buf) = 0;
    virtual void decode(std::vector<AbstractProtocol::s_ptr>& out_messages, Buffer* buf) = 0;

    virtual ~AbstractCoder() {}
};


}
}

#endif //TINYMUDUO_ABSTRACT_CODER_H
