//
// Created by sha on 6/10/23.
// Timestamp仅保留核心功能
// 1. 两种构造函数
// 2. now静态函数成员，返回当前时间
// 3. toString()转换为字符串格式
//


#ifndef TINYMUDUO_TIMESTAMP_H
#define TINYMUDUO_TIMESTAMP_H

#include <tinyMuduo/base/copyable.h>
#include <tinyMuduo/base/types.h>

#include <string>
#include <cstdint>

namespace tmuduo{

class Timestamp : copyable{
public:
    Timestamp() : microSecondsSinceEpoch_(0) {}
    explicit Timestamp(int64_t ms) : microSecondsSinceEpoch_(ms) {}

    int64_t microSecondsSinceEpoch() const { return microSecondsSinceEpoch_; }

    string toString() const;

    static Timestamp now();
    static Timestamp invalid() {
        return Timestamp();
    }
    bool valid() const { return microSecondsSinceEpoch_ > 0; }

    static const int kMicroSecondsPerSecond = 1000 * 1000;

private:
    int64_t microSecondsSinceEpoch_;
};

inline bool operator<(Timestamp lhs, Timestamp rhs) {
    return lhs.microSecondsSinceEpoch() < rhs.microSecondsSinceEpoch();
}

inline bool operator==(Timestamp lhs, Timestamp rhs) {
    return lhs.microSecondsSinceEpoch() == rhs.microSecondsSinceEpoch();
}

inline double timeDifference(Timestamp high, Timestamp low) {
    int64_t diff = high.microSecondsSinceEpoch() - low.microSecondsSinceEpoch();
    return static_cast<double>(diff) / Timestamp::kMicroSecondsPerSecond;
}

inline Timestamp addTime(Timestamp timestamp, double seconds) {
    int64_t delta = static_cast<int64_t>(seconds * Timestamp::kMicroSecondsPerSecond);
    return Timestamp(timestamp.microSecondsSinceEpoch() + delta);
}

} //namespace tmuduo

#endif //TINYMUDUO_TIMESTAMP_H
