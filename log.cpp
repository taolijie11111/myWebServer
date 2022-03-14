#include "log.h"

#include <iostream>
#include <functional>
namespace mywebserver{

const char* LogLevel::ToString(LogLevel::Level level){
    switch (level)
    {
#define XX(name)\
    case LogLevel::name: \
        return #name; \
        break;
    XX(DEBUG);
    XX(INFO);
    XX(WARN);
    XX(ERROR);
    XX(FATAL);
#undef XX
    default:
        return "UNKNOW";
    }
    return "UNKNOW";
}

 class MessageFormatItem:public LogFormatter::FormatItem{
        public:
        MassageFormatItem(const std::string& str=""){};
        void format(std::ostream& os,Logger::ptr logger,LogLevel::Level level,LogEvent::ptr event)override{
            os<<event->getContent();
        }
    };

    class LevelFormatItem:public LogFormatter::FormatItem{
        public:
        LevelFormatItem(const std::string& str=""){};
        void format(std::ostream& os,Logger::ptr logger,LogLevel::Level level,LogEvent::ptr event)override{
            os<<LogLevel::ToString(level);
        }
    };

    class ElapseFormatItem:public LogFormatter::FormatItem{
        public:
        ElapseFormatItem(const std::string& str=""){};
        void format(std::ostream& os,Logger::ptr logger,LogLevel::Level level,LogEvent::ptr event)override{
            os<<event->getElapse();
        }
    };

    class NameFormatItem:public LogFormatter::FormatItem{
        public:
        NameFormatItem(const std::string& str=""){};
        void format(std::ostream& os,Logger::ptr logger,LogLevel::Level level,LogEvent::ptr event)override{
            os<<logger ->getName();
        }
    };

    class ThreadIdFormatItem:public LogFormatter::FormatItem{
        public:
        ThreadIdFormatItem(const std::string& str=""){};
        void format(std::ostream& os,Logger::ptr logger,LogLevel::Level level,LogEvent::ptr event)override{
            os<<event->getThreadId();
        }
    };

     class FiberIdFormatItem:public LogFormatter::FormatItem{
        public:
        FilenameFormatItem(const std::string& str=""){};
        void format(std::ostream& os,Logger::ptr logger,LogLevel::Level level,LogEvent::ptr event)override{
            os<<event->getFiberId();
        }
    };

     class DateTimeFormatItem:public LogFormatter::FormatItem{
        public:
        DateTimeFormatItem(const std::string& str=""){};
        DateTimeFormatItem(const std::string& format="%Y:%m:%d %H:%M:%S"):m_format(format){

        }
        void format(std::ostream& os,Logger::ptr logger,LogLevel::Level level,LogEvent::ptr event)override{
            os<<event->getTime();
        }
        private:
        std::string m_format;
    };

     class FilenameFormatItem:public LogFormatter::FormatItem{
        public:
        void format(std::ostream& os,Logger::ptr logger,LogLevel::Level level,LogEvent::ptr event)override{
            os<<event->getFile();
        }
    };

    class LineFormatItem:public LogFormatter::FormatItem{
        public:
        void format(std::ostream& os,Logger::ptr logger,LogLevel::Level level,LogEvent::ptr event)override{
            os<<event->getline();
        }
    };

class StringFormatItem:public LogFormatter::FormatItem{
        public:
        StringFormatItem(const std::string& str):FormatItem(str),m_string(str){}
        void format(std::ostream& os,Logger::ptr logger,LogLevel::Level level,LogEvent::ptr event)override{
            os<<event->getline();
        }
        private:
        std::string m_string;
    };

class NewLineFormatItem:public LogFormatter::FormatItem{
        public:
        void format(std::ostream& os,Logger::ptr logger,LogLevel::Level level,LogEvent::ptr event)override{
            os<<std::endl;
        }
    };


