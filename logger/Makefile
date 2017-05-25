GCC    = g++

TARGET = log

SRCS   = $(wildcard *.cc) $(wildcard ./muduo/*.cc)

OBJS   = $(patsubst %.cc,%.o,$(SRCS))

LIB    = -lpthread

INC    = -I./muduo/

$(TARGET) : $(OBJS)
	$(GCC) $(INC) -o $@ $^ $(LIB) -std=c++11

.PHONY:rebuild clean
rebuild:clean $(TARGET)
clean:
	rm -rf $(TARGET) $(OBJS)

%.o:%.cc
	$(GCC) $(INC) -o $@ -c $< $(LIB)
