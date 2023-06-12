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

void print()
{
    printf("tid=%d\n", tmuduo::CurrentThread::tid());
}

void printString(const std::string& str)
{
    std::cout << str;
    usleep(100*1000);
}

void test(int maxSize)
{
    std::cout << tmuduo::Timestamp::now().toString() << " Test ThreadPool with max queue size = " << maxSize << std::endl;
    tmuduo::ThreadPool pool("MainThreadPool");
    pool.setMaxQueueSize(maxSize);
    pool.start(5);

    std::cout << "Adding... " << std::endl;
    for (int i = 0; i < 100; ++i)
    {
        char buf[32];
        snprintf(buf, sizeof buf, "task %d", i);
        pool.run(std::bind(printString, std::string(buf)));
    }

    tmuduo::CountDownLatch latch(1);
    pool.run(std::bind(&tmuduo::CountDownLatch::countDown, &latch)); // latch的countDown()放入队列最后
    latch.wait();  // 等待latch的countDown结束，由于是最后一个任务，保证了队列中所有任务执行结束。
    pool.stop();
    std::cout<< std::endl << tmuduo::Timestamp::now().toString() << " All tasks Done" << std::endl;
}

int main()
{
    test(0);   // 队列无上限
    test(1);   // 队列上限1（多个任务添加需要等待队列空出）
    test(5);	// 队列上限5（多个任务添加需要等待队列空出）
    test(10);	// 队列上限10（多个任务添加需要等待队列空出）
    test(50);	// 队列上限50（多个任务添加需要等待队列空出）
}