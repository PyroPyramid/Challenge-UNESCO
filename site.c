#include "site.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>


// EXO 1
Site* construireSite(char* nom, float LAT,float LONG,char* categorie,char* pays,int enDanger, int index){
    Site* newSite = (Site*)malloc(sizeof(Site));

    newSite->nom=strdup(nom);
    newSite->LAT=LAT;
    newSite->LONG=LONG;
    newSite->categorie = strdup(categorie);
    newSite->pays = strdup(pays);
    newSite->EnDanger=enDanger;
    newSite->index=index;

    return newSite;
}

void deleteSite(Site* s){
    free(s->nom);
    free(s->categorie);
    free(s->pays);
    free(s);
}

Site** creerTabSites(int n){
    Site** tabSite = (Site**) malloc(sizeof(Site*)*n);
    FILE* fic = fopen("sites.csv","r");

    SkipLine(fic); 
    char nom[256];
    float LAT;
    float LONG;
    char categorie[256];
    char pays[256];
    int enDanger;
    char buff[256];
    int val;

    for (int i=0;i<n;i++){
        
        GetChaine(fic,256,nom); // stock nom
        GetChaine(fic,256,buff); // stock latitude
        LAT = atof(buff); // convert en float
        GetChaine(fic,256,buff); // stock longitude
        LONG = atof(buff); // convert en float
        GetChaine(fic,256,categorie); // stock categorie
        GetChaine(fic,256,pays); // stock pays
        GetChaine(fic,256,buff); // stock continent mais osef
        GetChaine(fic,256,buff); // stock enDanger
        enDanger=atoi(buff); // convert en int

        tabSite[i]= construireSite(nom,LAT,LONG,categorie,pays,enDanger, i);

        SkipLine(fic);
    }
    fclose(fic);
    return tabSite;
}

void affichageSite(Site* s){
    printf("nom : %s ; LAT : %2f; LONG : %2f, categorie : %s; pays : %s; EnDanger : %d\n", s->nom, s->LAT, s->LONG, s->categorie, s->pays, s->EnDanger);
}

void deleteTabSites(Site** tabSites,int taille){
    for(int i=0; i<taille; i++){
        deleteSite(tabSites[i]);
    }
    free(tabSites);
}


// EXO 2
float ** matDistances(Site** tab,int n){
    float ** mat = (float**) malloc(n*sizeof(float*));
    for(int i=0;i<n;i++){
        mat[i] = malloc(n*sizeof(float));
        for(int j=0;j<n;j++){
            mat[i][j]=calculDistance(tab[i]->LAT,tab[i]->LONG,tab[j]->LAT,tab[j]->LONG);
        }
    }
    return mat;
}

void deleteMatDistance(float** matDistances, int taille){
    for(int i=0; i<taille; i++){
        free(matDistances[i]);
    }
    free(matDistances);
}


// EXO 3
LDS* creerLDS(Site* s){
    LDS* l = (LDS*) malloc(sizeof(LDS));
    l->s = construireSite(s->nom, s->LAT, s->LONG, s->categorie, s->pays, s->EnDanger, s->index);
    l->suiv = NULL;
    return l;
}

LDS* insererEnTete(LDS* l, Site* s){
    LDS* new =creerLDS(s);
    LDS* tmp=l;
    if(l==NULL){
        return new;
    }else{
        while(tmp->suiv){
            tmp=tmp->suiv;
        }
        tmp->suiv=new;
    }
    return l;
}

void deleteLDS(LDS* l){
    LDS* tmp = l;
    while(l){
        tmp=l->suiv;
        deleteSite(l->s);
        free(l); 
        l=tmp;
    }
}

void afficheLDS(LDS* l){
    LDS* cour = l;
    while(cour!=NULL){
        affichageSite(cour->s);
        cour=cour->suiv;
    }
}

int categorieDifferente(Site* s1, Site* s2){ //Retourne 1 si categorie s1 est differente (dans le sens de la contrainte) de categorie s2 et 0 sinon
    return (strcmp(s1->categorie, s2->categorie)!=0 || (strcmp(s1->categorie,"Mixed")==0 && strcmp(s2->categorie,"Mixed")==0));
}