    const char* m_file=nullptr;   //文件名
	int32_t m_line=0;					//行号
	uint32_t m_elapse=0; //程序启动到现在的豪秒数
	uint32_t m_threadID=0;//线程ID
	uint32_t m_fiberID=0; //协程ID
	uint64_t m_time;//时间节
	std::string m_content;


Logger::Logger(const std::string& name)
    :m_name(name){
        m_formatter.reset(new LogFormatter("%d [%p] %f %l %m %n"));
    }
void Logger::addAppender(LogAppender::ptr appender){
    if(!appender->getFormatter()){
        appender->setFormatter(m_formatter);
    }
    m_appenders.push_back(appender);
}
void Logger::delAppender(LogAppender::ptr appender){
    for(auto it=m_appenders.begin();it !=m_appenders.end();++it){
        if(*it==appender){
            m_appenders.erase(it);
            break;
        }
    }
}
void Logger::log(LogLevel::Level level, LogEvent::ptr event){
    if(level>=m_level){
        auto self=shared_from_this();
        for(auto& i:m_appenders){
            i->log(self,level,event);
        }
    }
}
void Logger::debug(LogEvent::ptr event){
    log(LogLevel::DEBUG,event);
}
void Logger::info(LogEvent::ptr event){
    log(LogLevel::INFO,event);
}
void Logger::warn(LogEvent::ptr event){
    log(LogLevel::WARN,event);
}
void Logger::error(LogEvent::ptr event){
    log(LogLevel::ERROR,event);
}
void Logger::fatal(LogEvent::ptr event){
    log(LogLevel::FATAL,event);
}
FileLogAppender::FileLogAppender(const std::string& filename)
:m_filename(filename){

}

bool FileLogAppender::reopen(){
    if(m_filestream){
        m_filestream.close();
    }
    m_filestream.open(m_filename);
    return !!m_filestream;
}

void StdoutLogAppder::log(std::shared_ptr<Logger> logger,LogLevel::Level level,LogEvent::ptr event) {
    if(level>=m_level){
        std::cout<<m_formatter.format(logger,level,event) <<std::endl;
    }
}
void FileLogAppender::log(std::shared_ptr<Logger> logger,LogLevel::Level level,LogEvent::ptr event) {
if(level>=m_level){
    m_filestream << m_formatter.format(logger,level,event); 
}
}
LogFormatter::LogFormatter(const std::string& pattern)
:m_pattern(pattern){
	
}

std::string LogFormatter::format(std::shared_ptr<Logger> logger,LogLevel::Level level,LogEvent::ptr event){
    std::stringstream ss;
    for(auto& i:m_items){
        i->format(ss,logger,level,event);
    }
    return ss.str();
}

//需要一个格式
//%xxx %xxx{xxx} %%
void LogFormatter::init(){
    //写一个格式
    //str,format,type
    std::vector<std::tuple<std::string,std::string,int>>vec;
    std::string nstr;
    size_t last_pos=0;
    for(size_t i=0;i<m_pattern.size();++i){
        if(m_pattern[i]!='%'){
            nstr.append(1,m_pattern[i]);
            continue;
        }

        if((i+1)<m_pattern.size()){
            if(m_pattern[i]!='%'){
                nstr.append(1,'%');
                continue;
            }
        }
        size_t n=i+1;
        int fmt_status=0;
        size_t fmt_begin=0;

        std::string str;
        std::string fmt;
        while ((n<m_pattern.size()))
        {
            /* code */
            if(isspace(m_pattern[n])){//判断有无空格符
            break;
            }
            if(fmt_status==0){
            if(m_pattern[n]=='{'){
             str=m_pattern.substr(i+1,n-i-1); 
             fmt_status=1;//格式接受，解析格式
             ++n;
             fmt_begin=n;
             continue;
            }
            }
            if(fmt_status==1){
                if(m_pattern[n]=='}'){
                    fmt=m_pattern.substr(fmt_begin+1,n-fmt_begin-1);
                    fmt_status =2;
                    ++n;
                    break;
                }
            }
        }
        if(fmt_status==0){
            if(nstr.empty()){
            vec.push_back(std::make_tuple(nstr,"",0));
            }
            str=m_pattern.substr(i+1,n-i-1);
            vec.push_back(std::make_tuple(str,fmt,1));
            i=n;
        }else if(fmt_status==1){
            std::cout<<"pattern parse error:"<<m_pattern<<" - "<<m_pattern.substr(i)<<std::endl;
            vec.push_back(std::make_tuple("<<pattern_error>>",fmt,0));
        }else if(fmt_status==2){
            vec.push_back(std::make_tuple(str,fmt,1));
            i=n;
        }
        if(!nstr.empty()){
            vec.push_back(std::make_tuple(nstr,"",0));
        }
        
        static std::map<std::string,std::function<FormatItem::ptr(const std::string& str)>> s_format_items={
#define XX(str,C)  {str,[](const std::string& fmt){return FormatItem::ptr(new C(fmt));}}

            XX(m,MessageFormatItem),
            XX(p,LevelFormatItem),
            XX(r,ElapseFormatItem),
            XX(c,NameFormatItem),
            XX(t,ThreadIdFormatItem),
            XX(n,NewLineFormatItem),
            XX(d,DateTimeFormatItem),
            XX(f,FilenameFormatItem),
            XX(l,LineFormatItem),
#undef XX
        };
        
        // %c 输出日志信息所属的类的全名
        // %d 输出日志时间点的日期或时间，默认格式为ISO8601，也可以在其后指定格式，比如：%d{yyy-MM-dd HH:mm:ss }，输出类似：2002-10-18- 22：10：28
        // %f 输出日志信息所属的类的类名
        // %l 输出日志事件的发生位置，即输出日志信息的语句处于它所在的类的第几行
        // %m 输出代码中指定的信息，如log(message)中的message
        // %n 输出一个回车换行符，Windows平台为“rn”，Unix平台为“n”
        // %p 输出优先级，即DEBUG，INFO，WARN，ERROR，FATAL。如果是调用debug()输出的，则为DEBUG，依此类推
        // %r 输出自应用启动到输出该日志信息所耗费的毫秒数
        // %t 输出产生该日志事件的线程

        for(auto& i :vec){
            if(std::get<2>(i)==0){
                m_item.push_back(FormatItem::ptr(new StringFormatItem(std::get<0>(i))));
            }else{
                auto it = s_format_items.find(std::get<0>(i));
                if(it == s_format_items.end()){
                    m_item.push_back(FormatItem::ptr(new StringFormatItem("<<error_format %"+std::get<0>(i)+">>")));
                }else{
                    m_item.push_back(it->second(std::get<1>(i)));
                }
            }

            std::cout << std::get<0>(i)<<" - "<<std::get<2>(i)<<std::endl;
        }
    }
   
}
