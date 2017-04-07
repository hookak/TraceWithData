CC 	= gcc
CXX	= g++
CFLAGS	= -Wall -g
LIBS	= -lm
TARGET	= ent


all : $(TARGET)

$(TARGET) : main.cpp genData.cpp calEnt.cpp
	$(CXX) $(LIBS) $(CFLAGS) -o $@ $^

clean :
	rm -rf *.o $(TARGET)
	rm *.dat
