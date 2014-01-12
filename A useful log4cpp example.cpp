// A useful log4cpp example

// The extremly expert implementation of log4cpp is unbalance by a very poor documentation. The test files provided in the distribution are not really that celebrating the features in a compact example. These following files shall be thought to help on that particular problem.

// This example loads the properties file located below to init the log4cpp categories and illustrates some typical log4cpp idioms in intermingling your cpp code. Note that 
// the log stream log_sub1 will be written to the file "log4cpp_inited.log".

// The include file names are not embraced properly - Blogger's templating. Modify if compiling fails.

// Put the files into any folder and try to compile by something like:

// g++ -gstabs+ -w -Iheaders log4cpp_inited.cpp -o log4cpp_inited.exe -llog4cpp

// log4cpp_inited.cpp




#include "stdio.h"
#include "iostream"

#include "log4cpp/Category.hh"
#include "log4cpp/Appender.hh"
#include "log4cpp/OstreamAppender.hh"
#include "log4cpp/FileAppender.hh"
#include "log4cpp/Layout.hh"
#include "log4cpp/BasicLayout.hh"
#include "log4cpp/Priority.hh"
#include "log4cpp/NDC.hh"
#include "log4cpp/PropertyConfigurator.hh"

#define CODE_LOCATION __FILE__

// DEBUG < INFO < NOTICE < WARN < ERROR < CRIT < ALERT < FATAL = EMERG

#define LOG_EMERG(__logstream)  __logstream << log4cpp::Priority::EMERG << CODE_LOCATION
#define LOG_ALERT(__logstream)  __logstream << log4cpp::Priority::ALERT << CODE_LOCATION
#define LOG_CRIT(__logstream)  __logstream << log4cpp::Priority::CRIT << CODE_LOCATION
#define LOG_ERROR(__logstream)  __logstream << log4cpp::Priority::ERROR << CODE_LOCATION
#define LOG_WARN(__logstream)  __logstream << log4cpp::Priority::WARN << CODE_LOCATION
#define LOG_NOTICE(__logstream)  __logstream << log4cpp::Priority::NOTICE << CODE_LOCATION
#define LOG_INFO(__logstream)  __logstream << log4cpp::Priority::INFO << CODE_LOCATION
#define LOG_DEBUG(__logstream)  __logstream << log4cpp::Priority::DEBUG << CODE_LOCATION

int main(int argc, char** argv) 
{
    log4cpp::Category &log_root = log4cpp::Category::getRoot();
    log4cpp::Category &log_sub1 = log4cpp::Category::getInstance( std::string("log_sub1") );
    log4cpp::Category &log_sub2 = log4cpp::Category::getInstance( std::string("log_sub1.log_sub2") );

    const char *file_log4cpp_init = "log4cpp_inited.log4cpp_properties";

    try
    {
      log4cpp::PropertyConfigurator::configure( file_log4cpp_init );
    }
    catch( log4cpp::ConfigureFailure &e )
      {
        std::cout 
         << e.what() 
         << " [log4cpp::ConfigureFailure catched] while reading " 
         << file_log4cpp_init 
         << std::endl;

        exit(1);
      }
 
    std::cout << " root priority = " << log_root.getPriority() << std::endl;
    std::cout << " log_sub1 priority = " << log_sub1.getPriority() << std::endl;
    std::cout << " log_sub2 priority = " << log_sub2.getPriority() << std::endl;

    //root

    log_root.error("root error");
    log_root.warn("root warn");

    //log_sub1

    log_sub1.error("log_sub1 error");
    log_sub1.warn("log_sub1 warn");
    log_sub1.info("%s%d", "i = ", 1122334455 );
    log_sub1.log( log4cpp::Priority::NOTICE, "reopen log" );
    log_sub1.info("log reopened");
    log_sub1.warn("could not reopen log");

    //log_sub2

    log_sub2.error("log_sub2 error");
    log_sub2.warn("log_sub2 warn");
    log_sub2.warnStream() << "streamed warn";
 
    log_sub2 
      << log4cpp::Priority::WARN 
      << "warn2" 
      << " warn3" 
      << log4cpp::eol 
      << " warn4"
      ;

#define CODE_LOCATION "[end] "

    // the log stream is not compatible with an ostream. Here an example how
    // to still output everything as easy as to an ostream with the aid of an
    // ostringstream. Of course, performance will be relative poor for this construct.

    std::string obj( "word" );

    ostringstream log_stream_tmp << "exiting without a " << obj << endl;

    LOG_NOTICE(log_root) << log_stream_tmp.str();  
}



// log4cpp_inited.log4cpp_properties





