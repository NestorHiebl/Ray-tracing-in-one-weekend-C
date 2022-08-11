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
1. [Overview](#1.-Overview)
2. [Output an Image](#2.-Output-an-Image)
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

Whenever you start a renderer, you need a way to see an image. The most straightforward way is to write it to a file. The catch is, there are so many formats. Many of those are complex. I always start with a plain text ppm file. Here’s a nice description from Wikipedia:

![PPM format description](./img/fig1.jpg)
<div align="center"><b>Figure 1:</b> PPM Example</div>

Let’s make some C++ code to output such a thing:

```c
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
<div align="center"><b>Listing 1:</b> [main.c] Creating your first image</div>

There are some things to note in that code:
1. The pixels are written out in rows with pixels left to right.

2. The rows are written out from top to bottom.
3. By convention, each of the red/green/blue components range from 0.0 to 1.0. We will relax that later when we internally use high dynamic range, but before output we will tone map to the zero to one range, so this code won’t change.
4. Red goes from fully off (black) to fully on (bright red) from left to right, and green goes from black at the bottom to fully on at the top. Red and green together make yellow so we should expect the upper right corner to be yellow.