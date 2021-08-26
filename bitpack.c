#include "bitpack.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>


Except_T Bitpack_Overflow = { "Overflow packing bits" };
bool Bitpack_fitsu(uint64_t n, unsigned width){
    uint64_t shiftedN = n << (64 - width);
    shiftedN = shiftedN >>(64 - width);
    if(shiftedN == n){
        return true;
    }
    return false;
}
bool Bitpack_fitss( int64_t n, unsigned width){
    int64_t shiftedN = n << (64 - width);
    shiftedN = (int64_t)shiftedN >> (64 - width);
    if(shiftedN == n){
        return true;
    }
    return false;
}


void print_binary(unsigned n)
{
    unsigned i;
    int counter = 0;
    for (i = 1 << 31; i > 0; i = i / 2){
        counter ++;
        
        (n & i) ? printf("1") : printf("0");
        
        if(counter % 8 == 0){
            printf(" ");
        }
    }
    printf("\n");
}

void print_binary2(uint64_t n)
{
    uint64_t i;
    int counter = 0;
    for (i = (uint64_t)1 << 63; i > 0; i = i / 2){
        counter ++;
        
        (n & i) ? printf("1") : printf("0");
        
        if(counter % 8 == 0){
            printf(" ");
        }
    }
    printf("\n");
}

uint64_t Bitpack_newu(uint64_t word, unsigned width, unsigned lsb, uint64_t unsignedvalue){
 
    unsignedvalue = unsignedvalue << lsb;
    word = word | unsignedvalue;

    (void) width;

    return word;

}

 uint64_t Bitpack_news(uint64_t word, unsigned width, unsigned lsb,  int64_t signedvalue){
    


    uint64_t newSignedValue = signedvalue << (64 - width);
    newSignedValue = (uint64_t)newSignedValue >> (64 - width);

    word = Bitpack_newu(word, width, lsb, newSignedValue);

    (void)width;
    (void)lsb;


    return word;
 }

uint64_t Bitpack_getu(uint64_t word, unsigned width, unsigned lsb){


    uint64_t rightShiftWord = word << (64 - width - lsb);
    uint64_t leftShiftWord = rightShiftWord >> (64 - width);


    return leftShiftWord;
    //     __(shift left by)16bits - width - lsb
    //    |   _______(shift right by)16bits - width
    //     ___________what we want
    //    |   |
    // 11011111 01001111
    // 00000000 00011111
}


int64_t Bitpack_gets(uint64_t word, unsigned width, unsigned lsb){

    int64_t rightShiftWord = word << (64 - width - lsb);
    int64_t leftShiftWord = (int64_t)rightShiftWord >> (64 - width);

    return (int64_t)leftShiftWord;
    //     __(shift left by)16bits - width - lsb
    //    |   _______(shift right by)16bits - width
    //     ___________what we want
    //    |   |
    // 11011111 01001111
    // 00000000 00011111
}