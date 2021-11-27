## README目录
* [网络编程的头文件](#关于头文件)
* [socket是什么？socket基础API，网络信息API](#socket地址)
* [读取设置sokcet_fd属性](#读取设置sokcet_fd属性)
* [通过地址信息函数获取本端、远端的socket地址](#通过地址信息函数获取本端、远端的socket地址)
* [通过网络信息API获取主机、服务的完整信息](#通过网络信息API获取主机、服务的完整信息)
* [并发模式的同步、异步与IO模型中的同步、异步](#并发模式的同步、异步与IO模型中的同步、异步)
* [问题记录、经验](#问题记录、经验)
---
### 关于头文件
```cpp
#include <sys/sokcet.h>
```
包含：socket(), bind(), accept(), recv(), send(), sockaddr_in结构体, connect()

```cpp
#include <unistd.h>
```
包含：read(), write(), close(), pipe()

```cpp
#include <string.h>
```
[包含](https://www.runoob.com/cprogramming/c-standard-library-string-h.html)：memset(), memcpy(), strlen(), bzero(),

```cpp
#include <arpa/inet.h>
```
包含：htonl(), htons(), ntohl(), ntohs(), inet_addr(), inet_pton(), inet_aton(), inet_ntoa()

```CPP
#include <stdlib.h>
```
包含：atoi(), basename()

```cpp
#include <fcntl.h>
```
包含：splice(), fcntl(), F_SETFL, F_GETFL, O_NONBLOCK, 

---
### socket是什么？socket基础API，网络信息API
<span id="socket地址"></span>
* socket 地址： socket最开始的含义是一个IP地址和端口对（ip,port）。它唯一地表示了使用TCP通信的一端。称为：socket地址  
* socket基础API：socket的主要API都定义在<sys/socket.h>头文件中。
* 网络信息API：linux提供了一套网络信息API，以实现主机名和IP地址之间的转换，以及服务名称和端口号之间的转换。

---
### 读取设置sokcet_fd属性
<span id="读取设置sokcet_fd属性"></span>
下面这两个系统调用可以专门用来读取和设置socket文件描述符属性的方法：
```cpp
#include <sys/socket.h>
int getsockopt( int sockfd, int level, int option_name, void* option_value, socklen_t * restrict option_len);
int setsockopt( int sockfd, int level, int option_name, const void* option_value, socklen_t option_len);
```
常用的设置：  
|level|option name | 数据类型|说明|
|:-----:|-------------|-------|-----|
|SOL_SOCKET(通用socket选项，与协议无关)|SO_REUSEADDR|int|重用本地地址，即使sock处于TIME_WAIT状态，与之绑定的socket地址也可以立即被重用。|
|           ...                       |SO_RCVBUF|int|TCP接收缓冲区大小，系统允许的TCP接收缓冲区最小为256字节，系统会将其值加倍|
|           ...                       |SO_SNDBUF|int|TCP发送缓冲区大小，最小值为2048字节，系统会将其值加倍|
|...                                  |SO_RCVLOWAT|int|TCP接受缓存区低水位标记，一般被IO服用系统调用用来判断socket是否可读,当TCP 接收缓冲区中可读数据的总数大于其低水位标记时,IO复用系统调用将通知应用程序可以从对应的socket 上读取数据|
|...                                  |SO_SNDLOWAT|int|TCP发送缓存区低水位标记，一般被IO服用系统调用用来判断socket是否可写,当TCP发送缓冲区中的空闲空间〈可以写人数据的空间）大于其低水位标记时,IO复用系统调用将通知应用程序可以往对应的socke上写人数据|
|...                                  |SO_LINGER|linger|若有数据待发送，则延迟关闭|
|...                                  |SO_SNDTIMEO|timeval|发送数据超时|
|...                                  |SO_RCVTIMEO|timeval|接收数据超时|

 * 此外，我们也可以通过修改内核参数/proc/sys/ncνipv4/tcp_tw_recycle 来快速回收被关闭的socket，从而使得TCP 连接根本就不进入TIME_WAIT状态，进而允许应用程序立即重用本地的socket地址。
 * 此外，我们可以直接修改内核参数/proc/sys/net/ipv4/tcp_rmem 和/proc/sys/net/ipv4/tcp_wmem 来强制TCP 接收缓冲区和发送缓冲区的大小没有最小值限制。  
 * 默认情况下， TCP接收缓冲区的低水位标记和TCP 发送缓冲区的低水位标记均为1字节．

---
### 通过地址信息函数获取本端、远端的socket地址
<span id="通过地址信息函数获取本端、远端的socket地址"></span>
```cpp
#include <sys/socket.h>
int getsockname(int sockfd, struct sockaddr* address, socklen_t* address_len)-->(0,-1);//getsocknamc 获取 sockfd 对应的自己本端 socket 地址，并将其存储于address 参数指定的内存中，该socket地址的长度则存储在address_len参数指向的变量中。  
//如果实际 socket 地址的长度大于 address 所指内存区的大小，那么该 socket 地址将被截断。

int getpeername(int sockfd, struct sockaddr* address, socklen_t* address_len)-->(0,-1);//getpeemame 获取sockfd对应的远端socket地址
```


---
### 通过网络信息API获取主机、服务的完整信息
<span id="通过网络信息API获取主机、服务的完整信息"></span>
```cpp
#include <netdb.h>
struct hostent* gethostbyname( const char* name); //根据主机名称获取主机的完整信息
struct hostent* gethostbyaddr( const void* addr, size_t len, int type);//根据IP地扯获取主机的完整信息

struct hostent
{
    char*     h_name;       //主机名
    char**    h_aliases;    //主机别名列表，可能有多个
    int       h_addrtype;   //地址类型
    int       h_length;     //地址长度
    char**    h_addr_list;  //按网络字节序列出的主机IP地址列表
}；
```
gethostbyname函数通常先在本地的/etc/hosts 配置文件中查找主机，如果没有找到，再去访问DNS服务器。
```CPP
>>cat /etc/hosts
127.0.0.1	localhost
```
getservbyname 函数根据名称获取某个服务的完整信息， getservbyport 函数根据端口号获取某个服务的完整信息。  
它们实际上都是通过读取/etc/services 文件来获取服务的信息的。
```cpp
#include<netdb.h>
struct servent* getservbyname(const char* name, const char* proto);
struct servent* getservbyport(int port, const char* proto);
/*name 参数指定目标服务的名字. port 参数指定目标服务对应的端口号. proto 参数指定
服务类型，给它传递“tcp ”表示获取流服务，给他传递“udp”表示获取数据报服务，给它传递NULL 则表示获取所有类型的服务。
这两个函数返回的都是servent 结构体*/

struct servent
{
    char*     s_name;       //服务名称
    char**    s_aliases;    //服务的别名列表，可能有多个
    int       s_port;       //端口号，以网络字节序存储
    char*     s_proto;      //服务类型，通常是tcp或者udp
}

>>cat /etc/services
服务名字   端口号/(tcp或udp)   别名   #注释
http	  80/tcp		     www	# WorldWideWeb HTTP

```

**以上这四个函数都是不可重入的，即非线程安全的。如果在他们函数名的尾部加上_r，则又可以重入了。这个名命规则也适用于其他函数的可重入版本。**

---
### 并发模式的同步、异步与IO模型中的同步、异步
<span id="并发模式的同步、异步与IO模型中的同步、异步"></span>
* 在IO模型中，“同步”和“异步”区分的是内核向应用程序通知的是何种IO事件（是就绪事件还是完成事件），以及该由谁来完成IO读写（是应用程序还是内核）。  
* 在并发模式中，“同步”指的是程序完全按照代码序列的顺序执行；“异步”指的是程序的执行需要由系统事件来驱动。常见的系统事件包括：终端、信号等。

---
### 问题记录、经验
<span id="问题记录、经验"></span>
1. 非阻塞connect，可以同时发起多个连接并一起等待。errno值：EINPROGRESS，用getsockopt（）查看。
2. `#define _GNU_SOURCE 1` 是什么？
3. POLLRDHUP是什么？当socket接收到对方关闭连接时的请求之后触发，有可能是TCP连接被对方关闭，也有可能是对方关闭了写操作。如果不使用EPOLLRDHUP事件，我们也可以单纯的使用EPOLLIN事件然后根据recv函数的返回值来判断socket上收到的是有效数据还是对方关闭连接的请求。 需要定义：`#define _GNU_SOURCE 1`.  
4. 客户端启动的时候需要传连接的IP地址和服务器的端口号。。而不是自己新建一个端口号。当连接成功后，内核会自动给客户端分配一个端口号。这时如果想查看客户端的地址信息，就用getsockname()、getpeername()来查看本端和对方的地址信息。
5. epoll的边缘触发，在读取数据的时候需要用while循环保证读取完整。
6. 需不需要关闭udp的fd，关了之后，sendto还能调用吗？
7. setsockopt(sockfd,SOL_SOCKET,SO_SNDTIMEO,&timeout,len);是什么P212页，P105页说明：如果说fcntl系统调用是控制文件描述符属性的通用POSIX方法，那么下面两个系统调用则是用来专门读取和设置socket文件描述符属性的方法：`setsockopt(int sockfd, int level, int option_name, const void* option_value, socklent_t option_len)、getsockopt()`
8. timeval 的超时是以秒、微妙计时？ select系统调用的超时是timeval数据结构。
