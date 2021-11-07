<span id="顶部"></span>
```cpp
#include <sys/select.h> 
int select(int nfds, fd_set *readfds, fd_set *writefds, fd_set *exceptfds, struct timeval *timeout)-->(<0,0,>0);

#include <sys/time.h>
struct timeval{
    long tv_sec;     //seconds
    long tv_usec;    //microseconds
};

strcut timespec {
    long tv_sec;     //seconds
    long tv_nsec;    //nanoseconds
};

```
`select()`和`pselect()`允许程序监管多个文件描述符，等待直到一个或多个文件描述符对于某些I/O操作“就绪”(例如，可能的输入)。如果能够执行相应的IO操作，就表示fd是就绪的状态。    
`select()`只能监视小于`FD_SETSIZE`的文件描述符个数。`poll（）`没有这个限制。     

`nfds`应传入最大的fd值+1。
fd_set中应包含服务器的listen_fd，和未来将要连接的client_fd（后添加）。
三个独立的文件描述符集被监视。 
  * 在`readfds`中列出的文件描述符将被监视，以查看字符是否可以被读取(更准确地说，是查看读取是否不会阻塞; 特别是，文件描述符也在文件结束处准备好了)。   
  * `writefds`中的文件描述符将被监视，以查看是否有可用空间用于写(尽管较大的写仍然可能阻塞)。   
  * 监视`exceptds`中的文件描述符，以查看异常情况。   

在退出时，每个文件描述符集都被适当地修改，以指示哪些文件描述符实际更改了状态。 (因此，如果在循环中使用select()，则必须在每次调用之前重新初始化集合。)    
**每次调用socket（）都会修改一次fd_set，因此需要备份fd_set.socket（）返回值就是修改后的fd_set里面被置为1的个数**  
成功时，select()和pselect()返回 三个描述符集中包含的 文件描述符的数量(即readfds、writefds、exceptds中设置的位的总数)。  
如果没有任何事件发生，超时了，则为零。  
当出现错误时，返回-1，并设置errno来指示错误;  
文件描述符集未被修改，超时变为未定义。  

**调用select（）后，用for循环从0遍历到max_fd，在循环里面调用FD_ISSET()判断哪些fd被置为了1，就能够知道哪个fd有事件。**

|返回值| 说明|
|-----|-----|
|<0   |错误|
|=0   |超时|
|>0   |事件个数|

---
有四个宏可以用来修改fd_sets。
```CPP
void FD_ZERO(fd_set *set);
void FD_SET(int fd, fd_set *set);
void FD_CLR(int fd, fd_set *set);
int FD_ISSET(int fd, fd_set *set)-->(0,!=0);
```
|函数|说明|
|---|----|
|FD_ZERO()|清除这个fd_set,全部位置0|
|FD_SET()|对fd_set的特定的fd位 置1|
|FD_CLR()|对fd_set的特定fd位 置0|
|FD_ISSET()|判断这个fd是否在fd_set里面，当select（）返回有事件的fd时，用这个函数判断。|



---
### socket水平触发模式  
select采用“水平触发“的方式，如果报告了fd后事件没有被处理，或者数据没有被全部读取，那么下次select时会再次报告该fd。不会丢失事件，也不会丢失数据。
如果数据太长，怎么保证数据的一致性？
假如数据长10字节，如果read一次只读2字节，那么读完2字节后，就会立刻报告给socket继续往下读，直到读完10字节为止。


---
### select性能测试
只测IO，不测网络通信的速度。不要用公网地址测试，用本地网络测试。   
select的性能是很好的。
当连接数比较多的时候，select的性能会下降。


---
### select的缺点
1. select支持的文件描述符数量太小了，默认是1024，虽然可以调整，但是，描述符数量越大，效率将更低，调整的意义不大。
2. 每次调用select，都需要把fdset从用户态拷贝到内核。
3. 同时在线的大量客户端有事件发生的可能很少，但还是需要遍历fdset，因此随着监视的描述符数量的增长，其效率也会线性下降。


---
select是I/O服用函数，除了用于网络通信，还可以用于文件、管道、终端、设备等操作，但开发场景比较少。


