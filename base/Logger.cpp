//
// Created by slg on 6/25/23.
//

#include "Logger.h"
#include "Timestamp.h"

#include <cstdarg>
#include <iostream>

using namespace tmuduo;

Logger& Logger::instance() {
    static Logger logger;
    return logger;
}

void Logger::log(int level, const char* logMsgFormat, ...) {
    std::unique_lock<mutex> lock(mutex_);
    va_list args;
    va_start(args, logMsgFormat);
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