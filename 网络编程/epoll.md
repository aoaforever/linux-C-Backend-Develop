## 一再提醒：程序的末尾需要关闭`epoll_create()`创建的`epfd`：`close(epfd)`.
epoll的好处：
1. 不用像select那样管理readfds位表，不用像poll那样管理struct pollfd fds[]数组;不需要像select、poll那样更改max_fd。  
2. 通过修改linux的某些参数，epoll可以支持高并发。  
3. epoll把用户关心的文件描述符上的事件放在内核里的一个事件表中，从而无须像select和poll那样每次调用都要重复传入文件描述符集或事件集。
4. 但epoll需要使用一个额外的文件描述符，来唯一标识内核中的这个时间表，使用epoll_create函数来创建。

```cpp
#include <sys/epoll.h>

int epoll_create(int size)-->int epfd;   //size参数已经被Linux 2.6.8忽略，只需要传一个>0的值就可以。
int epoll_ctl(int epfd, int op, int fd, struct epoll_event *event)-->(0,-1);//用来操作epoll的内核事件表

typedef unio epoll_data{
   void     *ptr;
   int      fd;
   uint32_t u32;
   uint64_t u64;
} epoll_data_t;

sturct epoll_event{
   uint32_t      events;  //Epoll events
   epoll_data_t  data;    //User data variable，用于存储用户数据
};

int epoll_wait(int epfd, struct epoll_event *events, int maxevents, int timeout)

```
* `epoll_data_t`是一个联合体，其4个成员中使用最多的是fd，它指定事件来自哪个文件描述符。  
* ptr成员可用来指定与fd相关的用户数据。但由于epoll_data_t是一个联合体，我们不能同时使用其ptr成员和fd成员，因此，如果要将fd和用户数据关联起来（正如8.5.2小节讨论的将句柄和事件处理器绑定一样），以实现快速的数据访问，只能使用其他手段，比如放弃使用fd成员，而在ptr指向的用户数据中包含fd。  
&emsp;  

* `epoll_wait`函数如果检测到事件，就将所有就绪的事件从内核事件表（由epfd参数指定）中复制到它的第二个参数events指向的数组中。这个数组只用于输出epoll_wait检测到的就绪事件。而不像select和poll的数组参数那样既用于传入用户注册的事件，又用于输出内核检测到的就绪事件。这就极大地提高了应用程序索引就绪文件描述符的效率。  
&emsp;  


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
