#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include "opti.h"

LDS* dupLDS (LDS* it){
    LDS* temp = it;
    LDS* res = creerLDS(temp->s);
    temp = temp->suiv;
    while (temp){
        res = insererEnTete(res, temp->s);
        temp = temp ->suiv;
    }
    return res;
}

Itineraire* dupItineraire (Itineraire* itin){
    Itineraire* res = creerItineraire();
    res->pointDepart = itin->pointDepart;
    res->it = dupLDS(itin->it);
    res->nbSites = itin->nbSites;
    res->nbSitesNat = itin->nbSitesNat;
    res->nbSitesCult = itin->nbSitesCult;
    res->nbSitesMix = itin->nbSitesMix;
    for(int i=0; i<NB_SITE; i++){
        res->occurence[i] = itin->occurence[i];
    }
    res->pts = itin->pts;
    res->tempsRestant = itin->tempsRestant;
    res->tempsRetour = itin->tempsRetour;

    for(int i=0; i<NBPAYS; i++){
        res->pays[i] = "";
    }
    int i = 0;
    while( (i<NBPAYS) && (strcmp(itin->pays[i], "")!=0) ){
        res->pays[i] = strdup(itin->pays[i]);
        i++;
    }

    return res;
}

int categorieValide(Itineraire* itin, Site* s){
    int cat = itin->nbSitesCult - itin->nbSitesNat;  //Si 1, on doit ajouter un site naturel ou mixte, si -1 on doit ajouter culturel ou mixte, si 0 on ajoute ce qu'on veut
    if( /* Cas 1 */ (cat==0) || /*Cas 2 */ ( cat==1 && strcmp(s->categorie,"Cultural")!=0 ) || /*Cas 3 */ ( cat==-1 && strcmp(s->categorie,"Natural")!=0 ) ){
        return 1;
    }else {
        return 0;
    }
}

Itineraire* insertion (Itineraire* itin){
    Site** tabSites = creerTabSites(NB_SITE);
    float** matDis = matDistances(tabSites, NB_SITE); 
    Itineraire* res = NULL;

    int cat = itin->nbSitesCult - itin->nbSitesNat; //Si 1, on doit ajouter un site naturel ou mixte, si -1 on doit ajouter culturel ou mixte, si 0 on ajoute ce qu'on veut

    for(int i = 0; i<NB_SITE; i++){
        if(itin->occurence[i]==0){
            if (/* Cas 1 */ (cat==0) 
            || /* Cas 2 */ ( (cat>0) && (strcmp(tabSites[i]->categorie,"Cultural")!=0) ) 
            || /* Cas 3 */ ( (cat<0) && (strcmp(tabSites[i]->categorie,"Natural")!=0) ) ){
                res = dupItineraire(itin);
                ajouteSite(res, tabSites[i], matDis[tabSites[i]->index][res->it->s->index], matDis[res->pointDepart][tabSites[i]->index]);
                res = deuxOpt(res);
                if(res->tempsRestant < 0){
                    deleteItineraire(res);
                }else{
                    deleteTabSites(tabSites, NB_SITE);
                    deleteMatDistance(matDis, NB_SITE);
                    return res;
                }
            }
        }
    }

    deleteTabSites(tabSites, NB_SITE);
    deleteMatDistance(matDis, NB_SITE);

    return dupItineraire(itin);
}


Itineraire* optiInsertion(Itineraire* itin){
    Itineraire* tmp = dupItineraire(itin);
    Itineraire* res = insertion(itin);

    while(tmp->pts < res->pts){
        deleteItineraire(tmp);
        tmp = res;
        res = insertion(tmp);
    }
    deleteItineraire(tmp);

    return res;
}
