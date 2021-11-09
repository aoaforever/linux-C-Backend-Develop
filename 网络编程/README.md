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
socket 地址： socket最开始的含义是一个IP地址和端口对（ip,port）。它唯一地表示了使用TCP通信的一端。称为：socket地址  
socket基础API：socket的主要API都定义在<sys/socket.h>头文件中。
网络信息API：linux提供了一套网络信息API，以实现主机名和IP地址之间的转换，以及服务名称和端口号之间的转换。
