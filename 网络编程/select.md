<span id="顶部"></span>
```cpp
#include <sys/select.h> 
int select(int nfds, fd_set *readfds, fd_set *writefds, fd_set *exceptfds, struct timeval *timeout);

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

三个独立的文件描述符集被监视。 
  * 在`readfds`中列出的文件描述符将被监视，以查看字符是否可以被读取(更准确地说，是查看读取是否不会阻塞; 特别是，文件描述符也在文件结束处准备好了)。   
  * `writefds`中的文件描述符将被监视，以查看是否有可用空间用于写(尽管较大的写仍然可能阻塞)。   
  * 监视`exceptds`中的文件描述符，以查看异常情况。   

成功时，select()和pselect()返回三个返回的描述符集中包含的文件描述符的数量(即readfds、writefds、exceptds中设置的位的总数)，如果在任何有趣的事情发生之前超时，则可能为零。  
当出现错误时，返回-1，并设置errno来指示错误;  
文件描述符集未被修改，超时变为未定义。  

|参数|说明|
|---|----|
|nfds
