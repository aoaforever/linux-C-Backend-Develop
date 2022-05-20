## 注意，以下的内容是Linux的函数，C语言对应fopen()、fwrite()、fread()、fclose()，C++对应ifstream对象、ofstream对象。

### write()
```cpp
#include <unistd.h>

ssize_t write(int fd, const void* buf, size_t count);
```
#### 细节：
1. 向fd写入最多count个字节。也就是说实际写入的字节可以小于count。  例如：物理媒介空间不足、或者达到了RLIMIT_FSIZE资源限制、又或者此次调用被信号中断，都会少于count字节。
2. 写入成功时，返回成功字节数。错误返回-1，并设置errno。
3. 在Linux，write最多能够写入 0x7ffff000 (2,147,479,552) 字节，大约2GB，不管时32位还是64位系统。
4. **如果我定义一个数组： `char buf[2000]="asdwdf";` , 那么当调用`wirte(fd,buf,sizeof(buf));`的时候，将会写入多少字节？**  
5. 从4的例子可以看出，**write()的count参数不是乱写的，要尽量是buf内容的长度，也就是strlen(buf)，而不是sizeof(buf)**。这样子write写的长度确定了，对方的read接收到的字节数也是确定的。
虽然对方的read的count设置的是sizeof(buf)，但是实际上接收到的是strlen(buf)字节。    
写入了2000字节。。。后面全是'\0'。
```CPP
// #include <sys/epoll.h>
#include <unistd.h>
#include <iostream>
// #include <vector>
#include <string.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/fcntl.h>

using namespace std;

int main(int argc, char const *argv[])
{
    char buf[2000]="asdaff";
    cout<<"sizeof(buf): "<<sizeof(buf)<<" strlen(buf): "<<strlen(buf)<<endl;;
    // FILE* fp = fopen("","w");
    int fd = open("./file_recv/test.txt",O_WRONLY|O_CREAT);
    if(fd==-1){
        perror("FILE OPEN ERROR\n");
        return 0;
    }
    ssize_t len = write(fd,buf,sizeof(buf));
    cout<<"success write :" <<len<<" bytes"<<endl;

    close(fd);
    return 0;
}
```




### read()
```cpp
#include <unistd.h>
ssize_t read(int fd, void *buf, size_t count);
```

1. 假设我有一文件，里面只有asdf四个字符。如果我设置1024字节的buf来读取这个文件，那么会都多少字节？
答：4字节。
```CPP
#include <iostream>
#include <string.h>
using namespace std;

//linux
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/fcntl.h>
#include <unistd.h>

int main(){
    int fd = open("./test.txt",O_RDWR);
    char buf[1024];
    memset(buf,0,sizeof(buf));
    size_t len = read(fd,buf,sizeof(buf));
    cout<<"len:"<<len<<" "<<buf<<endl;
    return 0;
}
```
