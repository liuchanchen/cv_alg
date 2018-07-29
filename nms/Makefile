CC=gcc
CXX=g++
BIN=a.out
CFLAGS=-std=c++11 -g
SRC=$(wildcard *.cpp)
OBJS=$(patsubst %.cpp, %.o, $(SRC))
LIBS= -lopencv_core \
	  -lopencv_highgui  \
	  -lopencv_imgproc 


all:${BIN}
${BIN}:$(OBJS)
	${CXX} -o $@ $(OBJS) $(LIBS)
%.o:%.cpp
	$(CXX) $(CFLAGS) -c $< -o $@

clean:
	rm *.o ${BIN}
