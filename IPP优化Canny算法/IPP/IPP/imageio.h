#pragma once
/*
FAST-EDGE
Copyright (c) 2009 Benjamin C. Haynor

Permission is hereby granted, free of charge, to any person
obtaining a copy of this software and associated documentation
files (the "Software"), to deal in the Software without
restriction, including without limitation the rights to use,
copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the
Software is furnished to do so, subject to the following
conditions:

The above copyright notice and this permission notice shall be
included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
OTHER DEALINGS IN THE SOFTWARE.
*/

#include <stdio.h>
#include<Windows.h>
#include "imageio.h"
#include "ipp.h"
#ifndef _IMAGEIO
#define _IMAGEIO
#define WIDTH  512  /* image width */
#define HEIGHT 512 /* image height */

struct image {
	int width;
	int height;
	unsigned char * pixel_data;
};

void write_pgm_image(Ipp8u * img);
int read_pgm_hdr(FILE *fp, int *w, int *h);
int skipcomment(FILE *fp);

#endif
