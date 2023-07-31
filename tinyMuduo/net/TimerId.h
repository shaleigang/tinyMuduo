//
// Created by slg on 7/30/23.
//

#ifndef TINYMUDUO_TIMERID_H
#define TINYMUDUO_TIMERID_H

#include <tinyMuduo/base/copyable.h>
#include <tinyMuduo/net/Timer.h>

namespace tmuduo {
namespace net {

class TimerId : public copyable {
public:
    TimerId() : timer_(nullptr), sequence_(0) {}
    TimerId(Timer* timer, int64_t seq) : timer_(timer), sequence_(seq) {}

    friend class TimerQueue;

private:
    Timer* timer_;
    int64_t sequence_;
};

}
}



#endif //TINYMUDUO_TIMERID_H
