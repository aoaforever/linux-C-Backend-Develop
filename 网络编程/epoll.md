## 一再提醒：程序的末尾需要关闭`epoll_create()`创建的`epfd`：`close(epfd)`.

```cpp
#include <sys/epoll.h>

int epoll_create(int size)-->int epfd;   //size参数已经被Linux 2.6.8忽略，只需要传一个>0的值就可以。
int epoll_ctl(int epfd, int op, int fd, struct epoll_event *event)-->(0,-1);

typedef unio epoll_data{
   void     *ptr;
   int      fd;
   uint32_t u32;
   uint64_t u64;
} epoll_data_t;

sturct epoll_event{
   uint32_t      events;  //Epoll events
   epoll_data_t  data;    //User data variable
};

int epoll_wait(int epfd, struct epoll_event *events, int maxevents, int timeout)

```

`epoll`有两种触发模式：水平触发（level-triggered）和边缘触发（edge-triggered）。  
epoll可以管理大量的文件描述符。  
epoll实例可以看作是一个容器，其监管两个列表：
  * 监听列表（interest list）：用于注册fds和监管这些fds。
  * 就绪列表（ready list）：表示有fd的IO就绪了。实质上是interest list的引用，某个fd的IO就绪了，内核就会动态的生成在这里。  

* epoll_create()创建一个新的epoll实例，并返回描述这个实例的fd。**不再用的时候需要用close（）关闭这个fd。**     

|返回值|说明|
|-----|---|
|>0的fd值|成功|
|-1|错误，并恰当的设置errno|


* epoll_ctl()将感兴趣的listen_fd或者客户端fd注册到interest list上，或者删除、修改。      

|op传入的值|说明|
|-------|----|
|EPOLL_CTL_ADD|将fd加到interest list并且配置相应的event|
|EPOLL_CTL_MOD|为fd配置新的event|
|EPOLL_CTL_DEL|从interest list中移除fd，event参数可以不传或者传NULL|

|epoll_event.events的值|说明|
|-------|-----|
|EPOLLIN|fd可读|
|EPOLLOUT|fd可写|
|EPOLLRDHUP||
|EPOLLPRI||
|EPOLLERR||
|EPOLLHUP||
|EPOLLET|设置边缘触发，默认是水平触发。|
|EPOLLONESHOT||
|EPOLLWAKEUP||
|EPOLLEXCLUSIVE||

|返回值|说明|
|------|----|
|0     |成功|
|-1    |错误，并且恰当的设置errno|


* epoll_wait()等待IO事件，如果没有事件则会阻塞调用这个函数的线程。这个系统调用可以看作是从ready list中获取就绪的项目。  
1.一次调用最多返回`maxevents`个事件。  
2.参数`timeout`以毫秒计时，epoll_wait()最多阻塞多少秒。`-1`表示无限阻塞，`0`表示即使没有事件也立即返回。  
3.参数：`events`是一个数组。如：struct epoll_event events[100];
4.返回的`events`值用`&`来判断返回了哪些事件。

|返回值|说明|
|------|---|
|>0|IO就绪的fd个数|
|0|超时|
|-1|错误，并且恰当的设置errno|
