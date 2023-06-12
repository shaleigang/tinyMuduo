//
// Created by sha on 6/10/23.
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
