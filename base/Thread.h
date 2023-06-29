//
// Created by sha on 6/11/23.
// 通过一个void() Func和name构造Thread实例
// 为实现thread的start()功能，使用std::shared_ptr管理指向std::thread的指针
// 初始化时不创建thread实例，start()中才创建
//

#ifndef TINYMUDUO_THREAD_H
#define TINYMUDUO_THREAD_H

#include "noncopyable.h"
#include "types.h"
#include <functional>

namespace tmuduo{

class Thread : noncopyable{
public:
    typedef std::function<void ()> ThreadFunc;

    explicit Thread(ThreadFunc &&f, const string &name = string());
    ~Thread();

    void start();
    void join();
    void detach();

    pid_t tid() const { return tid_; };
    inline const string& name() const { return name_; };
    static int numCreated()  { return numCreated_; };

private:
    void setDefaultName();

    bool started_;
    bool joined_;
    std::shared_ptr<std::thread> thread_;
    ThreadFunc func;
    pid_t tid_;
    string name_;
    // 用一个静态变量记录已创建的线程数
    static atomic_int32_t numCreated_;
};

}

#endif //TINYMUDUO_THREAD_H
