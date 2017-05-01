CC 	= gcc
CXX	= g++
CFLAGS	= -Wall -g
LIBS	= -lm
TARGET	= ent


all : $(TARGET) analyze

analyze : analyze.cpp
	$(CXX) $(CFLAGS) -o $@ $^

$(TARGET) : main.cpp genData.cpp calEnt.cpp
	$(CXX) $(LIBS) $(CFLAGS) -o $@ $^

clean :
	rm -rf *.o $(TARGET)
	rm *.dat
