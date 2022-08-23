CC=gcc
CFLAGS=-Werror -Wextra -pedantic
EXECUTABLE=raytracer

ifeq ($(OS), Windows_NT) 
RM = del
else
RM = rm
endif

$(EXECUTABLE): main.o vec3.o color.o ray.o camera.o
	$(CC) -o $(EXECUTABLE) $(CFLAGS) main.o vec3.o color.o ray.o camera.o

main.o: main.c utils.h
	$(CC) -o main.o -c $(CFLAGS) main.c

vec3.o: vec3/vec3.c vec3/vec3.h
	$(CC) -o vec3.o -c $(CFLAGS) vec3/vec3.c

color.o: color/color.c color/color.h
	$(CC) -o color.o -c $(CFLAGS) color/color.c

ray.o: ray/ray.c ray/ray.h
	$(CC) -o ray.o -c $(CFLAGS) ray/ray.c

camera.o: camera/camera.c camera/camera.h
	$(CC) -o camera.o -c $(CFLAGS) camera/camera.c

.PHONY: clean
clean:
	$(RM) *.o *.ppm $(EXECUTABLE) *.exe