log4j.rootCategory=DEBUG, rootAppender
log4j.appender.rootAppender=org.apache.log4j.ConsoleAppender
log4j.appender.rootAppender.layout=org.apache.log4j.BasicLayout

log4j.category.log_sub1=NOTICE, A1
log4j.category.log_sub1.log_sub2=NOTICE, A2
log4j.additivity.category.log_sub1=false
              
log4j.appender.A1=org.apache.log4j.FileAppender
log4j.appender.A1.fileName=log4cpp_inited.log
log4j.appender.A1.layout=org.apache.log4j.PatternLayout
log4j.appender.A1.layout.ConversionPattern=%n %d [%t] %-5p %c - %m%n%n%n
       
log4j.appender.A2=org.apache.log4j.ConsoleAppender
log4j.appender.A2.layout=org.apache.log4j.PatternLayout
log4j.appender.A2.layout.ConversionPattern=%d [%t] %-5p %c - %m%n



//-------------------------------------------------------------------------------------------------
官方两个例子
Sample main.cpp file that logs onto console and into file program.log. Loggers and appenders are created and configured manually. Example makes use of both function-style logging and stream-style logging.

// main.cpp

#include "log4cpp/Category.hh"
#include "log4cpp/Appender.hh"
#include "log4cpp/FileAppender.hh"
#include "log4cpp/OstreamAppender.hh"
#include "log4cpp/Layout.hh"
#include "log4cpp/BasicLayout.hh"
#include "log4cpp/Priority.hh"

int main(int argc, char** argv) {
  log4cpp::Appender *appender1 = new log4cpp::OstreamAppender("console", &std::cout);
  appender1->setLayout(new log4cpp::BasicLayout());

  log4cpp::Appender *appender2 = new log4cpp::FileAppender("default", "program.log");
  appender2->setLayout(new log4cpp::BasicLayout());

  log4cpp::Category& root = log4cpp::Category::getRoot();
  root.setPriority(log4cpp::Priority::WARN);
  root.addAppender(appender1);

  log4cpp::Category& sub1 = log4cpp::Category::getInstance(std::string("sub1"));
  sub1.addAppender(appender2);

  // use of functions for logging messages
  root.error("root error");
  root.info("root info");
  sub1.error("sub1 error");
  sub1.warn("sub1 warn");

  // printf-style for logging variables
  root.warn("%d + %d == %s ?", 1, 1, "two");

  // use of streams for logging messages
  root << log4cpp::Priority::ERROR << "Streamed root error";
  root << log4cpp::Priority::INFO << "Streamed root info";
  sub1 << log4cpp::Priority::ERROR << "Streamed sub1 error";
  sub1 << log4cpp::Priority::WARN << "Streamed sub1 warn";

  // or this way:
  root.errorStream() << "Another streamed error";

  return 0;
}
Console output for that example
1352973121 ERROR  : root error
1352973121 ERROR sub1 : sub1 error
1352973121 WARN sub1 : sub1 warn
1352973121 WARN  : 1 + 1 == two ?
1352973121 ERROR  : Streamed root error
1352973121 ERROR sub1 : Streamed sub1 error
1352973121 WARN sub1 : Streamed sub1 warn
1352973121 ERROR  : Another streamed error


Sample main.cpp file that makes use of logging configuration file log4cpp.properties

// main.cpp

#include <log4cpp/Category.hh>
#include <log4cpp/PropertyConfigurator.hh>

int main(int argc, char* argv[])
{
  std::string initFileName = "log4cpp.properties";
  log4cpp::PropertyConfigurator::configure(initFileName);

  log4cpp::Category& root = log4cpp::Category::getRoot();

  log4cpp::Category& sub1 = 
    log4cpp::Category::getInstance(std::string("sub1"));

  log4cpp::Category& sub2 = 
    log4cpp::Category::getInstance(std::string("sub1.sub2"));

  root.warn("Storm is coming");

  sub1.debug("Received storm warning");
  sub1.info("Closing all hatches");

  sub2.debug("Hiding solar panels");
  sub2.error("Solar panels are blocked");
  sub2.debug("Applying protective shield");
  sub2.warn("Unfolding protective shield");
  sub2.info("Solar panels are shielded");

  sub1.info("All hatches closed");

  root.info("Ready for storm.");

  log4cpp::Category::shutdown();

  return 0;
}
Configuration file for use with that code

# log4cpp.properties

log4cpp.rootCategory=DEBUG, rootAppender
log4cpp.category.sub1=DEBUG, A1, A2
log4cpp.category.sub1.sub2=DEBUG, A3

