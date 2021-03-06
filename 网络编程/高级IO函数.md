## 高级IO函数
Linux提供了很多高级的I/O函数。他们并不像linux基础I/O函数（比如open和read）那么常用，但在特定的条件下却表现出优秀的性能。  
这些函数大致分为三类：  
* 用于创建文件描述符的函数，包括pipe、dup/dup2函数。
* 用于读写数据的函数，包括readv/writev、sendfile、mmap/munmap、splice和tee函数。  
* 用于控制I/O行为和属性的函数，包括fcntl函数。

### pipe函数
<span id="pipe函数"></span>
pipe函数用于创建一个管道，以实现进程间通信。
```CPP
#include <unistd.h>
int pipe( int fd[2] )-->(0,-1);
```
* 成功时，将两个打开的fd填入数组里。  
* 通过pipe函数创建的两个文件描述符fd[0]和fd[1]分别构成管道的两端，往fd[1]写入的数据可以从fd[0]读出。  
* 并且，fd[0]只能用于从管道读出数据，fd[1]则只能用于往管道写入数据。而不能反过来使用。
* 如果要实现双向的数据传输，就应该使用两个管道。
* 默认情况下，这一对文件描述符都是阻塞的。此时如果我们用 read 系统调用来读取一个空的管道，
则 read 将被阻塞，直到管道内有数据可读。如果我们用 write 系统调用来往一个满的管道中写入数据，则 write 亦将被阻塞，直到管道有足够多的空闲空间可用。
*


### dup函数和dup2函数
<span id="dup函数和dup2函数"></span>
可用于实现CGI服务器，把服务器的标准输出重定向到客户端的socket上。

### readv函数和writev函数
<span id="readv函数和writev函数"></span>
readv函数将数据从文件描述符读到分散的内存块中，即分散读。  
writev函数则将多块分散的内存数据一并写入文件描述符中，即集中写。 
```CPP
#include <sys/uio.h>
ssize_t readv( int fd, const struct iovec* vector, int count)-->(ssize_t,-1);
ssize_t writev( int fd, const struct iovec* vector, int count)-->(ssize_t,-1);

struct iovec{

};
```
当Web服务器解析完一个HTTP请求后，如果目标文档存在且客户具有读取该文档的权限，那么他就需要发送一个HTTP应答来传输该文档。其中，前3部分的内容可能被Web服务器放置在一块内存中，而文档的内容则通常被读入到另一块单独的内存中（通过read函数或mmap函数）。我们并不需要把这两部分内容拼接到一起再发送，而是可以使用writev函数将他们同时写出。  

### sendfile函数与零拷贝
<span id="sendfile函数与零拷贝"></span>
sendfile函数在两个文件描述符之间直接传递数据（完全在内核中操作），从而避免了内核缓冲区和用户缓冲区之间的数据拷贝，效率很高，**这被称为零拷贝**。
```CPP
#include <sys/sendfile.h>
ssize_t sendfile(int out_fd, int in_fd, off_t* offset, size_t count)-->(ssize_t, -1);
```
* 该函数的 man 手册明确指出， in_fd 必须是一个支持类似 mmap 函数的文件描述符，即它必须指向真实的文件，不能是 socket 和管道；  
* 而 out_fd 则必须是一个 socket。由此可见，sendfile 几乎是专门为在网络上传输文件而设计的。  

### splice函数与零拷贝
<span id="splice函数与零拷贝"></span>
splice 函数用于在两个文件描述符之间移动数据,也是零拷贝操作。
```CPP
#include<fcntl.h>
ssize_t splice(int fd_in, loff_t* off_in, int fd_out, loff_t* off_out, size_t len, unsigned int flags);
```

* fd＿in 参数是待输入数据的文件描述符。如果 fd_in 是一个管道文件描述符，那么 off_in
参数必须被设置为 NULL。  
* 如果 fd_in 不是一个管道文件描述符〈比如 socket），那么 off_in表示从输入数据流的何处开始读取数据．此时，若 off_in 被设置为 NULL， 则袋示从输入数据流的当前偏移位置读入；若 off_in 不为 NULL，则它将指出具体的偏移位置。
* fd_out/off_out 参数的含义与 fd＿in/off_in相同，不过用于输出数据流。
* len 参数指定移动数据的长度 
* flags 参数则控制数据如何移动

|falgs|含义|
|-----|----|
|SPLICE_F_MOVE|如果合适的话，按整页内存移动数据，这只是给内核的一个提示。不过有bug，实际上没有任何效果|
|SPLICE_F_MORE|给内核的一个提示，后续的splice调用将读取更多数据|

* 使用 splice 函数时，**fd_in 和 fd_out 必须至少有一个是管道文件描述符**。  
* splice 函数调用成功时返回移动字节的数量。它可能返回 0，表示没有数据宿要移动，这发生在从管道中读取数据 （fd＿in 是管道文件描述符〉而该管道没有被写入任何数据时。  
* splice 函数失败时返回－1 并设置 errno。
* 可用于实现回射服务。


### tee函数与零拷贝
<span id="tee函数与零拷贝"></span>
* **tee 函数在两个管道文件描述符之间复制数据，也是零拷贝操作 。** 它不消耗数据，因此
源文件描述符上的数据仍然可以用于后续的读操作。  
* 与splice函数的区别是tee的两个fd都必须是管道fd。  

```CPP
#include <fcntl.h>
ssize_t tee(int fd_in, int fd_out, size_t len, unsigned int flags)-->(ssize_t,0,-1);
```
* tee函数与splice函数可用于实现 同时输出数据到终端和文件的tee程序（tee程序不是tee函数）  


### mmap函数和munmap函数与进程间通信的共享内存
<span id="mmap函数和munmap函数与进程间通信的共享内存"></span>
* mmap 函数用于申请一段内存空间。我们可以将这段内存作为进程间通信的共享内存，也可以将文件直接映射到其中。  
* munmap函数则释放由mmap创建的这段内存空间。  
```CPP
#include<sys/mman.h>
void* mmap(void* start, size_t length, int prot, int flags, int fd, off_t offset);
int munmap(void* start, size_t length);
```
prot参数用来设置内存段的访问权限。  
|prot参数|说明|
|--------|----|
|PROT_READ|内存段可读|
|PROT_WRITE|内存段可写|
|PROT_EXEC|内存段可执行|
|PROT_NONE|内存段不能被访问|

flags参数控制内存段内容被修改后程序的行为。MAP_SHARED和MAP_PRIVATE是互斥的。  
|flags参数|含义|
|--------|----|
|MAP_SHARED|在进程间共享这段内存，对该内存段的修改将反映到被映射的文件中。它提供了进程间共享内存的POSIX方法|
|MAP_PRIVATE|内存段为调用进程所私有。对该内存段的修改不会反映到被映射的文件中|
|MAP_ANONYMOUS|这段内存不是从文件映射而来的。其中内容被初始化为全0。在这种情况下，mmap函数的最后两个参数将被忽略|
|MAP_FIXED|内存段必须位于start参数指定的地址处，start必须是内存页面大小（4096字节）的整数倍|
|MAP_HUGETLB|按照“大内存页面”来分配内存空间，“大内存页面”的大小可以通过/proc/meminfo文件来查看|

`fd`参数是被映射文件对应的文件描述符。它一般通过 open 系统调用获得。  
`offset` 参数设置从文件的何处开始映射，对于不需要读人整个文件的情况。  


### fcntl函数






