#include "arithSub.h"
#include "uarray2.h"
#include "bitpack.h"





//1. pack struct
//2. append to array
//3. print to file
//4. return array
Uarray2 pack(Uarray2 arith_411);
Uarray2 unpack(Uarray2 wordArray);


void pack_map(int width, int height, Uarray2 value, void *elem, void *cl);
void unpack_map(int width, int height, Uarray2 value, void *elem, void *cl);


Uarray2 printpacked_to_file(FILE* fname);
Uarray2 read_from_packed_file(FILE* fname);