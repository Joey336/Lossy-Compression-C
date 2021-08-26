#include "arithSub.h"
#include "arith.h"
#include "RGBconvert.h"
#include "math.h"

Uarray2 compress_to_411(Uarray2 CV_data){
    //same as before (cl array should be width/2 and height/2)
    Uarray2 Array411 = Uarray2_new(sizeof(struct arith_411 *), Uarray2_width(CV_data) / 2, Uarray2_height(CV_data) / 2);
    Uarray2_map_row_major(CV_data, compress_CV_411, &Array411);
    return Array411;
}

Uarray2 decompress_to_CV(Uarray2 data_411){
    Uarray2 CVArray = Uarray2_new(sizeof(struct float_CV *), Uarray2_width(data_411) * 2, Uarray2_height(data_411) * 2);
    Uarray2_map_row_major(data_411, decompress_411_CV, &CVArray);
    return CVArray;
}
 
void compress_CV_411(int width, int height, Uarray2 value, void *elem, void *cl){
//read 2x2 PB, PR, Y values

    if (width % 2 == 0 && height % 2 == 0){
        arith_411 arith411Struct;
        NEW(arith411Struct);


        float_CV topleft = *(float_CV *)array_at(value, width, height);
        float_CV topright = *(float_CV *)array_at(value, width + 1, height);
        float_CV bottomleft = *(float_CV *)array_at(value, width, height + 1);
        float_CV bottomright = *(float_CV *)array_at(value, width + 1, height + 1);

        int denominator = topleft->denominator;
        float y1 = topleft->y;
        float y2 = topright->y;
        float y3 = bottomleft->y;
        float y4 = bottomright->y;
        //----------------------------------c
        if(round(((y4 + y3 + y2 + y1)/4.0)  *511) > 511){
            arith411Struct->a = 511;
        }else{
            arith411Struct->a = round(((y4 + y3 + y2 + y1)/4.0)  *511);
        }
        //----------------------------------b
        if(round(((y4 + y3 - y2 - y1)/4.0)  *50) > 15){
            arith411Struct->b = 15;
        }else if(round(((y4 + y3 - y2 - y1)/4.0)  *50) < -16){
            arith411Struct->b = -16;
        }else{
            arith411Struct->b = round(((y4 + y3 - y2 - y1)/4.0)  *50);
        }
        //----------------------------------c  
        if(round(((y4 - y3 + y2 - y1)/4.0)  *50) > 15){
            arith411Struct->c = 15;
        }else if(round(((y4 - y3 + y2 - y1)/4.0)  *50) < -16){
            arith411Struct->c = -16;
        }else{
            arith411Struct->c = round(((y4 - y3 + y2 - y1)/4.0)  *50);
        }
        //----------------------------------d
        if(round(((y4 - y3 - y2 + y1)/4.0)  *50) > 15){
            arith411Struct->d = 15;
        }else if(round(((y4 - y3 - y2 + y1)/4.0)  *50) < -16){
            arith411Struct->d = -16;
        }else{
            arith411Struct->d = round(((y4 - y3 - y2 + y1)/4.0)  *50);
        }
        
        arith411Struct->denominator = denominator;

        
        float pb1 = topleft->pb;
        float pb2 = topright->pb;
        float pb3 = bottomleft->pb;
        float pb4 = bottomright->pb;

        arith411Struct->pb = Arith_index_of_chroma((pb1 + pb2 + pb3 + pb4) / 4.0);


        float pr1 = topleft->pr;
        float pr2 = topright->pr;
        float pr3 = bottomleft->pr;
        float pr4 = bottomright->pr;
        arith411Struct->pr = Arith_index_of_chroma((pr1 + pr2 + pr3 + pr4) / 4.0);


     

        arith_411* arrayPos = array_at(*(Uarray2 *)cl, width / 2, height / 2);
        *arrayPos = arith411Struct;
    }
    
    (void)elem;
    (void)cl;
    
//calculate the ab,b,c,d pg, pr with the 4 struct abouve

//create stuct size width/2 height/2 name st
//append to st(at(width/2 height/2)) = a,b,c,d, py, pd

//calculate formula in pg10 directions and create a single arith_411 struct
}

 void decompress_411_CV(int width, int height, Uarray2 value, void *elem, void *cl){
  

     arith_411 arith_elem = *(arith_411 *) elem;
     //creating 4 struct to map
     
    float_CV CVstruct_tl;
    NEW(CVstruct_tl);

    float_CV CVstruct_tr;
    NEW(CVstruct_tr);

    float_CV CVstruct_bl;
    NEW(CVstruct_bl);

    float_CV CVstruct_br;
    NEW(CVstruct_br);
    //____________________________

    //assign values

    unsigned a = arith_elem->a;
    int b = arith_elem->b;
    int c = arith_elem->c;
    int d = arith_elem->d;

  
    CVstruct_tl->y =  ((float)(a/511.0) - (float)(b/50.0) - (float)(c/50.0) + (float)(d/50.0));
    CVstruct_tr->y = ((float)(a/511.0) - (float)(b/50.0) + (float)(c/50.0) - (float)(d/50.0));
    CVstruct_bl->y = ((float)(a/511.0) + (float)(b/50.0) - (float)(c/50.0) - (float)(d/50.0));
    CVstruct_br->y = ((float)(a/511.0) + (float)(b/50.0) + (float)(c/50.0) + (float)(d/50.0));
  
    float invertPb = (float)Arith_chroma_of_index(arith_elem->pb);
    float invertPr = (float)Arith_chroma_of_index(arith_elem->pr);

    CVstruct_tl->pb =  invertPb;
    CVstruct_tr->pb =  invertPb;
    CVstruct_bl->pb =  invertPb;
    CVstruct_br->pb =  invertPb;

    CVstruct_tl->pr =  invertPr;
    CVstruct_tr->pr =  invertPr;
    CVstruct_bl->pr =  invertPr;
    CVstruct_br->pr =  invertPr;

    
    CVstruct_tl->denominator =  arith_elem->denominator;
    CVstruct_tr->denominator =  arith_elem->denominator;
    CVstruct_bl->denominator =  arith_elem->denominator;
    CVstruct_br->denominator =  arith_elem->denominator;

    
    float_CV* arrayPostl = array_at(*(Uarray2 *)cl, width * 2, height * 2);
    float_CV* arrayPostr = array_at(*(Uarray2 *)cl, (width * 2) + 1 , height * 2);
    float_CV* arrayPosbl = array_at(*(Uarray2 *)cl, width * 2, (height * 2) + 1);
    float_CV* arrayPosbr = array_at(*(Uarray2 *)cl, (width * 2) + 1, (height * 2) + 1);

    *arrayPostl = CVstruct_tl;
    *arrayPostr = CVstruct_tr;
    *arrayPosbl = CVstruct_bl;
    *arrayPosbr = CVstruct_br;

    (void)elem;
    (void)cl;
    (void) width;
    (void) height;
    (void) value;
 }