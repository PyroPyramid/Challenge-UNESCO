#include "itineraires.h"
#include <stdlib.h>
#include <stdio.h>
#include "opti.h"

int main(void){

    FILE* fic = fopen("comp.txt","w");
    //for(int i=1;i<50;i++){
        int pts = 0;
        LDS* it = premierItineraire(48.8464111,2.3548468, &pts);
        /*LDS* itopti = insertion(it);
        deleteLDS(it);
        it = itopti;*/
        afficheLDS(it);
        printf("Points = %d\n", pts);
        //fprintf(fic,"%d %d\n",i,pts);

        FILE* f = fopen("Tour.txt", "w");

        LDS* tmp = it;

        while(tmp){
            fprintf(f, "%f, %f, %s\n", tmp->s->LAT, tmp->s->LONG, tmp->s->categorie);
            tmp = tmp->suiv;
        }

        fprintf(f, "%f, %f, %s\n", it->s->LAT, it->s->LONG, it->s->categorie);

        fclose(f);

        const char* commandLine= "java -jar UnescoMap.jar";
        system(commandLine);
        
        deleteLDS(it);
    //}
    fclose(fic);

    return 0;
}