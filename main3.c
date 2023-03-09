#include <stdlib.h>
#include <stdio.h>
#include "itineraires.h"

int main(void){
    Itineraire* itin = deuxiemeItineraire(48.8464111,2.3548468, 50);
    afficheLDS(itin->it);
    printf("Points = %d\n", itin->pts);
    printf("TempsRestant = %f\n", itin->tempsRestant);

    FILE* f = fopen("Tour.txt", "w");

    LDS* tmp = itin->it;

    while(tmp){
        fprintf(f, "%f, %f, %s\n", tmp->s->LAT, tmp->s->LONG, tmp->s->categorie);
        tmp = tmp->suiv;
    }

    fprintf(f, "%f, %f, %s\n", itin->it->s->LAT, itin->it->s->LONG, itin->it->s->categorie);
    fclose(f);

    const char* commandLine= "java -jar UnescoMap.jar";
    system(commandLine);
    
    deleteItineraire(itin);

    return 0;
}