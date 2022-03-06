//WebServer
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
    serv.sin_addr.s_addr=htonl(INADDR_ANY);
    int ret=bind(lfd,(struct sockaddr *)&serv,sizeof(serv));
    assert(ret>=0);

    //step 3: listen
    //int listen(int sockfd, int backlog);
    listen(lfd,128);//mix number backlog=128

    //step 4:accept
    //int accept(int sockfd,struct sockaddr *addr,socklen_t *addrlen);
    int cfd=accept(lfd,NULL,NULL);
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
    int n=0;
    char buf[1024];
    while (1)
    {
        //read data
        memset(buf,0x00,sizeof(buf));
        n=read(cfd,buf,sizeof(buf));//ssize_t read(int fd, void *buf, size_t count);
        assert(n>0);
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
test code:
nc 127.0.0.1 8888
*/