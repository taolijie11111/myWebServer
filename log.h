#ifndef __SYLAR_LOG_H__
#define __SYLAR_LOG_H__
//#once

#include<string>
#include<list>
//int32
#include<stdint.h>


#include<memory>

//file out 
//#include <stringstreams>
#include <sstream>
#include <fstream>

namespace mywebserver{

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
	virtual void log(LogLevel::Level level,LogEvent::ptr event)=0;//
	void setFormatter(LogFormatter::ptr val){m_formatter=val;}
	LogFormatter::ptr getFormatter() const {return m_formatter;}
	protected:
		LogLevel::Level m_level;//主要根据哪些日志定义日志级别
		LogFormatter::ptr m_formatter;
};

//日志器
class Logger{
	public:
		typedef std::shared_ptr<Logger> ptr;
		Logger(const std::string& name="root");
		void log(LogLevel::Level level, LogEvent::ptr event);

		void debug(LogEvent::ptr event);
		void info(LogEvent::ptr event);
		void warn(LogEvent::ptr event);
		void error(LogEvent::ptr event);
		void fatal(LogEvent::ptr event);

		void addAppender(LogAppender::ptr appender);
		void delAppender(LogAppender::ptr appender);
		LogLevel::Level getLevel() const {return m_level;}
		void setLevel(LogLevel::Level val){m_level=val;}

	private:
		std::string m_name;		                 //日志名称
		LogLevel::Level m_level;                //定义日志级别
		std::list<LogAppender::ptr> m_appenders;//appender集合
};

//输出到控制台
class StdoutLogAppder :public LogAppender{
public:
	typedef std::shared_ptr<StdoutLogAppder> ptr;
	virtual void log(LogLevel::Level level,LogEvent::ptr event) override;//override 用于描述确实是从父类那边继承过来的实现
private:
};

//输出到文件
class FileLogAppender:public LogAppender{
public:
typedef std::shared_ptr<FileLogAppender> ptr;
FileLogAppender(const std::string& filename);
virtual void log(LogLevel::Level level,LogEvent::ptr event) override;
private:
std::string m_name;
std::ofstream m_filestream;
};
//可能会出现轮转的appender
};
#endif
