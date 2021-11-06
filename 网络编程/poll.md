```cpp
int poll(struct pollfd *fds, nfds_t nfds, int timeout);

struct pollfd{
  int     fd;       //file descriptor
  short   events;   //requested events
  short   revents;  //returned events
};
```

`poll`本质上和`select`没有差别，都是监听多个文件描述符，管理多个描述符也是进行轮询，根据描述符的状态进行处理。  
但是poll没有最大文件描述符数量的限制。  
select采用fdset采用bitmap，而poll采用了数组。  
poll和selece同样存在一个缺点就是，文件描述符的数组被整体复制于用户态和内核态的地址空间之间，而不论这些文件描述符是否有事件，它的开销随着文件描述符数量的增加而线性增大。  
还有poll返回后，
