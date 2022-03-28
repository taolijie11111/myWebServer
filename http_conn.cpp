#include "http_conn.h"

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <fcntl.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <errno.h>
#include "locker.h"
#include <sys/uio.h>

//设置文件描述符
int setnonblocking(int fd){
    int old_flag =fcntl(fd,F_GETFL);
    int new_flag = old_flag | O_NONBLOCK;
    fcntl(fd,F_SETFL,new_flag);
}

void addfd(int epollfd,int fd,bool one_shot){
    epoll_event event;
    event.data.fd =fd;
    event.events = EPOLLIN| EPOLLRDHUP ;//这里他写的是水平触发模式,添加边缘触发模式应该使用EPOLLET

    if(one_shot){
        event.events | EPOLLONESHOT;
    }
    epoll_ctl(epollfd,EPOLL_CTL_ADD,fd,&event);

    //设置文件描述符非阻塞
    setnonblocking(fd);
}



void removefd(int epollfd,int fd){
    epoll_ctl(epollfd,EPOLL_CTL_DEL,fd,0);  
}

//修改文件描述符
void modfd(int epollfd,int fd,int ev){
    epoll_event event;
    event.data.fd = fd;
    event.events =ev | EPOLLONESHOT;
    epoll_ctl(epollfd,EPOLL_CTL_MOD,fd,&event);
}

void http_conn::init(int sockfd , const sockaddr_in& addr){

}

void http_conn::init(int sockfd,const sockaddr_in& addr){
    m_sockfd =sockfd;
    m_address = addr;

    int reuse =1;
    setsockopt(sockfd,SOL_SOCKET,SO_REUSEADDR,&reuse,sizeof(reuse));

    addfd(m_epollfd,m_sockfd,true);
    m_user_count++;
}

void http_conn::close_conn(){
    if(m_sockfd !=-1){
        removefd(m_epollfd,m_sockfd);
        m_sockfd = -1;
        m_user_count--;
    }
}

bool http_conn::read(){
    return true;
}
bool http_conn::write(){
    return true;
}

//处理http请求的入口函数
void http_conn::process(){

    //解析http请求
    //生成响应
}