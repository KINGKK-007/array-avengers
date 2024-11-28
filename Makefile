CC = gcc
CFLAGS = -Wall -arch arm64  # Add any other flags you need
OBJ = main.o user.o global_user.o corp.o prof.o edu_aca.o com_cul.o personalCeleb.o health.o  # Include all object files

TARGET = program

# Link object files into the final executable
$(TARGET): $(OBJ)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJ)

# Compile each .c file into its corresponding .o file
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Clean up object files and target
clean:
	rm -f $(OBJ) $(TARGET)
