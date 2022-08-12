# What is this?
The following is a verbatim copy of Peter Shirley's **Ray Tracing in One Weekend** book (version 3.2.3, 2020-12-07), with the following changes:
 - The language has been changed from C++ to C.
 - All paragraphs expounding upon language-specific properties of the code have been re-written in order to reflect the re-written code in C.
 - Instead of writing image data to *stdout*, the programs in this repo take an additional command line argument specifying a filename. When the programs are run, image data is written to this file. *stdout* is used purely for logging.

 The original book in C++ can be read for free [here](https://raytracing.github.io/books/RayTracingInOneWeekend.html), or can be bought [here](https://www.amazon.de/Tracing-Weekend-Minibooks-Book-English-ebook/dp/B01B5AODD8).

## General remarks
The state of the code as it is in a specific chapter in the book can be found in the directory labeled with the corresponding section number.

# Ray Tracing in One Weekend

[Peter Shirley](https://github.com/petershirley), edited by [Steve Hollasch](https://github.com/hollasch) and [Trevor David Black](https://github.com/trevordblack)

Version 3.2.3, 2020-12-07 (Modified)

Copyright 2018-2020 Peter Shirley. All rights reserved.

## Contents
1. [Overview](#1.-overview)
2. [Output an Image](#2.-output-an-image)
    1. The PPM Image format
    2. Creating an Image File
    3. Adding a Progress Indicator

## 1. Overview
**Note: The following section has been left unchanged as it expresses the author's reasoning for decisions that impacted the unmodified version of the text. The section concerning C++ does not apply to this, modified version of the book but the expressed ideas are still benefiecial to the reader.**

 I’ve taught many graphics classes over the years. Often I do them in ray tracing, because you are forced to write all the code, but you can still get cool images with no API. I decided to adapt my course notes into a how-to, to get you to a cool program as quickly as possible. It will not be a full-featured ray tracer, but it does have the indirect lighting which has made ray tracing a staple in movies. Follow these steps, and the architecture of the ray tracer you produce will be good for extending to a more extensive ray tracer if you get excited and want to pursue that.

When somebody says “ray tracing” it could mean many things. What I am going to describe is technically a path tracer, and a fairly general one. While the code will be pretty simple (let the computer do the work!) I think you’ll be very happy with the images you can make.

I’ll take you through writing a ray tracer in the order I do it, along with some debugging tips. By the end, you will have a ray tracer that produces some great images. You should be able to do this in a weekend. If you take longer, don’t worry about it. I use C++ as the driving language, but you don’t need to. However, I suggest you do, because it’s fast, portable, and most production movie and video game renderers are written in C++. Note that I avoid most “modern features” of C++, but inheritance and operator overloading are too useful for ray tracers to pass on. I do not provide the code online, but the code is real and I show all of it except for a few straightforward operators in the vec3 class. I am a big believer in typing in code to learn it, but when code is available I use it, so I only practice what I preach when the code is not available. So don’t ask!

I have left that last part in because it is funny what a 180 I have done. Several readers ended up with subtle errors that were helped when we compared code. So please do type in the code, but if you want to look at mine it is at:

https://github.com/RayTracing/raytracing.github.io/

I assume a little bit of familiarity with vectors (like dot product and vector addition). If you don’t know that, do a little review. If you need that review, or to learn it for the first time, check out Marschner’s and my graphics text, Foley, Van Dam, et al., or McGuire’s graphics codex.

If you run into trouble, or do something cool you’d like to show somebody, send me some email at ptrshrl@gmail.com.

I’ll be maintaining a site related to the book including further reading and links to resources at a blog https://in1weekend.blogspot.com/ related to this book.

Thanks to everyone who lent a hand on this project. You can find them in the acknowledgments section at the end of this book.

Let’s get on with it!

## 2. Output an Image
### 2.1. The PPM Image Format

Whenever you start a renderer, you need a way to see an image. The most straightforward way is to write it to a file. The catch is, there are so many formats. Many of those are complex. I always start with a plain text ppm file. Here’s a nice description from Wikipedia:

![PPM format description](./img/fig1.jpg)
<div align="center"><b>Figure 1:</b> PPM Example</div><br/>

Let’s make some C code to output such a thing:

```c
#include <stdio.h>

#define IMG_WIDTH 256
#define IMG_HEIGHT 256

int main(int argc, char *argv[]) {
    FILE *output_file = fopen("out.ppm", "w");

    if (output_file == NULL) {
        fprintf(stderr, "Could not open file\n");
        exit(1);
    }

    fprintf(output_file, "P3\n%d %d\n255\n", IMG_WIDTH, IMG_HEIGHT);

    for (int j = IMG_HEIGHT - 1; j >= 0; j--) {
        for (int i = 0; i < IMG_WIDTH; i++) {
            double r = ((double) i / (IMG_WIDTH - 1));
            double g = ((double) j / (IMG_HEIGHT - 1));
            double b = 0.25;

            int ir = (int) (255.999 * r);
            int ig = (int) (255.999 * g);
            int ib = (int) (255.999 * b);

            fprintf(output_file, "%d %d %d\n", ir, ig, ib);
        }
    }

    fflush(output_file);
    fclose(output_file);
}
```
<div align="center"><b>Listing 1:</b> [main.c] Creating your first image</div><br/>

There are some things to note in that code:
1. The pixels are written out in rows with pixels left to right.
2. The rows are written out from top to bottom.
3. By convention, each of the red/green/blue components range from 0.0 to 1.0. We will relax that later when we internally use high dynamic range, but before output we will tone map to the zero to one range, so this code won’t change.
4. Red goes from fully off (black) to fully on (bright red) from left to right, and green goes from black at the bottom to fully on at the top. Red and green together make yellow so we should expect the upper right corner to be yellow.

### 2.2. Creating an Image File

Since the image data isn't written to *stdout* we cannot use output redirection to create an image file. The program in **Listing 1** is hard-coded to always create (or overwrite) a file called `out.ppm`, but we may want the option to give our images custom names. In the following section, we'll make it so that a filename can be specified via command line argument, as so:

`raytracer [FILE]`  
`Where FILE is a filename ending with .ppm`

We always want our output file to end with .ppm so it makes sense to write a simple function for input validation. Let's create a new file called `utils.h` and populate it as so:

```c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void print_usage();
int validate_filename(const char *filename);

void print_usage() {
    printf( "Usage:\n\t"
            "raytracer [FILE]\n\t"
            "Where FILE is a filename ending with .ppm\n"
          );
}

int validate_filename(const char *filename) {
    if (filename == NULL) {
        return 0;
    }

    size_t len = strlen(filename);
    if ((len > FILENAME_MAX) || (len < 5)) {
        return 0;
    }

    if (strcmp(&filename[len - 4 /* Length of .ppm extension */], ".ppm") == 0) {
        return 1;
    }

    return 0;
}
```
<div align="center"><b>Listing 1a:</b> [utils.h] Some simple utility functions</div><br/>

The `validate_input` function is rudimentary but will do for now. All it does is check that the given filename is a string ending with but not consisting entirely of the characters ".ppm" and that it is not too long for the operating system. `print_usage` can be output in case of error to let the user know what kind of input is expected of them. When we insert the above functions into our code we get this:

```c
#include <stdio.h>
#include <errno.h>
#include "utils.h"

#define IMG_WIDTH 256
#define IMG_HEIGHT 256

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Invalid or no arguments supplied. See usage below:\n");
        print_usage();
        exit(1);
    }

    if (!validate_filename(argv[1])) {
        fprintf(stderr, "Invalid filename argument supplied. See usage below:\n");
        print_usage();
        exit(1);
    }

    FILE *output_file = fopen(argv[1], "w");

    if (output_file == NULL) {
        fprintf(stderr, "Could not open file %s\n", argv[1]);
        perror(NULL);
        exit(1);
    }

    fprintf(output_file, "P3\n%d %d\n255\n", IMG_WIDTH, IMG_HEIGHT);

    for (int j = IMG_HEIGHT - 1; j >= 0; j--) {
        for (int i = 0; i < IMG_WIDTH; i++) {
            double r = ((double) i / (IMG_WIDTH - 1));
            double g = ((double) j / (IMG_HEIGHT - 1));
            double b = 0.25;

            int ir = (int) (255.999 * r);
            int ig = (int) (255.999 * g);
            int ib = (int) (255.999 * b);

            fprintf(output_file, "%d %d %d\n", ir, ig, ib);
        }
    }

    fflush(output_file);
    fclose(output_file);
}
```
<div align="center"><b>Listing 1b:</b> [main.c] Specifying the output filename</div><br/>

Now that we have two separate code files (with many more to come) it makes sense to include a build system into the project. Since this is meant to be a book focusing on ray tracing, I will only give a quick and cursory account of a specific build system, **Makefile**. In case you're already familiar with it or just not interested, feel free to skip this section. If you would like to learn more about makefiles, I recommend [this website](https://makefiletutorial.com/).

Create a new file in the project's root directory called `Makefile`. Makefiles are used to specify and track dependencies between files. With one set up, we can simply run `make` from the command line and it will automatically check for changes in the source code, recompiling executables and object files as needed.

Let's insert the following into our makefile:

```Makefile
CC=gcc # Or replace gcc with whichever compiler you're using
CFLAGS=-Werror -Wextra -pedantic
EXECUTABLE=raytracer

$(EXECUTABLE): main.c utils.h
	$(CC) -o $(EXECUTABLE) $(CFLAGS) main.c
```

The above expresses that our executable file (`raytracer`) depends on the source files `main.c` and `utils.h`. As a result, whenever we run `make` after changing either of these files the build system will recognize that the executable is out of date and run the chain of commands specified in the indented line under `$(EXECUTABLE): main.c utils.h`.

The makefile will be refined further as the project becomes more complex, but I will not spend any more time on explaining its inner mechanics. If you're interested on seeing its state at a specific stage in the project's development, check its specific subsection directory. For a deeper read on the topic or if you want to look up a specific feature I recommend reading up [here](https://makefiletutorial.com/). 

Now all you need to do is run the command `make`. Your shell should output something along the lines of `gcc -o raytracer -Werror -Wextra -pedantic main.c` and you should find a new file called `raytracer` in your project's directory. If this is the case, run `raytracer image.ppm` and open the resulting file in the image viewer of your choice. This is the result:

![PPM format description](./img/img1.png)
<div align="center"><b>Figure 1:</b> First PPM image</div><br/>

Hooray! This is the graphics “hello world”. If your image doesn’t look like that, open the output file in a text editor and see what it looks like. It should start something like this:

```
P3
256 256
255
0 255 63
1 255 63
2 255 63
3 255 63
4 255 63
5 255 63
6 255 63
7 255 63
8 255 63
9 255 63
...
```
<div align="center"><b>Listing 2:</b> First image output</div><br/>

If it doesn’t, then you probably just have some newlines or something similar that is confusing the image reader. 

If you want to produce more image types than PPM, I am a fan of stb_image.h, a header-only image library available on GitHub at https://github.com/nothings/stb.

### 2.3. Adding a Progress Indicator

Before we continue, let's add a progress indicator to our output. This is a handy way to track the progress of a long render, and also to possibly identify a run that's stalled out due to an infinite loop or other problem.

```c
    for (int j = IMG_HEIGHT - 1; j >= 0; j--) {
        printf("\rScanlines remaining: %d ", j);
        for (int i = 0; i < IMG_WIDTH; i++) {
            double r = ((double) i / (IMG_WIDTH - 1));
            double g = ((double) j / (IMG_HEIGHT - 1));
            double b = 0.25;

            int ir = (int) (255.999 * r);
            int ig = (int) (255.999 * g);
            int ib = (int) (255.999 * b);

            fprintf(output_file, "%d %d %d\n", ir, ig, ib);
        }
    }

    printf("\nDone.\n");
```
<div align="center"><b>Listing 3:</b> [main.c] Main render loop with progress reporting</div><br/>

## 3. The vec3 Data Structure