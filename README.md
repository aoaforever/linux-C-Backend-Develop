# linux-C-Backend-Develop
Linux C++ 后台开发 相关的基础，网络编程。  


* 文件目录
  * 算法（leetcode）  
    * [经验](https://github.com/aoaforever/linux-C-Backend-Develop/blob/main/%E7%AE%97%E6%B3%95/%E7%BB%8F%E9%AA%8C.md) 
    * [c++ STL的各种模块](https://github.com/aoaforever/linux-C-Backend-Develop/blob/main/%E7%AE%97%E6%B3%95/%E9%97%AE%E9%A2%98%E8%AE%B0%E5%BD%95.md)
  * TCP
    * [TCP基础](https://github.com/aoaforever/linux-C-Backend-Develop/blob/main/TCP/TCP%E5%9F%BA%E7%A1%80.md)
  * 网络编程 
    * [sock网络编程函数说明](https://github.com/aoaforever/linux-C-Backend-Develop/blob/main/%E7%BD%91%E7%BB%9C%E7%BC%96%E7%A8%8B/socket.md)
    * [select函数说明](https://github.com/aoaforever/linux-C-Backend-Develop/blob/main/%E7%BD%91%E7%BB%9C%E7%BC%96%E7%A8%8B/select.md)
    * [poll函数说明](https://github.com/aoaforever/linux-C-Backend-Develop/blob/main/%E7%BD%91%E7%BB%9C%E7%BC%96%E7%A8%8B/poll.md)
    * [epoll函数说明](https://github.com/aoaforever/linux-C-Backend-Develop/blob/main/%E7%BD%91%E7%BB%9C%E7%BC%96%E7%A8%8B/epoll.md)
    * [read_write的使用](https://github.com/aoaforever/linux-C-Backend-Develop/blob/main/%E7%BD%91%E7%BB%9C%E7%BC%96%E7%A8%8B/read_wirte%E7%9A%84%E4%BD%BF%E7%94%A8.md)
    * [信号的使用](https://github.com/aoaforever/linux-C-Backend-Develop/blob/main/%E7%BD%91%E7%BB%9C%E7%BC%96%E7%A8%8B/%E4%BF%A1%E5%8F%B7%E7%9A%84%E4%BD%BF%E7%94%A8.md)
    * [定时器](https://github.com/aoaforever/linux-C-Backend-Develop/blob/main/%E7%BD%91%E7%BB%9C%E7%BC%96%E7%A8%8B/%E5%AE%9A%E6%97%B6%E5%99%A8.md)
    * [提高服务器性能的其他建议](https://github.com/aoaforever/linux-C-Backend-Develop/blob/main/%E7%BD%91%E7%BB%9C%E7%BC%96%E7%A8%8B/%E6%8F%90%E9%AB%98%E6%9C%8D%E5%8A%A1%E5%99%A8%E6%80%A7%E8%83%BD%E7%9A%84%E5%85%B6%E4%BB%96%E5%BB%BA%E8%AE%AE.md)
    * [读写事件触发的时机](https://github.com/aoaforever/linux-C-Backend-Develop/blob/main/%E7%BD%91%E7%BB%9C%E7%BC%96%E7%A8%8B/%E8%AF%BB%E5%86%99%E4%BA%8B%E4%BB%B6%E8%A7%A6%E5%8F%91%E7%9A%84%E6%97%B6%E6%9C%BA.md)
    * [高级IO函数](https://github.com/aoaforever/linux-C-Backend-Develop/blob/main/%E7%BD%91%E7%BB%9C%E7%BC%96%E7%A8%8B/%E9%AB%98%E7%BA%A7IO%E5%87%BD%E6%95%B0.md)
  * Linux基础
    * [信号](https://github.com/aoaforever/linux-C-Backend-Develop/blob/main/linux%E5%9F%BA%E7%A1%80/%E8%BF%9B%E7%A8%8B%E9%80%9A%E4%BF%A1/%E4%BF%A1%E5%8F%B7.md)
    * [cmake的使用](https://github.com/aoaforever/linux-C-Backend-Develop/blob/main/linux%E5%9F%BA%E7%A1%80/cmake%E7%9A%84%E4%BD%BF%E7%94%A8.md) 
    * [gdb调试](https://github.com/aoaforever/linux-C-Backend-Develop/blob/main/linux%E5%9F%BA%E7%A1%80/gdb%E8%B0%83%E8%AF%95.md)
    * [大端小端与网络字节序](https://github.com/aoaforever/linux-C-Backend-Develop/blob/main/linux%E5%9F%BA%E7%A1%80/%E5%A4%A7%E7%AB%AF%E5%B0%8F%E7%AB%AF%E4%B8%8E%E7%BD%91%E7%BB%9C%E5%AD%97%E8%8A%82%E5%BA%8F.md)
    * [常用命令](https://github.com/aoaforever/linux-C-Backend-Develop/blob/main/linux%E5%9F%BA%E7%A1%80/%E5%B8%B8%E7%94%A8%E5%91%BD%E4%BB%A4.md)
    * [文件描述符](https://github.com/aoaforever/linux-C-Backend-Develop/blob/main/linux%E5%9F%BA%E7%A1%80/%E6%96%87%E4%BB%B6%E6%8F%8F%E8%BF%B0%E7%AC%A6.md)
   

问题记录：
1. 什么叫竞态条件
2. 什么叫可重入
3. 对新来的fd设置边缘触发和非阻塞能提高效率吗？
4. 使用socketpair创建管道，注册pipefd[0]上的可读事件(P204页）
```CPP
socketpair(PU_UNIX, SOCK_STREAM, 0 , pipefd);
```