log4cpp.appender.rootAppender=ConsoleAppender
log4cpp.appender.rootAppender.layout=PatternLayout
log4cpp.appender.rootAppender.layout.ConversionPattern=%d [%p] %m%n 

log4cpp.appender.A1=FileAppender
log4cpp.appender.A1.fileName=A1.log
log4cpp.appender.A1.layout=BasicLayout

log4cpp.appender.A2=FileAppender
log4cpp.appender.A2.threshold=WARN
log4cpp.appender.A2.fileName=A2.log
log4cpp.appender.A2.layout=PatternLayout
log4cpp.appender.A2.layout.ConversionPattern=%d [%p] %m%n 

log4cpp.appender.A3=RollingFileAppender
log4cpp.appender.A3.fileName=A3.log
log4cpp.appender.A3.maxFileSize=200
log4cpp.appender.A3.maxBackupIndex=1
log4cpp.appender.A3.layout=PatternLayout
log4cpp.appender.A3.layout.ConversionPattern=%d [%p] %m%n 
Console output for that example
2012-11-14 18:44:58,163 [WARN] Storm is coming
2012-11-14 18:44:58,166 [DEBUG] Received storm warning
2012-11-14 18:44:58,170 [INFO] Closing all hatches
2012-11-14 18:44:58,172 [DEBUG] Hiding solar panels
2012-11-14 18:44:58,175 [ERROR] Solar panels are blocked
A3.log.1
2012-11-14 18:44:58,179 [DEBUG] Applying protective shield
2012-11-14 18:44:58,183 [WARN] Unfolding protective shield
2012-11-14 18:44:58,186 [INFO] Solar panels are shielded
2012-11-14 18:44:58,190 [INFO] All hatches closed
2012-11-14 18:44:58,192 [INFO] Ready for storm.
There are three loggers (including root) and three appenders. Root logger redirects all its own input and input of its descendant onto console. Descendant sub1 writes into two files, filtering output by message priority for second of them. Descendant sub1.sub2 writes also into rolling file. Log file A3.log is rolled over as soon as it reaches limit of 200 bytes, 1 backup log file is kept.







//========================================================================================
Log4j比较全面的配置

log4j.rootLogger=DEBUG,CONSOLE,A1,im
log4j.addivity.org.apache=true
# 应用于控制台
log4j.appender.CONSOLE=org.apache.log4j.ConsoleAppender
log4j.appender.Threshold=DEBUG
log4j.appender.CONSOLE.Target=System.out
log4j.appender.CONSOLE.layout=org.apache.log4j.PatternLayout
log4j.appender.CONSOLE.layout.ConversionPattern=[framework] %d - %c -%-4r [%t] %-5p %c %x - %m%n
#log4j.appender.CONSOLE.layout.ConversionPattern=[start]%d{DATE}[DATE]%n%p[PRIORITY]%n%x[NDC]%n%t[thread] n%c[CATEGORY]%n%m[MESSAGE]%n%n
#应用于文件
log4j.appender.FILE=org.apache.log4j.FileAppender
log4j.appender.FILE.File=file.log
log4j.appender.FILE.Append=false
log4j.appender.FILE.layout=org.apache.log4j.PatternLayout
log4j.appender.FILE.layout.ConversionPattern=[framework] %d - %c -%-4r [%t] %-5p %c %x - %m%n
# Use this layout for LogFactor 5 analysis
# 应用于文件回滚
log4j.appender.ROLLING_FILE=org.apache.log4j.RollingFileAppender
log4j.appender.ROLLING_FILE.Threshold=ERROR
log4j.appender.ROLLING_FILE.File=rolling.log //文件位置,也可以用变量${java.home}、rolling.log
log4j.appender.ROLLING_FILE.Append=true //true:添加 false:覆盖
log4j.appender.ROLLING_FILE.MaxFileSize=10KB //文件最大尺寸
log4j.appender.ROLLING_FILE.MaxBackupIndex=1 //备份数
log4j.appender.ROLLING_FILE.layout=org.apache.log4j.PatternLayout
log4j.appender.ROLLING_FILE.layout.ConversionPattern=[framework] %d - %c -%-4r [%t] %-5p %c %x - %m%n

#应用于socket
log4j.appender.SOCKET=org.apache.log4j.RollingFileAppender
log4j.appender.SOCKET.RemoteHost=localhost
log4j.appender.SOCKET.Port=5001
log4j.appender.SOCKET.LocationInfo=true
# Set up for Log Facter 5
log4j.appender.SOCKET.layout=org.apache.log4j.PatternLayout
log4j.appender.SOCET.layout.ConversionPattern=[start]%d{DATE}[DATE]%n%p[PRIORITY]%n%x[NDC]%n%t[thread]%n%c[CATEGORY]%n%m[MESSAGE]%n%n

