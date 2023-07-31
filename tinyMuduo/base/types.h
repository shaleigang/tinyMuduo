//
// Created by sha on 6/11/23.
// 使用using声明几个常用类型，方便使用
//

#ifndef TINYMUDUO_TYPES_H
#define TINYMUDUO_TYPES_H

#include <string>
#include <atomic>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <functional>

namespace tmuduo{

using std::string;
using std::atomic_int32_t;
using std::atomic_int64_t;
using std::thread;
using std::mutex;
using std::condition_variable;

}

#endif //TINYMUDUO_TYPES_H
