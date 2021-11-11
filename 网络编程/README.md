## README目录
* [网络编程的头文件](#关于头文件)
* [socket是什么？socket基础API，网络信息API](#socket地址)
* [读取设置sokcet_fd属性](#读取设置sokcet_fd属性)
---
### 关于头文件
```cpp
#include <sys/sokcet.h>
```
包含：socket(), bind(), accept(), recv(), send()  

```cpp
#include <unistd.h>
```
包含：read(), write(), close()

```cpp
#include <string.h>
```
[包含](https://www.runoob.com/cprogramming/c-standard-library-string-h.html)：memset(), memcpy(), strlen()

```cpp
#include <arpa/inet.h>
```
包含：htonl(), htons(), ntohl(), ntohs(), inet_addr(), inet_pton(), inet_aton(), inet_ntoa()

---
<span id="socket地址"></span>
* socket 地址： socket最开始的含义是一个IP地址和端口对（ip,port）。它唯一地表示了使用TCP通信的一端。称为：socket地址  
* socket基础API：socket的主要API都定义在<sys/socket.h>头文件中。
* 网络信息API：linux提供了一套网络信息API，以实现主机名和IP地址之间的转换，以及服务名称和端口号之间的转换。

---
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

 * 此外，我们也可以通过修改内核参数/proc/sys/ncνipv4/tcp_tw_recycle 来快速回收被关闭的socket，从而使得TCP 连接根本就不进入TIME_WAIT状态，进而允许应用程序立即重用本地的socket地址。
 * 此外，我们可以直接修改内核参数/proc/sys/net/ipv4/tcp_rmem 和/proc/sys/net/ipv4/tcp_wmem 来强制TCP 接收缓冲区和发送缓冲区的大小没有最小值限制。  
 * 默认情况下， TCP接收缓冲区的低水位标记和TCP 发送缓冲区的低水位标记均为1字节．
