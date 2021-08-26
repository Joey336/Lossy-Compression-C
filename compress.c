#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "ppmdiff.h"
#include "compress.h"
#include "readPnm.h"
#include "uarray2.h"
#include "RGBconvert.h"
#include "arithSub.h"
#include "bitpack_Caller.h"
void print_ppm(Uarray2 arr);
void write_compress(Uarray2 wordArray, FILE *file);
Uarray2 read_compress(FILE *file);
void print_binary123(unsigned n);

extern void compress(FILE *input){

    //step 1:
    //FILE -> FLOAT_ARRAY
    Uarray2 pnm_data = readFile(input);
    //Uarray2_map_row_major(pnm_data, printRGB_unsigned ,NULL);
    Uarray2 fArray = compress_to_float(pnm_data);
    //Uarray2_map_row_major(fArray, printRGB_float ,NULL);
    //test  individual decompress(comment out if going further than step 1)
    //Uarray2 fArrayd = decompress_to_int(fArray);
    //(void) fArrayd;



    //step2: 
    //FLOAT_ARRAY -> CV_ARRAY
    Uarray2 CVarray = compress_RGBtoCV(fArray);
    //test individual decompress(comment out if going further than step 2)
    //Uarray2 CVarrayd = decompress_CVtoRGB(CVarray);
    //(void)CVarrayd;
   


    //step 3:
    //CV_array to 411Array
    Uarray2 array411 = compress_to_411(CVarray);

    Uarray2 wordCompress = pack(array411);
    write_compress(wordCompress, stdout);

    (void)CVarray;
    (void)pnm_data;
    
  
} 

extern void decompress(FILE *input){




  Uarray2 uarr = read_compress(input);
  
    Uarray2 wordDecompress = unpack(uarr);

    Uarray2 array411D = decompress_to_CV(wordDecompress);
    Uarray2 rgbArray = decompress_CVtoRGB(array411D);
    Uarray2 arrayInt = decompress_to_int(rgbArray);
    print_ppm(arrayInt);
    
    //Uarray2 fArrayd = decompress_to_int(CVarrayd);
    //Uarray2_map_row_major(CVarray, printRGB_float2 ,NULL);
    //Uarray2_map_row_major(fArrayd, printRGB_unsigned ,NULL);

  
    //******FULL DECOMPRESS TEST*******
    //STEP 2 -> STEP 1
   // Uarray2 CVarrayd = decompress_CVtoRGB(array411D);
    //(void)CVarrayd;
  
    //STEP 1 -> FLOAT ARRAY
    //Uarray2 fArrayd = decompress_to_int(CVarrayd);
    //(void) fArrayd;

   //ppmDiff(pnm_data, fArrayd);
   //print_ppm(fArrayd);
     (void) input;

} // reads a compressed image and writes PPM



void print_ppm(Uarray2 arr){
  const int dimx = Uarray2_width(arr), dimy = Uarray2_height(arr);
  int i, j;
  FILE *fp = stdout; /* b - binary mode */
  (void) fprintf(fp, "P6\n%d %d\n255\n", dimx, dimy);
  
  for (j = 0; j < dimy; ++j)
  {
    for (i = 0; i < dimx; ++i)
    {
     unsigned_RGB* arrayPos = array_at(arr, i, j);
      static unsigned char color[3];
      //printf("%d\n", (*arrayPos)->R);
      color[0] = (*arrayPos)->R % 256;  /* red */
      color[1] = (*arrayPos)->G % 256;  /* green */
      color[2] = (*arrayPos)->B % 256;  /* blue */
    //   color[0] = i % 256;  /* red */
    //   color[1] = j % 256;  /* green */
    //   color[2] = (i * j) % 256;  /* blue */
      (void) fwrite(color, 1, 3, fp);
    }
  }
  (void) fclose(fp);
}
void print_binary123(unsigned n)
{
    unsigned i;
    int counter = 0;
    for (i = 1 << 31; i > 0; i = i / 2){
        counter ++;
        
        (n & i) ? printf("1") : printf("0");
        
    }

}


void write_compress(Uarray2 wordArray, FILE *file){
  (void)file;
  printf("Compressed image format 2\n%u %u\n", (unsigned)Uarray2_width(wordArray) * 2, (unsigned)Uarray2_height(wordArray) * 2);
  int height = Uarray2_height(wordArray);
  int width = Uarray2_width(wordArray);
  for (int i = 0; i < height; i++){
    for(int j = 0; j < width; j++){
       uint64_t elem = *(uint64_t*) (array_at(wordArray, j, i));
       char byte4 = (elem >> 24);
       char byte3 = (elem >> 16);
       char byte2 = (elem >> 8);
       char byte1 = (elem);
       putchar(byte4);
       putchar(byte3);
       putchar(byte2);
       putchar(byte1);

    }
  }
  
}
 
 Uarray2 read_compress(FILE* fname){
  unsigned height, width;
  assert(fname);
  int read = fscanf(fname, "Compressed image format 2\n%u %u", &width, &height);
  fflush(stdout);
  assert(read == 2);
  int c = getc(fname);
  assert(c =='\n');
  
  Uarray2 CLarr = Uarray2_new(sizeof(uint64_t), width / 2, height / 2);
  for (unsigned i = 0; i < height / 2; i++){
    for(unsigned j = 0; j < width / 2; j++){
       uint64_t elem4 = getc(fname);
       unsigned byte4 = (elem4 << 24);
       uint64_t elem3 = getc(fname);
       unsigned byte3 = (elem3 << 16);
       uint64_t elem2 = getc(fname);
       unsigned byte2 = (elem2 << 8);
       uint64_t elem = getc(fname);
       unsigned byte1 = (elem);
       unsigned bytefinal = byte4 | byte3 | byte2 | byte1;
       uint64_t *CLelem = (uint64_t*) (array_at(CLarr, j, i));
       *CLelem = bytefinal;
    }
  }
return CLarr;
} 