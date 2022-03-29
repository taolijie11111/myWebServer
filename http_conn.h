#pragma once

#include <sys/epoll.h>
#include <sys/types.h>
#include <sys/uio.h>     // readv/writev
#include <arpa/inet.h>   // sockaddr_in
#include <stdlib.h>      // atoi()
#include <errno.h>   

class http_conn{
public:

    static int m_epollfd;//所有socket事件都被注册到同一个epoll事件中
    static int m_user_count; //统计用户数量

    http_conn(){

    };
    ~http_conn();

    void process();//处理客户端请求
    void init(int sockfd,const sockaddr_in& addr);
    void close_conn();
    bool read();//非阻塞的读
    bool write(); //非阻塞的写

private:
    int m_sockfd;   //该http连接的fd
    sockaddr_in m_address;
};