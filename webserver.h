#pragma once

#include <sys/socket.h>
#include <netinet/in.h>

class webserver{
public:
    webserver();
    ~webserver();

    void init();

    void thread_pool();
    void sql_pool();
    void log_write();
    
    //预计完成的几大模块
    //日志系统
    //数据库
    //http
    //锁
    
};