# Log Factor 5 Appender
log4j.appender.LF5_APPENDER=org.apache.log4j.lf5.LF5Appender
log4j.appender.LF5_APPENDER.MaxNumberOfRecords=2000
# 发送日志给邮件
log4j.appender.MAIL=org.apache.log4j.net.SMTPAppender
log4j.appender.MAIL.Threshold=FATAL
log4j.appender.MAIL.BufferSize=10
log4j.appender.MAIL.From=web@www.wuset.com
log4j.appender.MAIL.SMTPHost=www.wusetu.com
log4j.appender.MAIL.Subject=Log4J Message
log4j.appender.MAIL.To=web@www.wusetu.com
log4j.appender.MAIL.layout=org.apache.log4j.PatternLayout
log4j.appender.MAIL.layout.ConversionPattern=[framework] %d - %c -%-4r [%t] %-5p %c %x - %m%n
# 用于数据库
log4j.appender.DATABASE=org.apache.log4j.jdbc.JDBCAppender
log4j.appender.DATABASE.URL=jdbc:mysql://localhost:3306/test
log4j.appender.DATABASE.driver=com.mysql.jdbc.Driver
log4j.appender.DATABASE.user=root
log4j.appender.DATABASE.password=
log4j.appender.DATABASE.sql=INSERT INTO LOG4J (Message) VALUES (’[framework] %d - %c -%-4r [%t] %-5p %c %x - %m%n’)
log4j.appender.DATABASE.layout=org.apache.log4j.PatternLayout
log4j.appender.DATABASE.layout.ConversionPattern=[framework] %d - %c -%-4r [%t] %-5p %c %x - %m%n

log4j.appender.A1=org.apache.log4j.DailyRollingFileAppender
log4j.appender.A1.File=SampleMessages.log4j
log4j.appender.A1.DatePattern=yyyyMMdd-HH’.log4j’
log4j.appender.A1.layout=org.apache.log4j.xml.XMLLayout
#自定义Appender
log4j.appender.im = net.cybercorlin.util.logger.appender.IMAppender
log4j.appender.im.host = mail.cybercorlin.net
log4j.appender.im.username = username
log4j.appender.im.password = password
log4j.appender.im.recipient = corlin@cybercorlin.net
log4j.appender.im.layout=org.apache.log4j.PatternLayout
log4j.appender.im.layout.ConversionPattern =[framework] %d - %c -%-4r [%t] %-5p %c %x - %m%n



//===================================================================================
http://blog.chinaunix.net/uid-20357359-id-1963611.html
封装了一个的log库 

为了给项目增加一个log库，找了一些开源的库，包括log4cpp、log4cxx。

总得来说，log4cxx是功能最彪悍的一个，是线程安全的，支持的PatternLayout符号表示的语义也最多，但是唯一不好的一点就是它需要apr库，有些不爽。log4cpp倒是一个单独的库，但是不是完整的支持线程安全。两个库的配置文件都跟log4j一样，两者的API也差不多，hacking了一个下午，封装了一个库，将log4cpp和log4cxx的接口统一起来了。

//logger.h

#ifndef _LOGGER_H
#define _LOGGER_H

#include <string>
using namespace std;

#ifdef USE_LOG4CPP
#include "log4cpp/Category.hh"
#include "log4cpp/CategoryStream.hh"
#include "log4cpp/PropertyConfigurator.hh"

using namespace log4cpp;
#elif defined USE_LOG4CXX
#include <log4cxx/logger.h>
#include <log4cxx/propertyconfigurator.h>
using namespace log4cxx;
#endif

#define LoggerName "MyApp"
#define AppenderName "rootAppender"

#ifdef USE_LOG4CPP

extern log4cpp::Category *logger_;
#define LOG_DEBUG(msg) \
    LOG4CPP_DEBUG_S((*logger_))<<msg<<"("<<__FILE__<<":"<<__LINE__<<")";
#define LOG_INFO(msg) \
    LOG4CPP_INFO_S((*logger_))<<msg<<"("<<__FILE__<<":"<<__LINE__<<")";
#define LOG_WARN(msg) \
    LOG4CPP_WARN_S((*logger_))<<msg<<"("<<__FILE__<<":"<<__LINE__<<")";
#define LOG_ERROR(msg) \
    LOG4CPP_ERROR_S((*logger_))<<msg<<"("<<__FILE__<<":"<<__LINE__<<")";
#define LOG_FATAL(msg) \
    LOG4CPP_FATAL_S((*logger_))<<msg<<"("<<__FILE__<<":"<<__LINE__<<")";

