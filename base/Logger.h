//
// Created by slg on 6/25/23.
// 一个简单的C风格日志实现
// 1. 单例模式，通过static，只产生一个Logger实例
// 2. 通过宏定义，实现不同的日志级别
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
    // singleton
    static Logger& instance();

    void log(int level, const char* logMsgFormat, ...);

private:
    char buf[1024] = {0};
    // 使用std::cout输出日志，mutex_保证互斥访问
    mutex mutex_;
};

// C风格日志记录，通过...实现可变参数
// 在宏中，使用##__VA_ARGS__进行可变参数传递
#define LOG_INFO(logMsgFormat, ...) Logger::instance().log(Logger::INFO, logMsgFormat, ##__VA_ARGS__)
#define LOG_ERROR(logMsgFormat, ...) Logger::instance().log(Logger::ERROR, logMsgFormat, ##__VA_ARGS__)
#define LOG_FATAL(logMsgFormat, ...) Logger::instance().log(Logger::FATAL, logMsgFormat, ##__VA_ARGS__)
#define LOG_DEBUG(logMsgFormat, ...) Logger::instance().log(Logger::DEBUG, logMsgFormat, ##__VA_ARGS__)


} // namespace tmuduo

#endif //TINYMUDUO_LOGGER_H
