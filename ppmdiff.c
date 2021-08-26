
#include "math.h"
#include "readPnm.h"

float ppmDiff(Uarray2 startArray, Uarray2 destArray){
    
    double diff = 0.0;
    double sum = 0.0;
    for(int i = 0; i < Uarray2_height(startArray); i++){
        for(int j = 0; j < Uarray2_width(startArray); j++){
        
            unsigned_RGB startRGB = *(unsigned_RGB *)array_at(startArray, j, i);
            unsigned_RGB destRGB = *(unsigned_RGB *)array_at(destArray, j, i);
            int denom = startRGB->denominator;
            double Rdiff = ((double)startRGB->R/denom) - ((double)destRGB->R/denom);
            double Gdiff = ((double)startRGB->G/denom) - ((double)destRGB->G/denom);
            double Bdiff = ((double)startRGB->B/denom) - ((double)destRGB->B/denom);
            
            sum += (pow(Rdiff, 2) + pow(Gdiff, 2) + pow(Bdiff,2));
            //printf("START: %d    DEST %d\n",startRGB->denominator, destRGB->denominator);
           if((pow(Rdiff, 2) + pow(Gdiff, 2) + pow(Bdiff,2)) > 10){
            //printf("%d    %d\n", i, j);
            //printf("R: %f    G: %f    B: %f \n",(double)destRGB->R,(double)destRGB->G,  (double)destRGB->B);
            //printf("%f  %f   %f\n", Rdiff,Gdiff,Bdiff );
            //printf("ADDING: %f     SUM: %f\n\n",(pow(Rdiff, 2) + pow(Gdiff, 2) + pow(Bdiff,2)), sum);
            
           }
        }
        
    }

    diff = (double)sum / (double)(3*Uarray2_width(startArray)*Uarray2_height(startArray));
    //printf("diff = %f\n", diff);
    //printf("sum = %f\n", sum);
    diff = sqrt(diff);
    printf("difference = %f\n",diff);
    return diff;

}