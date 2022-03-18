#pragma once

#include <stdio.h>//FILE

#include <string>
#include "block_queue.h"

class Log
{
    public:
    static Log *get_instance(){
        static Log instance;
        return &instance;
    }
    static void*flush_log_thread(void *args)
    {
        Log::get_instance()->async_write_log();
    }
    //可选择的参数有日志文件，日志缓冲区大小，最大行署，以及最长日志条队列
    bool init(const char *file_name,int close_log,int log_buf_size=8192,int split_lines=5000000,int max_queue_size=0);

    void write_log(int level,const char *format,...);

    void flush();

    private:
    Log();
    virtual ~Log();
    void *async_write_log()
    {
        std::string single_log;
        while (m_log_queue->pop(single_log))
        {
            m_mutex.lock();
            fputs(single_log.c_str(),m_fp);
            m_mutex.unlock();
        }
        
    }
    private:
    char dir_name[128];//路径名
    char log_name[128];//log文件名
    int m_split_lines;//日志最大行数
    int m_long_buf_size;//日志缓冲区大小
    long long m_count;//日志行数记录
    int m_today;    //按天分类，记录当前时间是那一天
    FILE *m_fp;     //打开log文件指针
    char *m_buf;    
    block_queue<std::string> *m_log_queue;//string定义需要std::
    bool m_is_async;
    locker m_mutex;
    int m_close_log;//关闭日志
};