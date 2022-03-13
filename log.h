#ifndef __SYLAR_LOG_H__
#define __SYLAR_LOG_H__

//#once

#include<string>
//int32
#include<stdint.h>

namespace mysebsever{

class LogEvent{
    public:
    LogEvent();
    private:
    const char* m_file=nullptr;
    int32_t m_line=0;
    uint32_t m_threadID=0;
    uint32_t m_fiberID=0;
    uint64_t m_time;
    std::string m_content;
}

class Logger{
public:
Logger();
private:
}

class LogAppender{
public:
virtual ~LogAppender(){}

}

class LogFormatter{

}

}

#endif