#include <Logging.h>
#include <LogFile.h>
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
	loggingDemos();
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
    g_file = ::fopen("/tmp/muduo_log", "ae");
    Logger::setOutput(loggingOutput);
    Logger::setFlush(loggingFlush);

    LOG_TRACE << "trace ...";
    LOG_DEBUG << "debug ...";
    LOG_INFO << "info ...";
    LOG_WARN << "warn ...";
    LOG_ERROR << "error ...";
    //LOG_FATAL<<"fatal ...";
    errno = 13;
    LOG_SYSERR << "syserr ...";
    //LOG_SYSFATAL<<"sysfatal ...";

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
  g_logFile.reset(new muduo::LogFile(::basename(name), 256 * 1024 * 1024));
  muduo::Logger::setOutput(logfileOutput);
  muduo::Logger::setFlush(logfileFlush);

  muduo::string line = "1234567890 abcdefghijklmnopqrstuvwxyz ABCDEFGHIJKLMNOPQRSTUVWXYZ ";

  for (int i = 0; i < 10000; ++i)
  {
    LOG_INFO << line << i;
    usleep(1000);
  }
}
