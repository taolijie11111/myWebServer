#ifndef __SYLAR_LOG_H__
#define __SYLAR_LOG_H__
//#once

#include<string>
//int32
#include<stdint.h>

#include<memory>

namespace mysebsever{

class LogEvent{
	public:
	typedef std::shared_ptr<LogEvent> ptr;
	LogEvent();
	private:
	const char* m_file=nullptr;   //文件名
	int32_t m_line=0;					//行号
	uint32_t m_elapse=0; //程序启动到现在的豪秒数
	uint32_t m_threadID=0;//线程ID
	uint32_t m_fiberID=0; //协程ID
	uint64_t m_time;//时间节
	std::string m_content;
};    

//日志级别
class LogLevel{
	public:
		enum Level{
				DEBUG=1,
				INFO=2,
				WARN=3,
				ERROR =4,
				FATAL=5
			};
};

//日志格式器
class LogFormatter{
	public:
	typedef std::shared_ptr<LogFormatter> ptr;

	std::string format(LogEvent::ptr event);
	private:
};


//日志输出地
class LogAppender{
public:
typedef std::shared_ptr<LogAppender> ptr;
virtual ~LogAppender(){}
void log(LogLevel::Level level,LogEvent::ptr event);
private:
	LogLevel::Level m_level;//主要根据哪些日志定义日志级别

};

//日志器
class Logger{
public:
	typedef std::shared_ptr<Logger> ptr;
	Logger(const std::string& name="root");
	void log(LogLevel::Level level, LogEvent::ptr event);
private:
	std::string m_name;
	LogLevel::Level m_level;//定义日志级别
	//LogAppender::ptr
};

class StdoutLogAppder :public LogAppender{

};

class FileLogAppender:public LogAppender{

};
//可能会出现轮转的appender
};
#endif
