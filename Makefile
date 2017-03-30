CC 	= gcc
CFLAGS	= -Wall -g
LIBS	= -lm
TARGET	= ent


all : $(TARGET)

$(TARGET) : genData.o calEnt.o main.o
	$(CC) $(CFLAGS) -o $@ $^ $(LIBS) 
		   
genData.o : genData.c
	$(CC) -c $^

calEnt.o : calEnt.c
	$(CC) -c $^ 

main.o : main.c
	$(CC) -c $^

clean :
	rm -rf *.o $(TARGET)
	rm *.dat
