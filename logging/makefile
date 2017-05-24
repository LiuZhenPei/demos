CC 		 = g++
CFLAGS   = -Wall -g
INCLUDES = -I.
LDFLAGS	 = -lpthread
VPATH    = muduo/base/
OBJS	 = Logging.o LogFile.o LogStream.o ThreadPool.o TimeZone.o Timestamp.o \
           FileUtil.o ProcessInfo.o Date.o Thread.o Exception.o

TARGETS  = LogFile_test Logging_test

all : $(TARGETS)

LogFile_test:LogFile_test.o $(OBJS)
	$(CC) -o $@ $^ $(LDFLAGS)

Logging_test:Logging_test.o $(OBJS)
	$(CC) -o $@ $^ $(LDFLAGS)

%.o:%.c
	$(CC) -c $< -o $@ $(INCLUDES) $(CFLAGS)

%.d:%.c
	@set -e; $(CC) -MM $< $(INCLUDES) > $@.$$$$; \
	sed 's,\($*\)\.o[ :]*,\1.o $@ : ,g' < $@.$$$$ > $@; \
	rm -f $@.$$$$

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
