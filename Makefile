CC 	= gcc
CXX	= g++
CFLAGS	= -Wall -g
LIBS	= -lm
TARGET	= generator


all :  $(TARGET) blkToLba csvToLba  

#analyzer : analyzer.cpp page.cpp
#	$(CXX) $(LIBS) $(CFLAGS) -o $@ $^

$(TARGET) : generator.cpp box.cpp
	$(CXX) $(LIBS) $(CFLAGS) -o $@ $^

blkToLba : blkToLba.cpp
	$(CXX) $(LIBS) $(CFLGAS) -o $@ $^

csvToLba : csvToLba.cpp
	$(CXX) $(LIBS) $(CFLGAS) -o $@ $^



clean :
	rm -rf *.o $(TARGET) 
	rm blkToLba csvToLba
	rm  *.dac 
