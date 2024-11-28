# # Variables
# CC = gcc
# CFLAGS = -w
# LDFLAGS = -o program

# # Targets
# program: main.o user.o health.o personalCeleb.o com_cul.o edu_aca.o prof.o corp.o
# 	$(CC) main.o user.o health.o personalCeleb.o com_cul.o edu_aca.o prof.o corp.c $(LDFLAGS)

# main.o: main.c user.h global_user.h
# 	$(CC) $(CFLAGS) main.c

# global_user.o: global_user.c global_user.h
# 	$(CC) $(CFLAGS) global_user.c

# user.o: user.c user.h health.h global_user.h
# 	$(CC) $(CFLAGS) user.c

# health.o: health.c health.h global_user.h
# 	$(CC) $(CFLAGS) health.c

# personalCeleb.o: personalCeleb.c personalCeleb.h global_user.h
# 	$(CC) $(CFLAGS) personalCeleb.c

# com_cul.o: com_cul.c com_cul.h global_user.h
# 	$(CC) $(CFLAGS) com_cul.c

# edu_aca.o: edu_aca.c edu_aca.h global_user.h
# 	$(CC) $(CFLAGS) edu_aca.c

# prof.o: prof.c prof.h global_user.h
# 	$(CC) $(CFLAGS) prof.c

# corp.o: corp.c corp.h global_user.h
# 	$(CC) $(CFLAGS) corp.c

# clean:
# 	rm -f *.o program
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
