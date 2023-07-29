//
// Created by sha on 6/11/23.
// 返回当前线程tid(),并通过cache方法减少系统调用
//

#ifndef TINYMUDUO_CURRENTTHREAD_H
#define TINYMUDUO_CURRENTTHREAD_H


namespace tmuduo{
namespace CurrentThread{

// __thread保证每个线程有各自自己的t_cachedTid变量
extern __thread int t_cachedTid;
void cacheTid();
inline int tid(){
    // __builtin_expect是一种优化手段
    // __builtin_expect(EXP, N)。意思是告诉编译器：EXP==N的概率很大。
    if(__builtin_expect(t_cachedTid == 0, 0)){
        cacheTid();
    }
    return t_cachedTid;
}

} //namespace CurrentThread
} //namespace tmuduo


#endif //TINYMUDUO_CURRENTTHREAD_H
