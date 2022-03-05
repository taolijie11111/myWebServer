//http socket.h 
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <fcntl.h>
#include <stdlib.h>
#include <cassert>
#include <sys/epoll.h>

int listenfd = socket(PF_INET, SOCK_STREAM, 0);
assert(listenfd >= 0);

//struct linger tmp={1,0};
//SO_LINGER若有数据待发送，延迟关闭
//setsockopt(listenfd,SOL_SOCKET,SO_LINGER,&tmp,sizeof(tmp));

int ret = 0;
struct sockaddr_in address;
bzero(&address, sizeof(address));
address.sin_family = AF_INET;
address.sin_addr.s_addr = htonl(INADDR_ANY);
address.sin_port = htons(port);

int flag = 1;
setsockopt(listenfd, SOL_SOCKET, SO_REUSEADDR, &flag, sizeof(flag));
ret = bind(listenfd, (struct sockaddr *)&address, sizeof(address));
assert(ret >= 0);
ret = listen(listenfd, 5);
assert(ret >= 0);

//