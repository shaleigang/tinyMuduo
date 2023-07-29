//
// Created by slg on 6/25/23.
//

#include <tinyMuduo/base/Logger.h>
#include <tinyMuduo/base/Timestamp.h>

#include <cstdarg> // 用于获取可变参数列表，并进行可变参数的传递
#include <iostream>

using namespace tmuduo;

Logger& Logger::instance() {
    static Logger logger;
    return logger;
}

void Logger::log(int level, const char* logMsgFormat, ...) {
    std::unique_lock<mutex> lock(mutex_);
    va_list args;
    va_start(args, logMsgFormat); // args指向可变参数列中的第一个参数
    // 因为涉及到可变参数，所以用vsnprintf()
    vsnprintf(buf, sizeof(buf), logMsgFormat, args);
    va_end(args);
    string pre;
    switch(level) {
        case INFO:
            pre = "[INFO ]"; break;
        case ERROR:
            pre = "[ERROR]"; break;
        case FATAL:
            pre = "[FATAL]"; break;
        case DEBUG:
            pre = "[DEBUG]"; break;
        default:
            break;
    }
    std::cout << pre + Timestamp::now().toString() << ": " << buf << std::endl;
}