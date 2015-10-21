CC = gcc
CFLAGS = -Wall -O0 -D_XOPEN_SOURCE=600
LIBS = 
SRCS = main.c	
OBJS = main.o

EXE = async

.PHONY: clean

$(EXE): $(OBJS)	
	@echo "Building $(EXE)..."
	@$(CC) $(CFLAGS) -o $@ $^ $(LIBS)
	@echo "Build complete..."

clean:
	@echo "Cleaning object files..."
	@$(RM) $(OBJS)
