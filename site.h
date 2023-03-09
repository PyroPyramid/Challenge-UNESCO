#include "lectureFichiers.h"
#include "haversine.h"

#ifndef SITE_H
#define SITE_H

#define NB_SITE 1052
#define VITESSE 85
#define TEMPSSITE 7
#define TEMPSTOT 504
#define PTSSITE 10
#define PTSPAYS 20
#define PTSDANG 30
#define NBPAYS 197

typedef struct site{
    char* nom;
    float LAT;
    float LONG;
    char* categorie;
    char* pays;
    int EnDanger;
    int index;
} Site;

typedef struct lds{
    Site* s;
    struct lds* suiv; 
} LDS;


// EXO 1
Site* construireSite(char* nom, float LAT,float LONG,char* categorie,char* pays,int enDanger,int index);
void deleteSite(Site* s);
Site** creerTabSites(int n);
void affichageSite(Site* s);
void deleteTabSites(Site** tabSites,int taille);

// EXO 2
float ** matDistances(Site** tab,int n);
void deleteMatDistance(float** matDistances, int taille);

// EXO 3
LDS* creerLDS(Site* s);
LDS* insererEnTete(LDS* l, Site* s);
void deleteLDS(LDS* l);
void afficheLDS(LDS* l);
int categorieDifferente(Site* s1, Site* s2);

#endif