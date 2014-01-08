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