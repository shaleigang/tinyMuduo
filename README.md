# tinyMuduo
A tiny version of muduo without Boost. For learning purposes only.

## 1. 简介
* base
主要包含封装的Thread、ThreadPool、Logger、TimeStamp类，另外实现了BlockingQueue和CountDownLatch

* net
网络库的核心部分，主要是Socket、EventLoop、TcpConnection、Poller、Accepter、TcpServer类

## 2. base
* TimeStamp
核心功能主要是两个：
    * now() 静态函数，获取当前时间
    * toString() 将时间戳转化为string，方便打印
* Thread
对C++ 11 std::thread进一步封装，主要想实现的功能是
    * 每个Thread有自己的name
    * 静态变量记录当前已创建Thread数量，ThreadPool中会用到
    * 实现start()功能，原生的std::thread创建即运行
* ThreadPool
线程池实现
    * 一个vector存储所有已创建Thread的指针，使用unique_ptr管理
    * 一个deque存储所有待处理任务
    * 所有Thread都执行一个while循环，不断从队列中取task，并执行
    * 使用std::mutex和std::condition_variable实现deque为有界阻塞队列
* Logger
比较简单的日志类，实现为C风格的日志记录函数
    * 单例模式，只产生一个Logger，并通过mutex实现互斥访问
    * 宏定义不同等级的日志记录函数
## 3. net
* InetAddress
用于IP地址和port的封装，屏蔽背后的实现细节，如字节序转换、sockaddr结构体细节
    * 通过string类型的IP或者int类型的port构造，方便易用
    * toIp()和toIpPort()转化为string字符串类型的IP地址， port返回端口号
* Socket、SockerOps
管理sockfd，屏蔽实现细节
    * 通过sockfd构造 
    * 析构时负责close sockfd
* Buffer
当发送消息很长时，一次write可能无法完全写入，此时需要重复多次write。通过引入Buffer这一层，用户只管向Buffer写入，至于需要向socket写入数据就不用用户过于关注了
    * 底层是一个std::vector\<char\>类型
    * Buffer前端预留部分空间，可以用于消息的头部信息，这样向消息前端添加头部信息时效率更高
    * 当Buffer空间不足时，使用double的增长策略
    * 实现了将Buffer中数据转换为字符串的操作
    * 由于Buffer从socket中读数据时，并不知道将会读到多少数据，如果Buffer容量很小，待读数据很大就会造成多次read调用，严重影响效率。所以通过一个栈上数组的配合，可以很好地解决这个问题。详见Buffer.readFd()实现。
* Channel
管理sockfd相关的操作和回调函数
    * 管理关注的sockfd事件，如读、写
    * 管理读、写、关闭、错误事件到来时的回调函数
* Poller
实现IO复用，监听sockfd的事件。当有事件发生时，调用sockfd对应的Channel的handleEvent()函数进行事件处理。handleEvent()内部将根据事件类型，调用对应的回调函数。tinyMuduo只实现了基于epoll的EPollPoller，核心操作如下：
    * updateChannel() 更新对应socket的监听事件类型
    * removeChannel() 从epoll监听中删除socket
    * poll() 返回所有监听socket中有事件发生的
* EventLoop
事件循环。
    * 每个EventLoop中都有一个Poller。另外，每个Channel也应该属于一个EventLoop，所以Channe中有一个指向EventLoop的指针，EventLoop中也有一个指向Poller的指针，所有Channel update的操作都通过EventLoop作为中介。
    * 另外EventLoop中有一个队列用于存放待处理任务。
    * EventLoop循环简单来说就是：Poller.poll()获取有事件发生的socket，然后依次调用对应的handleEvent()函数处理事件。然后处理队列中的任务。然后再次poll，如此循环。
* EventLoopThread
将一个EventLoop循环封装到一个线程中。
    * EventLoopThread中包含一个Thread对象
    * EventLoopThread在ThreadFunc中创建EventLoop，并执行
    * 也就是说EventLoopThread包含一个线程，这个线程不断执行EventLoop.loop()
* EventLoopThreadPool
    * 将包含EventLoop的线程进一步封装为一个线程池，实现方式和ThreadPool类似
    * EventLoopThreadPool有一个指向一个baseLoop，代表该线程池所属loop。
* TcpConnection
管理一个TCP连接。管理一个TCP连接的整个声明周期。比如连接建立或者关闭时的回调。
    * 构造时需要提供对应的sockfd
    * TcpConnection包含sockfd对应的Channel，由TcpConnection创建，并将自身的相关回调（比如连接中有读写事件时的回调）注册为Channel对应的回调
* Accepter
用于监听连接事件，接受新的TCP连接
* TcpServer
包含一个Accepter，一个线程池。


