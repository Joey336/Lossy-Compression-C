#ifndef ARITH411SUB_H
#define ARITH411SUB_H

#include "mem.h"
#include "uarray2.h"
#include "arith.h"

typedef struct arith_411{
    
    int b, c, d, pb, pr;
    unsigned a, denominator;
} *arith_411;


Uarray2 compress_to_411(Uarray2 CV_data);
Uarray2 decompress_to_CV(Uarray2 data_411);

void compress_CV_411(int width, int height, Uarray2 value, void *elem, void *cl);

void decompress_411_CV(int width, int height, Uarray2 value, void *elem, void *cl);


#endif