//
// Created by sha on 6/10/23.
//
#include "base/ThreadPool.h"
#include "base/CountDownLatch.h"
#include "base/CurrentThread.h"
#include "base/Timestamp.h"

#include <cstdio>
#include <iostream>
#include <unistd.h>  // usleep

#include <muduo/base/ThreadPool.h>
#include <muduo/base/CountDownLatch.h>
#include <muduo/base/CurrentThread.h>
#include <muduo/base/Logging.h>

#include <future>

class Mytask
{
    int delaySec_;
    std::promise<std::string> prom;
public:
    explicit Mytask(int delaySec) : delaySec_(delaySec) {}
    void run()
    {
        printf("tid: %d, Delay %d run...\n", muduo::CurrentThread::tid(), delaySec_);
        usleep(delaySec_ * 1000 * 1000);
        prom.set_value(std::string("Dealy ") + std::to_string(delaySec_));
    }

    auto getResult() { return prom.get_future(); }
};

int main()
{
    muduo::ThreadPool pool("threadpool test");
    pool.start(10); // 保证后续任务都加入到队列中

    std::vector<std::shared_ptr<Mytask>> tasks;
    // 任务入队
    for(int i = 0; i < 5; i++){
        tasks.emplace_back( new Mytask(rand() % 5) ); // 随机0~5秒
        pool.run([=]{
            tasks[i]->run();
        });
    }
    // 异步等待结果
    for(int i = 0; i < 5; i++){
        pool.run([=]{
            printf("tid: %d, Task %s done!\n",
                   muduo::CurrentThread::tid(), tasks[i]->getResult().get().c_str());
        });
    }

    LOG_WARN << "Done";

    {  // 若去掉此处代码，并且任务数量小于队列容量，可能会导致队列中任务未全部执行完毕就关闭线程池退出程序。
        muduo::CountDownLatch latch(1);
        pool.run(std::bind(&muduo::CountDownLatch::countDown, &latch));
        latch.wait();
    }
    pool.stop();

    return 0;
}
