CC=g++
EXEC=Generator

HTML_PATH= html
LATEX_PATH= latex
GENERATOR= Generator
	
INPUT=\
	generator.cpp \
	main_gen.cpp \

FICHEIROS=$(INPUT) Makefile

OBJS=$(INPUT:.cpp=.o)


CFLAGS=\
       -Wall \

$(EXEC): $(OBJS)
	$(CC) -o $(EXEC) $(CFLAGS) $(OBJS)

doc:
	doxygen -g
	doxygen

clean:
	@echo "remove *.3d , Generator, $(HTML_PATH) e $(LATEX_PATH)"
	@$(RM) -rf $(OBJS) *.3d
	@$(RM) $(GENERATOR)
	@$(RM) -r $(HTML_PATH)
	@$(RM) -r $(LATEX_PATH)
	
