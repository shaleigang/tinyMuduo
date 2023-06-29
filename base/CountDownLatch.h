//
// Created by sha on 6/11/23.
// 使用std::mutex std::condition_variable 和 std::unique_lock 替代原本实现
// 一个CountDownLatch countDown N次之后，通知所有阻塞在wait()的线程解除阻塞
//

#ifndef TINYMUDUO_COUNTDOWNLATCH_H
#define TINYMUDUO_COUNTDOWNLATCH_H

#include "noncopyable.h"
#include "types.h"

#include <mutex>
#include <condition_variable>

namespace tmuduo{

class CountDownLatch : noncopyable {
public:
    explicit CountDownLatch(int count);

    void wait();
    void countDown();
    int getCount() const;

private:
    mutable mutex mutex_;
    condition_variable cond_;
    int count_;
};

} // namespace tmuduo

#endif //TINYMUDUO_COUNTDOWNLATCH_H
