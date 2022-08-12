CC=gcc
CFLAGS=-Werror -Wextra -pedantic
EXECUTABLE=raytracer

$(EXECUTABLE): main.o vec3.o
	$(CC) -o $(EXECUTABLE) $(CFLAGS) main.o

main.o: main.c utils.h
	$(CC) -o main.o -c $(CFLAGS) main.c

vec3.o: vec3/vec3.c vec3/vec3.h
	$(CC) -o vec3.o -c $(CFLAGS) vec3/vec3.c

.PHONY: clean
clean:
	rm *.o *.ppm $(EXECUTABLE)