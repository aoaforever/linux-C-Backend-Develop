```cpp
int poll(struct pollfd *fds, nfds_t nfds, int timeout);

struct pollfd{
  int     fd;       //file descriptor
  short   events;   //requested events
  short   revents;  //returned events
};
```
如果没有任何事件发生，poll将会被阻塞。
|参数|说明|
|  -  |-    |
|fds|存放多个fd的数组|
|nfds|管理的fd的数量|
|timeout|超时，以毫秒为单位，表示poll阻塞多少毫秒。传入-1表示会超时，一直阻塞。传入0，poll立刻返回，不阻塞|
|fd|文件描述符，如果设置为-1，那么对应的事件events将会被忽略，revents将会返回0|
|events|一般传入POLLIN,表示想要读文件|
|revents|fd有事件，将在这里返回值，并不会修改fd和events|

|传给events的值|说明|
|-            | -  |
|POLLIN|      读数据|
|POLLOUT|     写数据|
|POLLPRI||
|POLLRDHUP||
|POLLERR||
|POLLNVAL||

* `poll`本质上和`select`没有差别，都是监听多个文件描述符，管理多个描述符也是进行轮询，根据描述符的状态进行处理。  
* 但是poll没有最大文件描述符数量的限制。  
* select采用fdset采用bitmap，而poll采用了数组。  
* poll和selece同样存在一个缺点就是，文件描述符的数组被整体复制于用户态和内核态的地址空间之间，而不论这些文件描述符是否有事件，它的开销随着文件描述符数量的增加而线性增大。  
* 还有poll返回后，也需要遍历整个描述符数组才能得到有事件的描述符。

