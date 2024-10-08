CC = clang-15
CFLAGS = -g -flto -fvisibility=hidden -fsanitize=cfi -fno-sanitize-trap=all -Wall

# Object files
OBJ = main.o operation.o util.o irrelevant.o

# Target executable
TARGET = complex_program

# Build target
$(TARGET): $(OBJ)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJ)

# Compile source files to object files
main.o: main.c operation.h util.h irrelevant.h
	$(CC) $(CFLAGS) -c main.c

operation.o: operation.c operation.h
	$(CC) $(CFLAGS) -c operation.c

irrelevant.o: irrelevant.c irrelevant.h
	$(CC) $(CFLAGS) -c irrelevant.c

util.o: util.c util.h
	$(CC) $(CFLAGS) -c util.c


# Clean up object files and executable
clean:
	rm -f $(OBJ) $(TARGET)
