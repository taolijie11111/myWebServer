#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include<unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/epoll.h>
#include <iostream>
#include "locker.h"
#include "threadpool.h"
#include <signal.h>
#include "http_conn.h"

#define MAX_FD 65535 
#define MAX_EVENT_NUMBER 10000 



void addsig(int sig,void(handler)(int)){
    struct sigaction sa;
    memset(&sa,'\0',sizeof(sa));//清空数据
    sa.sa_handler =handler;
    sigfillset(&sa.sa_mask);//临时信号集都是要阻塞的
    sigaction(sig,&sa,NULL);

}

//添加文件描述符大批epoll中
extern void addfd(int epollfd,int fd,bool one_shot);
//从epoll中删除文件描述符
extern void removefd(int epollfd,int fd);

//修改文件描述符
extern void modfd(int epollfd,int fd,int ev);

using namespace std;
int main(int argc,char* argv[]){//指定端口号需要这个来传
    if(argc <= 1){//需要传递端口号
    cout <<"portnumber errno"<<endl;
    exit(-1);
    }

    //获取端口好
    int port = atoi(argv[1]);
    
    //对SIGPIE信号进行处理
    addsig(SIGPIPE,SIG_IGN);//写捕捉信号的函数

    //创建线程池初始化信息
    threadpool<http_conn>* pool = NULL;
    try{
        pool = new threadpool<http_conn>;
    }catch(...){
        exit(-1);
    }

    //创建一个数组用于保存所有的客户端信息
    http_conn* users=new http_conn[MAX_FD];

    int listenfd = socket(PF_INET,SOCK_STREAM,0);
    
    //
    int reuse=1;
    setsockopt(listenfd,SOL_SOCKET,SO_REUSEADDR,&reuse,sizeof(reuse));
    
    //绑定
    struct sockaddr_in address;
    address.sin_family = AF_INET;
    address.sin_addr.s_addr=INADDR_ANY;
    address.sin_port=htons(port);
    bind(listenfd,(struct sockaddr*)&address,sizeof(address));

    //监听
    listen(listenfd,5);

    //创建epool对象，事件数组，添加
    epoll_event events[MAX_EVENT_NUMBER];
    int epollfd = epoll_create(5);

    //将监听的文件描述符添加到epoll对象中
    addfd(epollfd,listenfd,false);
    http_conn::m_epollfd = epollfd;

    while (1)
    {
        int num = epoll_wait(epollfd,events,MAX_EVENT_NUMBER,-1);
        //判断一下

        //循环便利事件族
        for(int i=0;i<num;i++){
            int sockfd = events[i].data.fd;
            if(sockfd == listenfd){
                //有客户段连接
                struct sockaddr_in client_address;
                socklen_t client_addrlen = sizeof(client_address);
                int connfd= accept(listenfd,(struct sockaddr*)&client_address,&client_addrlen);

                if(http_conn::m_user_count >=MAX_FD){
                    //目前的连接数满了
                    //服务器正忙
                    close(connfd);
                    continue;
                }
                //将新的客户的数据初始化，放到数组中
                users[connfd].init(connfd,client_address);
            }else if (events[i].events & (EPOLLRDHUP|EPOLLHUP|EPOLLERR))
            {
                users[sockfd].close_conn();
            }else if (events[i].events & EPOLLIN)
            {
                if(users[sockfd].read()){
                    //一次性把所有数据读完
                    pool->append(users+sockfd);
                }else
                {
                    users[sockfd].close_conn();
                }
            }else if(events[i].events & EPOLLOUT)
            {
                if(!users[sockfd].write()){
                    users[sockfd].close_conn();
                }
            }
            
            
            
        }
    }
    
    close(epollfd);
    close(listenfd);
    delete [] users;
    delete pool;
    return 0;
}