#include <stdio.h>
#include <stdlib.h>
#include "mem.h"
#include "readPnm.h"


Uarray2 readpng(FILE *input);
void printRGB(int width, int height, Uarray2 value, void *elem, void *cl);


Uarray2 readFile(FILE *file){
    //Pnm_ppmread(file, )
    Uarray2 arr = readpng(file);
    return arr;
}


Uarray2 readpng(FILE *input){
        //input = fopen(input, "rb");
        if(!input){
            //perror(input);
            exit(1);
        }

    Pnmrdr_T r = Pnmrdr_new(input);
    
    Pnmrdr_mapdata data = Pnmrdr_data(r);
    
    int maxh = 0, maxw = 0;
    //printf("width: %d \nheight: %d\n", data.width, data.height);
    //printf("w%d, h%d\n", data.width, data.height);
    maxw = data.width;
    maxh = data.height;

    if (data.width % 2 == 1){
        maxw-=1;
    }
    if (data.height % 2 == 1){
        maxh-=1;
    }
    

    Uarray2 testArray = Uarray2_new(sizeof(struct unsigned_RGB *), maxw, maxh);
    
    for(unsigned int i = 0; i < data.height; i++){
        for(unsigned int j = 0; j < data.width; j++){
            
            if((int)i < maxh && (int)j< maxw){
                //printf("WIDTH: %d\n", j);
                unsigned_RGB RGB;
                NEW(RGB);
                RGB->denominator = data.denominator;
                RGB->R = Pnmrdr_get(r);
                RGB->G = Pnmrdr_get(r);
                RGB->B = Pnmrdr_get(r);
                //printf("%d HI\n",RGB->B.type);
                //printf("%d %d %d\n",RGB->R, RGB->G, RGB->B );
                unsigned_RGB* arrayPos = array_at(testArray, j, i);
                *arrayPos = RGB;
            }
             else{
                 //printf("I: %d   maxH: %d  J: %d   maxW: %d\n",i, maxh, j, maxw);
                Pnmrdr_get(r);
                Pnmrdr_get(r);
                Pnmrdr_get(r);
            }
            
        }

    }
    //printf("hi\n");
    return testArray;
}

Uarray2 compress_to_float(Uarray2 RGB_data){
    Uarray2 CLArray = Uarray2_new(sizeof(struct float_RGB *), Uarray2_width(RGB_data), Uarray2_height(RGB_data));
    Uarray2_map_row_major(RGB_data, compress_floats_map, &CLArray);
    return CLArray;
}

Uarray2 decompress_to_int(Uarray2 RGB_data){
     Uarray2 CLArray = Uarray2_new(sizeof(struct unsigned_RGB *), Uarray2_width(RGB_data), Uarray2_height(RGB_data));
    Uarray2_map_row_major(RGB_data, decompress_int_map, &CLArray);
    return CLArray;
}

void compress_floats_map(int width, int height, Uarray2 value, void *elem, void *cl){
    //Uarray2 *fRGB = (Uarray2 *)cl;
    unsigned_RGB uRGB = *(unsigned_RGB *)elem;
    float_RGB f_RGB;
    NEW(f_RGB);
    f_RGB->R = (float)uRGB->R / uRGB->denominator;
    f_RGB->G  = (float)uRGB->G / uRGB->denominator;
    f_RGB->B = (float)uRGB->B / uRGB->denominator;
    f_RGB->denominator = uRGB->denominator;
    float_RGB* arrayPos = array_at(*(Uarray2 *)cl, width, height);
    *arrayPos = f_RGB;
    
    (void) width;
    (void) height;
    (void) value;
}

int num = 0;
void decompress_int_map(int width, int height, Uarray2 value, void *elem, void *cl){
    //Uarray2 *fRGB = (Uarray2 *)cl;
    
    float_RGB fRGB = *(float_RGB *)elem;
    unsigned_RGB u_RGB;
    NEW(u_RGB);
    unsigned R_ =  (unsigned)(fRGB->R * fRGB->denominator);
    unsigned G_ =  (unsigned)(fRGB->G * fRGB->denominator);
    unsigned B_ =  (unsigned)(fRGB->B * fRGB->denominator);

    if(R_ >= fRGB->denominator){
        u_RGB->R = fRGB->denominator;
    }else{
        u_RGB->R = R_;
    }
    if(G_ >= fRGB->denominator){
        u_RGB->G = fRGB->denominator;
    }else{
        u_RGB->G = G_;
    }
    if(B_ >= fRGB->denominator){
        u_RGB->B = fRGB->denominator;
    }else{
        u_RGB->B = B_;
    }
    
    u_RGB->denominator = fRGB->denominator;
    unsigned_RGB* arrayPos = array_at(*(Uarray2 *)cl, width, height);
    *arrayPos = u_RGB;
   // printf("%d, %d, %d\n", u_RGB->R, u_RGB->G, u_RGB->B);
    (void) width;
    (void) height;
    (void) value;
}



void printRGB_unsigned(int width, int height, Uarray2 value, void *elem, void *cl){
    unsigned_RGB RGB = *(unsigned_RGB *)elem;
    printf("%d ", RGB->R);
    printf("%d ", RGB->G);
    printf("%d   ", RGB->B);
    if(width == 23){
        printf("\n");
    }
    (void)height;
    (void)value;
    (void)cl;
}

void printRGB_float(int width, int height, Uarray2 value, void *elem, void *cl){
    float_RGB RGB = *(float_RGB *)elem;
    printf("%f ", RGB->R);
    printf("%f ", RGB->G);
    printf("%f   ", RGB->B);
    if(width == 22){
        printf("\n");
    }
    (void)height;
    (void)value;
    (void)cl;
}


