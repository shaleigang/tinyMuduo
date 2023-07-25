//
// Created by slg on 7/25/23.
//

#ifndef TINYMUDUO_MYPB_CODER_H
#define TINYMUDUO_MYPB_CODER_H

#include "abstract_coder.h"
#include "MyPB_protocol.h"

namespace tmuduo {
namespace net {

class MyPBCoder : public AbstractCoder{
public:
    MyPBCoder() {}
    ~MyPBCoder() {}

    void encode(std::vector<AbstractProtocol::s_ptr>& messages, Buffer* out_buf) override;
    void decode(std::vector<AbstractProtocol::s_ptr>& out_messages, Buffer* buf) override;

private:
    const char* encodeMyPB(std::shared_ptr<MyPBProtocol> message, int& len);
};

}
}


#endif //TINYMUDUO_MYPB_CODER_H
