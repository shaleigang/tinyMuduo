//
// Created by slg on 6/25/23.
//

#ifndef TINYMUDUO_LOGGER_H
#define TINYMUDUO_LOGGER_H

#include "noncopyable.h"
#include "types.h"
#include <iostream>

namespace tmuduo {

class Logger : noncopyable {
public:
    enum LogLevel {
        INFO,
        ERROR,
        FATAL,
        DEBUG,
    };
    static Logger& instance();

    void log(int level, const char* logMsgFormat, ...);

private:
    char buf[1024] = {0};
    mutex mutex_;
};

#define LOG_INFO(logMsgFormat, ...) Logger::instance().log(Logger::INFO, logMsgFormat, ##__VA_ARGS__)
#define LOG_ERROR(logMsgFormat, ...) Logger::instance().log(Logger::ERROR, logMsgFormat, ##__VA_ARGS__)
#define LOG_FATAL(logMsgFormat, ...) Logger::instance().log(Logger::FATAL, logMsgFormat, ##__VA_ARGS__)
#define LOG_DEBUG(logMsgFormat, ...) Logger::instance().log(Logger::DEBUG, logMsgFormat, ##__VA_ARGS__)


} // namespace tmuduo

#endif //TINYMUDUO_LOGGER_H
