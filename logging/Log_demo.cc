#include <muduo/base/Logging.h>
#include <muduo/base/LogFile.h>
#include <errno.h>
#include <stdio.h>

using namespace muduo;

FILE *g_file;
boost::scoped_ptr<muduo::LogFile> g_logFile;

void loggingOutput(const char *msg, int len);
void loggingFlush();
void loggingDemos();

void logfileOutput(const char* msg, int len);
void logfileFlush();
void logfileDemos(char *fileName);

int main(int argc, char* argv[]){
    /*纪录日志级别信息*/
	loggingDemos();
    /*日志每256K回滚一次*/
	logfileDemos(argv[0]);

	return 0;
}


void loggingOutput(const char *msg, int len)
{
    if (g_file)
    {
        fwrite(msg, 1, len, g_file);
    }
}

void loggingFlush()
{
    fflush(g_file);
}

void loggingDemos()
{
    g_file = ::fopen("./muduo_log", "ae");
    Logger::setOutput(loggingOutput);
    Logger::setFlush(loggingFlush);

    LOG_TRACE << "trace ...";
    LOG_DEBUG << "debug ...";
    LOG_INFO << "info ...";
    LOG_WARN << "warn ...";
    LOG_ERROR << "error ...";
    errno = 13;
    LOG_FATAL << "fatal error...";

    ::fclose(g_file);
}

void logfileOutput(const char* msg, int len)
{
  g_logFile->append(msg, len);
}

void logfileFlush()
{
  g_logFile->flush();
}

void logfileDemos(char *fileName)
{
  char name[256];
  strncpy(name, fileName, 256);
  g_logFile.reset(new muduo::LogFile(::basename(name), 256 * 1024));//当达到256k时回滚一次
  muduo::Logger::setOutput(logfileOutput);
  muduo::Logger::setFlush(logfileFlush);

  muduo::string line = "1234567890 abcdefghijklmnopqrstuvwxyz ABCDEFGHIJKLMNOPQRSTUVWXYZ ";

  for (int i = 0; i < 10000; ++i)
  {
    LOG_INFO << line << i;
    usleep(1000);
  }
}
