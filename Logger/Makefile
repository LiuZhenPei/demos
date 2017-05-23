GCC    = g++

TARGET = log

SRCS   = $(wildcard *.cc) $(wildcard ./muduo/*.cc)

OBJS   = $(SRCS:.cc=.o)

LIB    = -lpthread

INC    = -I./muduo/

$(TARGET):$(OBJS)
	$(GCC) $(INC) -o $@ $^ $(LIB)

.PHONY:clean
clean:
	rm -rf $(TARGET) $(OBJS)

%.o:%.cc
	$(GCC) $(INC) -o $@ -c $< $(LIB)
