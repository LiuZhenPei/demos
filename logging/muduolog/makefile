CC 		 = g++
CFLAGS   = -Wall -g
INCLUDES = -I.
LDFLAGS	 = -lpthread
VPATH    = muduo/base/
#OBJS	 = Logging.o LogFile.o LogStream.o ThreadPool.o TimeZone.o Timestamp.o \
#           FileUtil.o ProcessInfo.o Date.o Thread.o Exception.o

OBJS     = $(patsubst %.cc,%.o,$(wildcard ./muduo/base/*.cc))

TARGETS  = Log_demo

all : $(TARGETS)

Log_demo:Log_demo.o $(OBJS)
	$(CC) -o $@ $^ $(LDFLAGS)

%.o:%.cc
	$(CC) -c $< -o $@ $(INCLUDES) $(CFLAGS)

%.d:%.cc
	@set -e; $(CC) -MM $< $(INCLUDES) > $@.$$$$; \
	sed 's,\($*\)\.o[ :]*,\1.o $@ : ,g' < $@.$$$$ > $@; \
	rm -f $@.$$$$

sinclude $(OBJS:.o=.d)

.PHONY:
clean:
	rm -f $(TARGETS) *.o *.d *.d.*
