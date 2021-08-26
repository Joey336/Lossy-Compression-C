#include "RGBconvert.h"
#include "mem.h"

Uarray2 compress_RGBtoCV(Uarray2 arr){
    Uarray2 CVarray = Uarray2_new(sizeof(struct float_CV *),Uarray2_width(arr), Uarray2_height(arr));
    Uarray2_map_row_major(arr, RGB_to_CV_map, &CVarray);
    //Uarray2_map_row_major(CVarray, printRGB_float2, NULL);
    return CVarray;
}


Uarray2 decompress_CVtoRGB(Uarray2 arr){
    Uarray2 RGBarray = Uarray2_new(sizeof(struct float_RGB *),Uarray2_width(arr), Uarray2_height(arr));
    Uarray2_map_row_major(arr, CV_to_RGB_map, &RGBarray);
    //Uarray2_map_row_major(RGBarray, printRGB_float2, NULL);
    return RGBarray;
}


void RGB_to_CV_map(int width, int height, Uarray2 arr, void* elem, void* cl){
    float_RGB fRGB = *(float_RGB *)elem;
    float_CV CVarr;
    NEW(CVarr);
    CVarr->y    = 0.299 *       fRGB->R + 0.587 *       fRGB->G + 0.114 *       fRGB->B;
    CVarr->pb   = -0.168736 *   fRGB->R - 0.331264 *    fRGB->G + 0.5 *         fRGB->B;
    CVarr->pr   = 0.5 *         fRGB->R - 0.418688 *    fRGB->G - 0.081312 *    fRGB->B;
    CVarr->denominator = fRGB->denominator;
    //printf("COMPRESS: %f   %f    %f  \n", fRGB->R, fRGB->G , fRGB->B );
    float_CV* arrayPos = array_at(*(Uarray2 *)cl, width, height);
    *arrayPos = CVarr;
    (void)arr;
}

void CV_to_RGB_map(int width, int height, Uarray2 arr, void* elem, void* cl){
    float_CV CVarr = *(float_CV *)elem;
    float_RGB RGBarr;
    NEW(RGBarr);
    RGBarr->R    = (1.0 * CVarr->y) + 0.0 * CVarr->pb + 1.402 * CVarr->pr;
    RGBarr->G   = 1.0 * CVarr->y - 0.344136 * CVarr->pb - 0.714136 * CVarr->pr;
    RGBarr->B   = 1.0 * CVarr->y + 1.772 * CVarr->pb + 0.0 * CVarr->pr;
    //printf("DECOMPRESS: %f   %f    %f  \n", RGBarr->R, RGBarr->G , RGBarr->B );
    RGBarr->denominator = CVarr->denominator;
    float_RGB* arrayPos = array_at(*(Uarray2 *)cl, width, height);
    *arrayPos = RGBarr;
    //printf("_______%f______\n", (*arrayPos)->R);
    (void)arr;
}


void printRGB_float2(int width, int height, Uarray2 value, void *elem, void *cl){
    float_RGB RGB = *(float_RGB *)elem;
    printf("%f ", RGB->R);
    printf("%f ", RGB->G);
    printf("%f   ", RGB->B);
    if(width == Uarray2_width(value) - 1){
        printf("\n");
    }
    (void)height;
    (void)value;
    (void)cl;
}


