#include <stdio.h>
#include <stdlib.h>
#include "uarray2.h"
#include <stdint.h>
#include "bitpack_Caller.h"


bool Bitpack_fitsu(uint64_t n, unsigned width);

bool Bitpack_fitss( int64_t n, unsigned width);


Uarray2 pack(Uarray2 arith_411){

    Uarray2 CLarr = Uarray2_new(sizeof(uint64_t), Uarray2_width(arith_411), Uarray2_height(arith_411));


    Uarray2_map_row_major(arith_411, pack_map, &CLarr);

    return CLarr;

}



Uarray2 unpack(Uarray2 wordArray){
    Uarray2 CLarr = Uarray2_new(sizeof(struct arith_411 *), Uarray2_width(wordArray), Uarray2_height(wordArray));

    //printf("here\n");
    Uarray2_map_row_major(wordArray, unpack_map, &CLarr);

    return CLarr;
}


void pack_map(int width, int height, Uarray2 value, void *elem, void *cl){

    arith_411 teststruct = *(arith_411 *)elem;
    uint64_t word = 0;


    uint64_t newWord = Bitpack_newu(word,  9, 23, (uint64_t)teststruct->a);
    //print_binary2(newWord);

    uint64_t newWord_b = Bitpack_news(newWord, 5, 18, (uint64_t)teststruct->b);
    //print_binary2(newWord_b);

    uint64_t newWord_c = Bitpack_news(newWord_b, 5, 13, (uint64_t)teststruct->c);
    //print_binary2(newWord_c);

    uint64_t newWord_d = Bitpack_news(newWord_c, 5, 8, (uint64_t)teststruct->d);
    //print_binary2(newWord_d);

    uint64_t newWord_pb = Bitpack_newu(newWord_d,  4, 4, (uint64_t)teststruct->pb);
    //print_binary2(newWord_pb);

    uint64_t newWord_pr = Bitpack_newu(newWord_pb,  4, 0, (uint64_t)teststruct->pr);
    //print_binary2(newWord_pr);

    uint64_t*clarr = array_at(*(Uarray2 *)cl, width, height);
    *clarr = newWord_pr;
    (void)width;
    (void)height;
    (void)value;    
    (void)cl;
}


void unpack_map(int width, int height, Uarray2 value, void *elem, void *cl){
    uint64_t word = *(uint64_t *)elem;
    uint64_t unpackedA = Bitpack_getu(word, 9, 23);
    int64_t unpackedB = Bitpack_gets(word, 5, 18);
    int64_t unpackedC = Bitpack_gets(word, 5, 13);
    int64_t unpackedD = Bitpack_gets(word, 5, 8);
    int64_t unpackedpb = Bitpack_getu(word, 4, 4);
    int64_t unpackedpr = Bitpack_getu(word, 4, 0);
    arith_411 struct411;
    NEW(struct411);

    struct411->a = (unsigned)unpackedA;
    struct411->b = (int)unpackedB;
    struct411->c = (int)unpackedC;
    struct411->d = (int)unpackedD;
    struct411->pb = (unsigned)unpackedpb;
    struct411->pr = (unsigned)unpackedpr;
    struct411->denominator = 255;
    
    arith_411* arrayPos = array_at(*(Uarray2 *)cl, width, height);
    *arrayPos = struct411;
   // printf("%d\n\n", (*arrayPos)->a);
    (void)value;

}
