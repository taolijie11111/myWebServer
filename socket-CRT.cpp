//CRT
/*
technological process:
step 1:socket()
step 2:connect()
step 3:data 
step 4:close
*/

//std
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <string.h>
using namespace std;

//sockaddr_in htons
#include <netinet/in.h>

//assert
#include <assert.h>

//socket()
//int socket(int domain, int type, int protocol);
//connect()
//int connect(int sockfd, const struct sockaddr *addr,socklen_t addrlen);
#include <sys/types.h>          
#include <sys/socket.h>

//data
#include <unistd.h>

//int inet_pton(int af, const char *src, void *dst);
#include <arpa/inet.h>


//



int main()
{
    //用于通信，websever中lfd用于监听，cfd用于通信
    int cfd=socket(AF_INET,SOCK_STREAM,0);
    assert(cfd>=0);

    //connect
    struct sockaddr_in serv;
    serv.sin_family=AF_INET;
    serv.sin_port=htons(8888);
    inet_pton(AF_INET,"127.0.0.1",&serv.sin_addr.s_addr);
    /*
    AF_INET
            src points to a character  string  containing  an  IPv4  network
            address  in  dotted-decimal format, "ddd.ddd.ddd.ddd", where ddd
            is a decimal number of up to three digits in the range 0 to 255.
            The  address is converted to a struct in_addr and copied to dst,
            which must be sizeof(struct in_addr) (4) bytes (32 bits) long.
    */
    cout<<"serv.sin_addr.sin_addr:"<<serv.sin_addr.s_addr<<endl;
    int ret=connect(cfd,(struct sockaddr *)&serv,sizeof(serv));
    assert(ret>=0);
    /*
    connect():
    If  the connection or binding succeeds, zero is returned. 
    On error, -1  is returned, and errno is set appropriately.
    */

    sleep(30);

    //data
    int n=0;
    char buf[256];
    while (1)
    {
        //read
        memset(buf,0x00,sizeof(buf));
        n=read(STDIN_FILENO,buf,sizeof(buf));
        //send
        write(cfd,buf,n);

        //read data from webserver
        memset(buf,0x00,sizeof(buf));
        n=read(cfd,buf,sizeof(buf));
        assert(n>0);
        cout<<"n:"<<n<<"\tbuf:"<<buf<<endl;
    }

    //close
    close(cfd);
    return 0;
}

       
/*
check network:
netstat -anp |grep 8888
(Not all processes could be identified, non-owned process info
 will not be shown, you would have to be root to see it all.)
tcp        0      0 0.0.0.0:8888            0.0.0.0:*               LISTEN      2466/./socket       
tcp        0      0 127.0.0.1:8888          127.0.0.1:51514         ESTABLISHED 2466/./socket       
tcp        0      0 127.0.0.1:51514         127.0.0.1:8888          ESTABLISHED 2544/./socket-CRT   
*/
