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
	emum Level{
	DEBUG=1,
		INFO=2,
		WARN=3,
		ERROR =4,
		FATAL=5
	};

Logger();

	void log(Level level,const LogEvent& event);
private:
	LogAppender::ptr
};

class LogAppender{
public:
virtual ~LogAppender(){}

}

class LogFormatter{

}

}

#endif
