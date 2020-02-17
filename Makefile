CC=g++
EXEC=Generator
	
INPUT=\
	generator.cpp \
	main_gen.cpp \

FICHEIROS=$(INPUT) Makefile

OBJS=$(INPUT:.cpp=.o)


CFLAGS=\
       -Wall \

$(EXEC): $(OBJS)
	$(CC) -o $(EXEC) $(CFLAGS) $(OBJS)

clean:
	rm -rf $(OBJS) *.3d
	
