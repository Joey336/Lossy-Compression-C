#ifndef BITPACK_INCLUDED
#define BITPACK_INCLUDED
#include <stdbool.h>
#include <stdint.h>
#include "except.h"


//1
bool Bitpack_fitsu(uint64_t n, unsigned width);
bool Bitpack_fitss( int64_t n, unsigned width);


//3 unpack values 
uint64_t Bitpack_getu(uint64_t word, unsigned width, unsigned lsb);
 int64_t Bitpack_gets(uint64_t word, unsigned width, unsigned lsb);



//2 pack values
uint64_t Bitpack_newu(uint64_t word, unsigned width, unsigned lsb, uint64_t unsignedvalue);
uint64_t Bitpack_news(uint64_t word, unsigned width, unsigned lsb,  int64_t signedvalue);

//helper functions
void print_binary(unsigned n);
void print_binary2(uint64_t n);
extern Except_T Bitpack_Overflow;
#endif

//11111111 11111111 11111111 11111111 
//11110000 00000000 00000000 00000000
//00000000 00000000 00000000 00001111