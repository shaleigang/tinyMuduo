//
// Created by sha on 6/10/23.
// Timestamp仅保留核心功能
// 1. 两种构造函数
// 2. now静态函数成员
// 3. 装换为字符串
//


#ifndef TINYMUDUO_TIMESTAMP_H
#define TINYMUDUO_TIMESTAMP_H

#include "copyable.h"
#include "types.h"
#include <string>
#include <cstdint>

namespace tmuduo{

class Timestamp : copyable{
public:
    Timestamp() : microSecondsSinceEpoch_(0) {}
    explicit Timestamp(int64_t ms) : microSecondsSinceEpoch_(ms) {}

    static Timestamp now();
    string toString() const;

private:
    static const int kMicroSecondsPerSecond = 1000 * 1000;
    int64_t microSecondsSinceEpoch_;
};

} //namespace tmuduo

#endif //TINYMUDUO_TIMESTAMP_H
