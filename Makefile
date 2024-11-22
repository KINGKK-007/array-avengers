program: main.o user.o health.o
	gcc main.c user.c health.c personalCeleb.c -o program

main.o: main.c user.h
	gcc -c main.c

user.o: user.c user.h
	gcc -c user.c

health.o: health.c health.h
	gcc -c health.c

personalCeleb.o:  personalCeleb.c personalCeleb.h
	gcc -c personalCeleb.c

clean:
	rm -f *.o program
