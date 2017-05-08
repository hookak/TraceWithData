CC 	= gcc
CXX	= g++
CFLAGS	= -Wall -g
LIBS	= -lm
TARGET	= generator


all : $(TARGET) analyzer

analyze : analyzer.cpp
	$(CXX) $(CFLAGS) -o $@ $^

$(TARGET) : generator.cpp box.cpp
	$(CXX) $(LIBS) $(CFLAGS) -o $@ $^

clean :
	rm -rf *.o $(TARGET)
	rm *.dat
