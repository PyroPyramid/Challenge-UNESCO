#ifndef ITINERAIRE_H
#define ITINERAIRE_H

#include"site.h"

typedef struct _itineraire{
    int pointDepart;
    LDS* it;
    int nbSites;
    int nbSitesCult;
    int nbSitesNat;
    int nbSitesMix;
    int occurence[NB_SITE];
    int pts;
    float tempsRestant;
    float tempsRetour;
    char* pays[NBPAYS];
}Itineraire;

typedef struct _indexDistance{
    int index;
    int distance;
}IndexDistance;


//PARTIE 1 : Fonctions de calcul
void MAJTemps(float* tempsRestant, float* tempsRetour, float distance2sites, float distanceDuPointDepart);
int ajoutePoints(int pts, Site* s, char* paysVisite[]);


//PARTIE 2 : Manipulation d'itineraires
Itineraire* creerItineraire();
void ajouteSite(Itineraire* itin, Site* s, float distance2sites, float distanceDuPointDepart);
void deleteItineraire(Itineraire* itin);


//PARTIE 3 : Algorithmes de constructions d'itineraires
Itineraire* premierItineraire(float LAT, float LONG);
Itineraire* deuxOpt(Itineraire* itin);
Itineraire* premierItineraireDeuxOpt(float LAT, float LONG);
Itineraire* deuxiemeItineraire(float LAT, float LONG, int comp);

//Supports pour deuxiemeItineraire
//Premierement: fonctions support
int calculPoints(Site* s, char* paysVisite[]);
//Deuxiemement : Structure tabIndex pour faire un tri des sites
IndexDistance** creerTabIndex(float** matDis, int indexSite);
void deleteTabIndexDistance(IndexDistance** tab);
//Troisiemement: tri insertion
void insertionElem(IndexDistance** tab, int j);
void triInsertion (IndexDistance** tab, int len);
IndexDistance** tri_distance(int site,float** matDis);

#endif