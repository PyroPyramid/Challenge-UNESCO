#include "itineraires.h"
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#define NB_DEP 20
#define COMP 20


int main(void){

        float LAT=0;
        float LONG=0;
        FILE* fic = fopen("resulats.txt","w");
       
        srand(time(NULL));


        for(int i=0; i<NB_DEP; i++){
            int pts1=0;
            int pts2=0;
            int pts3=0;
            LAT =(float)((rand()%3600)-1800)/10.0;
            LONG =(float)((rand()%3600)-1800)/10.0;
            //printf("%2f %2f",LAT,LONG);
            LDS* it1 = premierItineraire(LAT,LONG, &pts1);
            LDS* it2 = premierItineraireDeuxOpt(LAT,LONG, &pts2);
            LDS* it3 = deuxiemeItineraireDeuxOpt(LAT,LONG, &pts3,COMP);
            

            fprintf(fic,"%d %d %d %d\n",i,pts1, pts2, pts3);
            
            
            deleteLDS(it1);
            deleteLDS(it2);
            deleteLDS(it3);

        }

        fclose(fic);

    return 0;
}