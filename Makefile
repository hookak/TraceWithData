CC 	= gcc
CXX	= g++
CFLAGS	= -Wall -g
LIBS	= -lm
TARGET	= generator


all : analyzer $(TARGET)  

analyzer : analyzer.cpp page.cpp
	$(CXX) $(LIBS) $(CFLAGS) -o $@ $^

$(TARGET) : generator.cpp box.cpp
	$(CXX) $(LIBS) $(CFLAGS) -o $@ $^

clean :
	rm -rf *.o $(TARGET) analyzer
	rm *.dat *.dac *.in
