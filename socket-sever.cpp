//WebServer
/*
technological process:
step 1:socket()
step 2:bind()--struct sockaddr_in 
step 3:listen()
step 4:accpet()
step 5:data
step 6:close() -listenfd && cupsacceptfd
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <assert.h>
//bzero headfile
#include <strings.h>
//read headfile
#include <unistd.h>


using namespace std;

int main()
{
    //step 1: creat an endpoint
    //int socket(int domain,int type,int protocol);
    //AF_INET:for IPV4
    //SOCK_STREAM: TCP 
    int lfd=socket(AF_INET,SOCK_STREAM,0);
    assert(lfd >= 0);

    //step 2: connections 
    //int bind(int sockfd,const struct sockaddr *addr,socklen_t addrlen);
    //addr:local IP and PORT
    //addelen: size
    struct sockaddr_in serv;//creat a struct
    bzero(&serv,sizeof(serv));//void bzero(void *s, size_t n);
    /*
     The  bzero()  function  erases  the  data  in the n bytes of the memory
       starting at the location pointed to by s, by writing zeroes (bytes con‐
       taining '\0') to that area.
    */
    //man 7 ip
    // struct sockaddr_in {
    //            sa_family_t    sin_family; /* address family: AF_INET */
    //            in_port_t      sin_port;   /* port in network byte order */
    //            struct in_addr sin_addr;   /* internet address */
    //        };
    serv.sin_family=AF_INET;
    serv.sin_port=htons(8888);//not use number before 1024
    /*
    POST:
    端口：
    逻辑端口：是指逻辑意义上用于区分服务的端口，通过不同的逻辑端口来区分不同的服务，如TCP/IP
    协议中的服务端口，比如用于浏览网页服务的80端口，用于FTP服务的21端口等。
    物理端口：指的是物理存在的端口，如ADSL Modem、集线器、交换机、路由器上用 于连接其他网络
    设备的接口，如RJ-45端口、SC端口等等。

    端口号：
    为了对端口进行区分，将每个端口进行了编号，这就是端口号。

    二：客户端使用的端口号：
    也叫做动态端口号，或者短暂端口号。数值为49152~65535，由于这类端口号仅在客户进程运行时才
    动态选择，程序结束，则已使用过的端口号就被回收，以便以后供其它客户进程使用。
    */
    serv.sin_addr.s_addr=htonl(INADDR_ANY);
    int ret=bind(lfd,(struct sockaddr *)&serv,sizeof(serv));
    assert(ret>=0);



    //step 3: listen
    //int listen(int sockfd, int backlog);
    listen(lfd,128);//mix number backlog=128

    //step 4:accept
    //int accept(int sockfd,struct sockaddr *addr,socklen_t *addrlen);
    struct sockaddr_in client;
    socklen_t len=sizeof(client);//len is a out and in virtual
    int cfd=accept(lfd,(struct sockaddr *)&client,&len);
    //linked
    char SIP[16];
    memset(SIP,0x00,sizeof(SIP));
    //get client IP and Post
    cout<<"client:IP:"<<inet_ntop(AF_INET,&client.sin_addr.s_addr,SIP,sizeof(SIP))<<"\tPORT:"<<ntohs(client.sin_port)<<endl;


/*
change here:
int cfd=accept(lfd,NULL,NULL);
->
int cfd=accept(lfd,)
*/


    //block
    /*
     The cupsaccept command instructs the printing system  to  accept  print
       jobs to the specified destinations.

       The  cupsreject  command  instructs the printing system to reject print
       jobs to the specified destinations.  The -r option sets the reason  for
       rejecting  print jobs. If not specified, the reason defaults to "Reason
       Unknown".
    */
    cout<<"lfd: "<<lfd<<"cfd: "<<cfd<<endl;

    //step 5: data
    int i=0;
    int n=0;
    char buf[1024];
    while (1)
    {
        //read data
        memset(buf,0x00,sizeof(buf));
        n=read(cfd,buf,sizeof(buf));//ssize_t read(int fd, void *buf, size_t count);
        assert(n>0);// it close if sever close
        cout<<"n: "<<n<<"buf: "<<buf<<endl;
        //man toupper
        // These functions convert lowercase letters to uppercase,
        for(int i=0;i<n;i++){
            buf[i]=toupper(buf[i]);
        }

        //send data
        write(cfd,buf,n);
    }

    //step 6: close
    close(lfd);
    close(cfd);
    
}

/*
test server:
nc 127.0.0.1 8888
*/