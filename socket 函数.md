函数说明
==

```cpp
int socket(int domain, int type, int protocol);
```
### 参数：  
`domain`:选择协议族用以通信。这些协议族定义在了 <sys/socket.h>中。一般传入以下值：
| Name | Purpose | 
| ----|--------|
|AF_INET|IPv4 Internet protocols|
|AF_INET6|IPv6 Internet protocols|

`type`:指定socket的通信语义。一般传入以下值：
|Name|Description|
|:-|-:|
|SOCK_STREAM|提供序列、可靠、双工、基与连接的字节流。如TCP|
|SOCK_DGRAM|数据报，无连接，一个固定最大长度的不可靠消息|

`protocol`:指定socket使用特定的协议。正常情况下，在给定的协议族中，只有一个协议支持特定的套接字类型，在这种情况下protocol值设为0。

### SOCK_STREAM套接口（流套接口）的性质  
1. 不保留任何消息的边界  
举一个例子：本地主机通过两次独立的`write()`调用向远程主机发送数据，第一次本地进程写入25字节的数据，并通过套接口发送到远程进程，第二次再写入30字节的数据发往远程进程，总共55字节的数据，而远程进程从套接口接收数据时，将消息作为一个完整的单元来接收，或是通过若干次独立的读操作来将数据取走，即接受端并不知道这55字节的数据是分25字节和30字节两次来发送的。  
2. 有序性  
可以保证接受的数据字节与发送是顺序完全一致（意味着通信之前必须建立一个连接）
3. 无错性  
可以保证接受的数据在接收端被无错的接受。如果有错误发生，在尝试完所有的错误恢复措施后仍无法消除错误，流套接口就会报告错误。所进行的错误恢复措施尝试是完全自动的，不需编程者的指导。

### SOCK_STRAM常用的通信函数
流套接字在发送或者接受数据的时候处于`connectd`状态。  
连接对方端的时候可以调用`connect()`系统调用。  
连接之后，数据就可以通过`read()`或`write()`系统调用来传输数据。函数的另一种变体也可以调用`send()`或`recv()`系统调用来传输数据。 
当一个会话已经完成时，可以执行`close()`关闭连接。  
Out-of-band data（[带外数据、加速数据](https://blog.csdn.net/yejing_utopia/article/details/45154159 "解释")）也可以使用`send()`和`recv()`来发送接受紧急数据。 

---
```cpp
int bind(int sockfd, const struct sockaddr *addr, socklen_t addrlen);
```
### 参数：  
`sockfd`:套接字的fd  
`*addr`:用于绑定ip地址和端口等信息  
`addrlen`:该sockaddr结构体的长度

其中sockaddr结构体如下所示：
```cpp
struct sockaddr{  
  sa_family_t sa_family;//地址族，2字节
  char        sa_data[14];//14字节，包含套接字中的目标地址和端口信息
};
```  
sockaddr的缺陷是: `sa_data`把目标地址和端口信息混在了一起。  
因此，通常会用下面这个结构体来代替：  
```cpp
struct sockaddr_in{
    sa_family_t     sin_family;   //地址镞（Address Family） 2字节
    uint16_t        sin_port;     //16位TCP/UDP端口号  2字节
    struct in_addr  sin_addr;     //32位IP地址         4字节
    char            sin_zero[8];  //不使用             8字节
};

struct in_addr{
    In_addr_t s_addr;             //32位IP地址
};

```
`sin_port`和`sin_addr`都必须是网络字节序（NBO），一般可视化的数字都是主机字节序（HBO）。  


 ```cpp
 int inet_aton( const char* string, struct in_addr* addr );
 
 ```
 ### 参数：  
 `string`:IP地址字符串  
 `addr`:转换后的值存放在这里.一般传入sockaddr_in结构体里的sin_addr成员。  
`inet_aton()`能够将一个字符串IP地址转换为一个32位的网络序列IP地址。  
成功返回非零值，如果输入地址不正确则会返回零。使用这个函数并没有`错误码`存放在`errno`中，所以他的值会被忽略。  

---  
```cpp
int accept( int sockfd, struct sockaddr *addr, socklen_t *addrlen);
```  
accept()系统调用与基于连接的套接字类型(SOCK_STREAM, SOCK_SEQPACKET)一起使用。   
它从listen套接字的sockfd提取挂在连接队列上的第一个连接请求，从而创建一个新的connected套接字，并返回一个新的文件描述符套接字。  
新创建的套接字未处于监听状态。原始套接字sockfd不受此调用的影响。  
新的fd用于`send()`和`recv()`。  

### 参数   
`sockfd`:传入由服务器调用`socket()`创建的fd，这个fd也用于`bind()`和`listen()`。  
`addr`:存放客户端地址。  
`addrlen`:注意是个指针，先初始化为addr指向的结构体的长度，在函数调用结束后被设置为实际地址信息的长度。  

---  
```cpp
size_t recv(int sockfd, void*buf, size_t len, int flags);
ssize_t read(int fd, void* buf, size_t len);
```  
`recv()`和`read()`之间的唯一区别是`recv()`有`flags`参数。 如果`flags`参数为0, `recv()`通常等同于`read()`。    
`recv()`系统调用用于从套接字接收消息。它们可以用于在无连接和面向连接的套接字上接收数据。  
返回消息的长度。如果消息太长而无法放入所提供的缓冲区，则根据接收消息的套接字类型，可能会丢弃多余的字节。  
如果套接字上没有可用的消息，`recv()`将阻塞等待消息到达，除非套接字是非阻塞的，在这种情况下返回值`-1`，并将外部变量`errno`设置为`EAGAIN`或`EWOULDBLOCK`。  



