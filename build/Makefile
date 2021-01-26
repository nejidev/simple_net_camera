CROSS_COMPILE =
LD		      = $(CROSS_COMPILE)ld
CC		      = $(CROSS_COMPILE)gcc
CXX		      = $(CROSS_COMPILE)g++
CFLAGS       := -Wall -O2 -g -std=c++11
CFLAGS       += -Iinclude
LDFLAGS      := -lpthread -lcurl 
TARGET        = simple_net_camera

OBJS := src/main.o src/HttpUpload.o src/CaptureV4l2.o

all:$(OBJS)
	$(CXX) -o $(TARGET) $^  $(LDFLAGS)

clean:
	rm -f *.o
	rm -f $(TARGET)

%.o:%.cpp
	$(CXX) $(CFLAGS) -c -o $@ $<

