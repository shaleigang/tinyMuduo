//
// Created by sha on 6/10/23.
// 1. 拷贝构造和拷贝赋值设为delete，故不可拷贝
// 2. 构造和析构声明为protected，故不可实例化
//

#ifndef TINYMUDUO_NONCOPYABLE_H
#define TINYMUDUO_NONCOPYABLE_H

namespace tmuduo{

class noncopyable{
public:
    noncopyable(const noncopyable &) = delete;
    noncopyable& operator=(const noncopyable &) = delete;

protected:
    noncopyable() = default;
    ~noncopyable() = default;
};

} //namespace tmuduo

#endif //TINYMUDUO_NONCOPYABLE_H
