//
// Created by sha on 6/11/23.
//

#ifndef TINYMUDUO_CURRENTTHREAD_H
#define TINYMUDUO_CURRENTTHREAD_H


namespace tmuduo{
namespace CurrentThread{

extern __thread int t_cachedTid;
void cacheTid();
inline int tid(){
    if(__builtin_expect(t_cachedTid == 0, 0)){
        cacheTid();
    }
    return t_cachedTid;
}

} //namespace CurrentThread
} //namespace tmuduo


#endif //TINYMUDUO_CURRENTTHREAD_H
