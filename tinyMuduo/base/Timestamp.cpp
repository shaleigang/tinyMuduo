//
// Created by sha on 6/10/23.
//

#include <tinyMuduo/base/Timestamp.h>

#include <sys/time.h>
#include <ctime>
#include <cstdio>

using namespace tmuduo;

Timestamp Timestamp::now() {
    struct timeval tv;
    gettimeofday(&tv, nullptr);
    int64_t seconds = tv.tv_sec;
    return Timestamp(seconds * kMicroSecondsPerSecond + tv.tv_usec);
}

std::string Timestamp::toString() const {
    struct tm result;
    time_t seconds = static_cast<time_t>(microSecondsSinceEpoch_ / kMicroSecondsPerSecond);
    localtime_r(&seconds, &result);
    char buf[128] = {0};
    snprintf(buf, sizeof(buf), "%4d-%02d-%02d %02d:%02d:%02d",
             result.tm_year + 1900, result.tm_mon + 1, result.tm_mday,
             result.tm_hour, result.tm_min, result.tm_sec);
    return buf;
}