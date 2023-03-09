#include "itineraires.h"
#include <stdlib.h>
#include <stdio.h>
#include "opti.h"

int main(void){

    Itineraire* itin1 = premierItineraire(48.8464111,2.3548468);
    Itineraire* itin = optiInsertion(itin1);
    afficheLDS(itin->it);
    printf("PointsItiInitial = %d\n", itin1->pts);
    printf("SitesCult = %d\nSitesNat = %d\nSitesMixtes = %d\n", itin->nbSitesCult, itin->nbSitesNat, itin->nbSitesMix);
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
    deleteItineraire(itin1);

    return 0;
}