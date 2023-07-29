//
// Created by sha on 6/11/23.
//

#include <tinyMuduo/base/CurrentThread.h>

#include <sys/syscall.h>
#include <unistd.h>

__thread int tmuduo::CurrentThread::t_cachedTid = 0;
void tmuduo::CurrentThread::cacheTid() {
    if(tmuduo::CurrentThread::t_cachedTid == 0){
        tmuduo::CurrentThread::t_cachedTid = static_cast<pid_t>(::syscall(SYS_gettid));
    }
}