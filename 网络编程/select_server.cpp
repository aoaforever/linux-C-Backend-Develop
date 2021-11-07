#include <iostream>
#include <sys/select.h>
#include <sys/socket.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <string.h>
using namespace std;
#define MAX_BYTES 1024

int create_socket()
{
	int fd;
	fd = socket(PF_INET,SOCK_STREAM,0);
	return fd;
}

int create_server_socket( const char* const  argv[])
{
	int fd;
	struct sockaddr_in addr;
	memset(&addr,0, sizeof(addr));//string.h
	addr.sin_family = AF_INET;
	addr.sin_port = htons(atoi(argv[1]));
	addr.sin_addr.s_addr = htonl(INADDR_ANY);
	
	fd = socket(PF_INET,SOCK_STREAM,0);
	
	if((bind(fd, (struct sockaddr*)&addr, sizeof(addr) ))!=0)
	{
		perror("bind error\n");
		exit(-1);
	}

	if( (listen(fd, 5))!=0 )
	{
		perror("listen error\n");
		exit(-1);
	}
	return fd;
}	



int main(int argc , char * argv[])
{
	if (argc != 2 )
	{
		cout<<"Usage: ./select_server <port>"<<endl;
		return  0;
	}
	//int select(int nfds, fd_set *readfds, fd_set *writefds, fd_set *execeptfds, struct timeval *timeout );
	int  listen_fd, max_fd;
	
	listen_fd = create_server_socket(argv);
	cout<<"listen_fd is: "<<listen_fd<<endl;

	fd_set readfds;
	FD_ZERO(&readfds);
	FD_SET(listen_fd,&readfds);
	max_fd = listen_fd;
	
	
	while(true){
		fd_set tmpsets = readfds;
		int retval; 
		if((  retval=select(max_fd+1,&tmpsets,NULL,NULL,NULL)) <0)
		{
		 perror("select error\n");
		 exit(-1);
		}
		if( retval ==0)
		{
		 cout<<"out of time \n";
		 continue;
		}
		if (retval>0)
		{
		 for(int fd=0; fd<max_fd+1;fd++)
		 {
		  if(FD_ISSET(fd,&tmpsets) <=0) continue;//这里是需要半段tmpsets而不是readfds。
		   if(fd == listen_fd)
		   {
		    cout<<"listen_fd："<<fd<<" 有事件"<<endl;
	   	    int clinet_fd;
		    cout<<"开是添加新的连接"<<endl;
		    //accept()
	 	    struct sockaddr_in client;
		    socklen_t  addrlen=sizeof(client);
		    clinet_fd = accept(listen_fd,(struct sockaddr*)&client, &addrlen);
		    if(clinet_fd<0)
		    {
		     perror("clinet_fd error\n");
		     continue;//这里不是停止程序，而是进入下一次的select。
		    }
		    cout<<"新的客户端fd："<<clinet_fd<<endl<<endl;
		    FD_SET(clinet_fd,&readfds);
		    if(clinet_fd>max_fd)
		    {
		     max_fd = clinet_fd;
		    }
		    continue;
		   }
		   else 
		   {
		    cout<<"客户端fd: "<<fd<<" 有事件"<<endl;
		    char buf[MAX_BYTES];
		    memset(buf,0,sizeof(buf));
		    int isize;
		    isize = read(fd,buf,sizeof(buf));//应该要这么写，不然isize不会更新。
		    if(isize <= 0)
		    {
		     cout<<"read error\n";
		     cout<<"closing fd: "<<fd<<endl;
		     close(fd);
		     cout<<"closed fd: "<<fd<<endl<<endl;

		     //你之前忘记清楚fds了
		     FD_CLR(fd,&readfds);
		     if(fd==max_fd)
		     {
		      for(int ii = max_fd;ii>0;ii--)
		      {
			if(FD_ISSET(ii,&readfds))
			{
			 max_fd=ii;
			 break;
			}
		      }
		      cout<<"max_fd = "<<max_fd<<endl<<endl;
		     }
		     continue;
		     }
		     printf("recv(fd=%d,size=%d):%s\n",fd,isize,buf);
		     write(fd,buf,strlen(buf));
		
		     //read()

		     }
		
	       }
	      }	
	     }
	return 0;
}

