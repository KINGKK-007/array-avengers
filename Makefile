program: main.o user.o health.o
	gcc main.c user.c health.c -o program

main.o: main.c user.h
	gcc -c main.c

user.o: user.c user.h
	gcc -c user.c

health.o: health.c health.h
	gcc -c health.c

clean:
	rm -f *.o program
