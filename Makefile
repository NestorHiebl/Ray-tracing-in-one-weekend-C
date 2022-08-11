CC=gcc
CFLAGS=-Werror -Wextra -pedantic
EXECUTABLE=raytracer

$(EXECUTABLE): main.c
	$(CC) -o $(EXECUTABLE) $(CFLAGS) main.c