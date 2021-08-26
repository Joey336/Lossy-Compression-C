#ifndef CV_INCLUDED
#define CV_INCLUDED


#include "uarray2.h"
#include "readPnm.h"


typedef struct float_CV{
    float y, pb, pr;
    unsigned denominator;
} *float_CV;


Uarray2 compress_RGBtoCV(Uarray2 arr);
Uarray2 decompress_CVtoRGB(Uarray2 arr);


void RGB_to_CV_map(int r, int c, Uarray2 arr, void* elem, void* cl);
void CV_to_RGB_map(int r, int c, Uarray2 arr, void* elem, void* cl);

void printRGB_float2(int width, int height, Uarray2 value, void *elem, void *cl);
#endif