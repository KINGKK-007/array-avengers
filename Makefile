# Variables
CC = gcc
CFLAGS = -c -Wall -Wextra
LDFLAGS = -o program

# Targets
program: main.o user.o health.o personalCeleb.o
	$(CC) main.o user.o health.o personalCeleb.o com_cul.o $(LDFLAGS)

main.o: main.c user.h
	$(CC) $(CFLAGS) main.c

user.o: user.c user.h health.h
	$(CC) $(CFLAGS) user.c

health.o: health.c health.h
	$(CC) $(CFLAGS) health.c

personalCeleb.o: personalCeleb.c personalCeleb.h
	$(CC) $(CFLAGS) personalCeleb.c

com_cul.o: com_cul.c com_cul.h
	$(CC) $(CFLAGS) com_cul.c

clean:
	rm -f *.o program