#elif defined USE_LOG4CXX

extern LoggerPtr logger_;
#define LOG_DEBUG(msg) \
{\
    stringstream temp_; \
    temp_<<msg<<"("<<__FILE__<<":"<<__LINE__<<")"; \
    LOG4CXX_DEBUG(logger_, temp_.str()); \
}
#define LOG_INFO(msg) \
{\
    stringstream temp_; \
    temp_<<msg<<"("<<__FILE__<<":"<<__LINE__<<")"; \
    LOG4CXX_INFO(logger_, temp_.str()); \
}
#define LOG_WARN(msg) \
{\
    stringstream temp_; \
    temp_<<msg<<"("<<__FILE__<<":"<<__LINE__<<")"; \
    LOG4CXX_WARN(logger_, temp_.str()); \
}
#define LOG_ERROR(msg) \
{\
    stringstream temp_; \
    temp_<<msg<<"("<<__FILE__<<":"<<__LINE__<<")"; \
    LOG4CXX_ERROR(logger_, temp_.str()); \
}
#define LOG_FATAL(msg) \
{\
    stringstream temp_; \
    temp_<<msg<<"("<<__FILE__<<":"<<__LINE__<<")"; \
    LOG4CXX_FATAL(logger_, temp_.str()); \
}

#endif

void InitLog(const char *conf_file);

#endif /*_LOGGER_H*/


//logger.cpp


#include "logger.h"

#ifdef USE_LOG4CPP
log4cpp::Category *logger_;
#elif defined USE_LOG4CXX
LoggerPtr logger_(Logger::getLogger(LoggerName));
#endif

void InitLog(const char *conf_file)
{
#ifdef USE_LOG4CPP
    log4cpp::PropertyConfigurator::configure(conf_file);
    log4cpp::Category& log = log4cpp::Category::getInstance(string(AppenderName));
    logger_ = &log;
#elif defined USE_LOG4CXX
    PropertyConfigurator::configure(File(conf_file)); // Configure file, log4j format
    logger_ = Logger::getLogger("org.apache.log4j.RollingFileAppender"); // Appender name
#endif
}

#if 0
int main(int argc, char* argv[])
{
    string conf_file = "./log.conf";
    InitLog(conf_file.c_str());

    LOG_DEBUG("This is Some Debug Message.");
    LOG_INFO("This is Some Info Message.");
    LOG_WARN("This is Some Warn Message.");
    LOG_ERROR("This is Some Error Message.");
    LOG_FATAL("This is Some Fatal Message.");
    
    return 0;
}
#endif


封装之后的库，在InitLog(conf_file)之后，就可以直接使用LOG_DEBUG(msg)的方式写log了，直接修改配置文件就可以控制log的行为了;-)

log.conf
log4j.rootCategory=INFO, rootAppender

log4j.Category.rootAppender=INFO
log4j.appender.rootAppender=org.apache.log4j.RollingFileAppender
#由于log4cpp无法处理"MB"等字母表示的文件大小，为了兼容性，建议直接使用数字表示。
#log4j.appender.rootAppender.maxFileSize = 1MB
log4j.appender.rootAppender.maxFileSize = 1024000
log4j.appender.rootAppender.maxBackupIndex = 2
log4j.appender.rootAppender.append=true
log4j.appender.rootAppender.fileName=emule.log

log4j.appender.rootAppender.layout=org.apache.log4j.PatternLayout
#log4j.appender.rootAppender.layout.ConversionPattern=%p [%d] %F | %m%n
log4j.appender.rootAppender.layout.ConversionPattern=[%d] %-5p | %m%n

注意：使用log4cpp的配置文件时，RollingFileAppender方式设置的maxFileSize不能够使用"MB"等字母，需要直接填写数字；log4cxx没有这个问题。因此，在设置maxFileSize时候建议直接使用数字，而不是"MB"等字母表示。

使用的时候，通过定义不同的宏来使用不同的库，例如使用log4cpp，编译的时候，使用-DUSE_LOG4CPP宏，并在链接的时候，使用-llog4cpp进行库链接；同理使用log4cxx。

Makefile.log4cpp
CXX=g++
CXXFLAGS=-g -fPIC -c -DUSE_LOG4CPP
LDFLAGS=-llog4cpp
OBJS= logger.o
PROG=t_log

all: t_log

.cpp.o :
    $(CXX) $(CXXFLAGS) $(INCLUDE) $<

t_log: t_log.o $(OBJS)
    $(CXX) -o $@ $^ $(LDFLAGS)

clean:
    rm -f *.o
    rm -f $(PROG)