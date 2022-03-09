# chenleng

```C++
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
```

result:
```shell
serv.sin_addr.sin_addr:16777343
socket-CRT: socket-CRT.cpp:52: int main(): Assertion `ret>=0' failed.
Aborted (core dumped)
```

reason:
websever don't open ,so it can't connect

## epoll
flow:
```C++
1. socket()---lfd
2. setsockopt()
3. bind()
4. listen()
5. create epoll tree ---int epfd=epoll_create();
   //lfd up to tree
   struct epoll_event ev;
   ev.everts=EPOLLIN;
   ev.data.fd=lfd;
   epoll_ctl(epfd,EPOLL_CTL_ADD,lfd,&ev);

   //waite for events
   struct epoll_event events[1024];
   while(1)
   {
       nready=epoll_wait(epfd,events,1024,-1);
       if(nready<0){
           if(error==EINTR)continue;
           break;
       }
    for(i=0;i<nready;i++)
    {
        sockfd=events[i].data.fd;
        //clients come
        if(sockfd=lfd){
            cfd=Accept(lfd,NULL.NULL);

            //up it to tree
            ev.data.fd=cfd;
            ev.events=EPOLLIN;
            epoll_ctl(epfd,EPOLL_CTL_ADD,cfd,&ev);
            continue;
        }
        //clients data come
        n=Read(sockfd,buf,sizeof(buf));
        if(n<=0)
        {
            close(sockfd);
            //out to tree
            epoll_ctl(epfd,EPOLL_CTL_DEL,sockfd,NULL);
            perror("read error");
            continue;
        }
        else{
            whrite(sockfd,buf,n);
        }
    }
   }
6. close(lfd);
```
### 有关ET，LT
1.在ET模式下，如何在epoll——wait返回一次情况下读完数据：
循环读完数据，但是读完数据会阻塞
2.若能够一次性读完还需要设置什么？
将通信文件描述符设置为非阻塞模式


### epoll