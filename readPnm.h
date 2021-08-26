#ifndef F_INCLUDED
#define F_INCLUDED


#include "uarray2.h"
#include "pnm.h"
#include "pnmrdr.h"


//original data
typedef struct unsigned_RGB{
    unsigned R, G, B;
    unsigned denominator;
} *unsigned_RGB;

//after devided by the de
typedef struct float_RGB{
    float R, G, B;
    unsigned denominator;

} *float_RGB;


Uarray2 readFile(FILE *file);

Uarray2 compress_to_float(Uarray2 RGB_data);
Uarray2 decompress_to_int(Uarray2 RGB_data);

void compress_floats_map(int width, int height, Uarray2 value, void *elem, void *cl);

void decompress_int_map(int width, int height, Uarray2 value, void *elem, void *cl);
extern void printRGB_float(int width, int height, Uarray2 value, void *elem, void *cl);
extern void printRGB_unsigned(int width, int height, Uarray2 value, void *elem, void *cl);

void print_ppm(Uarray2 arr);

#endif