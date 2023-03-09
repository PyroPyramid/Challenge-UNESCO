#include"site.h"
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include "itineraires.h"

//PARTIE 1 : Fonctions de calcul
void MAJTemps(float* tempsRestant, float* tempsRetour, float distance2sites, float distanceDuPointDepart){
    *tempsRestant = *tempsRestant - TEMPSSITE - distance2sites / VITESSE;
    *tempsRetour = distanceDuPointDepart / VITESSE;
}

int ajoutePoints(int pts, Site* s, char* paysVisite[]){
    char* paysSite = s->pays;
    int res = PTSSITE;
    if(s->EnDanger==1) res+= PTSDANG;
    int i = 0;
    while( (i<NBPAYS) && (strcmp(paysVisite[i], "")!=0) ){
        if(strcmp(s->pays,paysVisite[i])==0){ //verifier si pays visité
            return pts + res;
        }
        i++;
    }
    paysVisite[i]=strdup(paysSite);
    return pts + res + PTSPAYS;
}


//PARTIE 2 : Manipulation d'itineraires
Itineraire* creerItineraire(){
    Itineraire* res = (Itineraire*)malloc(sizeof(Itineraire));
    res->pointDepart = -1;
    res->it = NULL;
    res->nbSites = 0;
    res->nbSitesCult = 0;
    res->nbSitesNat = 0;
    res->nbSitesMix = 0;
    for(int i=0; i<NB_SITE; i++){
        res->occurence[i] = 0;
    }
    res->pts = 0;
    res->tempsRestant = TEMPSTOT;
    res->tempsRetour = 0;
    for(int i=0; i<NBPAYS; i++){
        res->pays[i] = "";
    }
    return res;
}

void ajouteSite(Itineraire* itin, Site* s, float distance2sites, float distanceDuPointDepart){
    itin->it = insererEnTete(itin->it, s);
    itin->occurence[s->index] = 1;
    itin->pts = ajoutePoints(itin->pts, s, itin->pays);
    MAJTemps(&(itin->tempsRestant), &(itin->tempsRetour), distance2sites, distanceDuPointDepart);
    (itin->nbSites)++;
    if(strcmp(s->categorie, "Natural")==0){
        (itin->nbSitesNat)++;
    }else if(strcmp(s->categorie, "Cultural")==0){
        (itin->nbSitesCult)++;
    }else{
        (itin->nbSitesMix)++;
    }
}

void deleteItineraire(Itineraire* itin){
    int i = 0;
    while( (i<NBPAYS) && (strcmp(itin->pays[i], "")!=0) ){
        free(itin->pays[i]);
        i++;
    }
    deleteLDS(itin->it);
    free(itin);
}





//PARTIE 3 : Algorithmes de constructions d'itineraires

//Itineraire naif: de proche en proche
Itineraire* premierItineraire(float LAT, float LONG){
    //Creation d'un itineraire vide
    Itineraire* res = creerItineraire();

    //Variables indispensables
    Site** tabSites = creerTabSites(NB_SITE);
    float** matDis = matDistances(tabSites, NB_SITE);


    //Trouver le premier site et l'ajouter à l'itinéraire
    int pointDepart = 0;
    float distance = calculDistance(LAT, LONG, tabSites[0]->LAT, tabSites[0]->LONG); //Distance entre 2 sites successives dans les calculs
    for(int i=1;i<NB_SITE;i++){
        float distanceNewSite = calculDistance(LAT, LONG, tabSites[i]->LAT, tabSites[i]->LONG);
        if (distanceNewSite < distance){
            distance = distanceNewSite;
            pointDepart=i;   
        }
    }
    res->pointDepart = pointDepart; 
    ajouteSite(res, tabSites[pointDepart], 0, 0);


    //L'Algorithme
    //Variables
    int candidatDebutBoucle = pointDepart;
    int candidatCourant;

    while(res->tempsRestant > res->tempsRetour){
        //On initialise candidatCourant a -1 a chaque tour de boucle
        candidatCourant = -1;

        float distanceDebutBoucle;
        //Initialiser distanceDebutBoucle avec le premier site de tabSites qui n'est pas le site courant
        for(int i=0; i<NB_SITE; i++){
            if(candidatDebutBoucle != i){
                distanceDebutBoucle = matDis[candidatDebutBoucle][i];
            }
        }
        //Trouver le site le plus proche
        for (int i=0; i<NB_SITE; i++){
            if(candidatDebutBoucle != i && res->occurence[i]==0){ //Verifier que distance != 0 et qu'on n'a pas visiter ce site avant
                if(categorieDifferente(tabSites[candidatDebutBoucle], tabSites[i])){ //Verifier si les categories alternes
                    distance = matDis[candidatDebutBoucle][i];
                    if(distance < distanceDebutBoucle){
                        //Comme on a trouve le prochain site a visiter, on n'a qu'a verifier si on a bien le temps de le visiter et rentrer
                        if(distance/VITESSE + TEMPSSITE + matDis[pointDepart][i]/VITESSE <= res->tempsRestant){ 
                            distanceDebutBoucle = distance;
                            candidatCourant = i;
                        }
                    }
                }
            }
        }

        if(candidatCourant != -1){ //Si candidatCourant = -1, cela veut dire qu'il n'y a pas d'autres sites qu'on peut visiter et on rentre
            //Insertion du site le plus proche
            ajouteSite(res, tabSites[candidatCourant], distanceDebutBoucle, matDis[pointDepart][candidatCourant]);

            //Mise A Jour candidatDebutBoucle
            candidatDebutBoucle = candidatCourant;
        }else{
            res->tempsRestant -= res->tempsRetour;
            break;
        }
    }

    //Les frees
    deleteTabSites(tabSites, NB_SITE);
    deleteMatDistance(matDis, NB_SITE);

    return res;
}


Itineraire* deuxOpt(Itineraire* itin){
    //Creer un tableau de sites de l'itineraire (beaucoup plus facile pour gerer les indices)
    Site* tabSites[itin->nbSites];
    LDS* tmp = itin->it;
    for(int i=0; i<itin->nbSites; i++){
        tabSites[i] = tmp->s;
        tmp = tmp->suiv;
    }

    //On range les sites par le plus proche
    int amelioration = 1;
    while(amelioration){
        amelioration = 0;
        for(int i=1; i<itin->nbSites-1; i++){
            for(int j=1; j<itin->nbSites-1; j++){
                if(j!=i && j!=i+1 && j!=i-1){
                    float distanceII1 = calculDistance(tabSites[i]->LAT, tabSites[i]->LONG, tabSites[i+1]->LAT, tabSites[i+1]->LONG);
                    float distanceJJ1 = calculDistance(tabSites[j]->LAT, tabSites[j]->LONG, tabSites[j+1]->LAT, tabSites[j+1]->LONG);
                    float distanceIJ = calculDistance(tabSites[i]->LAT, tabSites[i]->LONG, tabSites[j]->LAT, tabSites[j]->LONG);
                    float distanceI1J1 = calculDistance(tabSites[i+1]->LAT, tabSites[i+1]->LONG, tabSites[j+1]->LAT, tabSites[j+1]->LONG);
                    if(distanceII1 + distanceJJ1 > distanceIJ + distanceI1J1){
                        for(int k=0; k<=(i+1+j)/2 -i-1; k++){
                            Site* tmp = tabSites[i+1+k];
                            tabSites[i+1+k] = tabSites[j-k];
                            tabSites[j-k] = tmp;
                        }
                        amelioration = 1;
                    }
                }
            }
        }
    }

    //Retour aux listes chainees
    Itineraire* res = creerItineraire();
    res->pointDepart = tabSites[0]->index;
    ajouteSite(res, tabSites[0], 0, 0);
    for(int i = 1; i<itin->nbSites; i++){
        float distance = calculDistance(tabSites[i-1]->LAT, tabSites[i-1]->LONG, tabSites[i]->LAT, tabSites[i]->LONG);
        float distancePointDep = calculDistance(tabSites[0]->LAT, tabSites[0]->LONG, tabSites[i]->LAT, tabSites[i]->LONG);
        ajouteSite(res, tabSites[i], distance, distancePointDep);
    }

    deleteItineraire(itin);

    return res;
}


//DeuxOpt applique au premier itineraire
Itineraire* premierItineraireDeuxOpt(float LAT, float LONG){
    //Creation d'un itineraire vide
    Itineraire* res = creerItineraire();

    //Variables indispensables
    Site** tabSites = creerTabSites(NB_SITE);
    float** matDis = matDistances(tabSites, NB_SITE);


    //Trouver le premier site et l'ajouter à l'itinéraire
    int pointDepart = 0;
    float distance = calculDistance(LAT, LONG, tabSites[0]->LAT, tabSites[0]->LONG); //Distance entre 2 sites successives dans les calculs
    for(int i=1;i<NB_SITE;i++){
        float distanceNewSite = calculDistance(LAT, LONG, tabSites[i]->LAT, tabSites[i]->LONG);
        if (distanceNewSite < distance){
            distance = distanceNewSite;
            pointDepart=i;   
        }
    }
    res->pointDepart = pointDepart; 
    ajouteSite(res, tabSites[pointDepart], 0, 0);


    //L'Algorithme
    //Variables
    int candidatDebutBoucle = pointDepart;     //Index du dernier site visité dans l'itinéraire
    int candidatCourant;
    int deuxOptFait = 0;

    while(res->tempsRestant > res->tempsRetour){
        //On initialise candidatCourant a -1 a chaque tour de boucle
        candidatCourant = -1;

        float distanceDebutBoucle;
        //Initialiser distanceDebutBoucle avec le premier site de tabSites qui n'est pas le site courant
        for(int i=0; i<NB_SITE; i++){
            if(candidatDebutBoucle != i){
                distanceDebutBoucle = matDis[candidatDebutBoucle][i];
            }
        }
        //Trouver le site le plus proche
        for (int i=0; i<NB_SITE; i++){
            if(candidatDebutBoucle != i && res->occurence[i]==0){ //Verifier que distance != 0 et qu'on n'a pas visiter ce site avant
                if(categorieDifferente(tabSites[candidatDebutBoucle], tabSites[i])){ //Verifier si les categories alternes
                    float distance = matDis[candidatDebutBoucle][i];
                    if(distance < distanceDebutBoucle){
                        //Comme on a trouve le prochain site a visiter, on n'a qu'a verifier si on a bien le temps de le visiter et rentrer
                        if(distance/VITESSE + TEMPSSITE + matDis[res->pointDepart][i]/VITESSE <= res->tempsRestant){ 
                            distanceDebutBoucle = distance;
                            candidatCourant = i;
                        }
                    }
                }
            }
        }

        if(candidatCourant != -1){ //Si candidatCourant = -1, cela veut dire qu'il n'y a pas d'autres sites qu'on peut visiter et on rentre
            //Insertion du site le plus proche
            ajouteSite(res, tabSites[candidatCourant], distanceDebutBoucle, matDis[res->pointDepart][candidatCourant]);

            //Mise A Jour candidatDebutBoucle
            candidatDebutBoucle = candidatCourant;

            //On applique le 2-opt derriere
            deuxOptFait = 0;
        }else{
            if(deuxOptFait == 0){
                res = deuxOpt(res);
                deuxOptFait =1;
            }else{
                res->tempsRestant -= res->tempsRetour;
                break;
            }
        }
    }

    //Les frees
    deleteTabSites(tabSites, NB_SITE);
    deleteMatDistance(matDis, NB_SITE);

    return res;
}


//Dans la suite des algorithmes, on applique toujours 2-opt au fur et a mesure afin d'optimiser le mieux possible

//Deuxieme itineraire : on regarde les sites qui apportent le plus de points
//Premierement: fonctions support
int calculPoints(Site* s, char* paysVisite[]){
    char* paysSite = s->pays;
    int res = PTSSITE;
    if(s->EnDanger==1) res+= PTSDANG;
    int i = 0;
    while ((i<NBPAYS) && (strcmp(paysVisite[i], "")!=0)){
        if(strcmp(s->pays,paysVisite[i])==0){ //verifier si pays visité
            return res;
        }
        i++;
    }
    return res+PTSPAYS;
}

//Deuxiemement : Structure tabIndex pour faire un tri des sites
IndexDistance** creerTabIndex(float** matDis, int indexSite){
    IndexDistance** tabIndex= (IndexDistance**)malloc(sizeof(IndexDistance*)*NB_SITE);
    for(int i=0; i<NB_SITE; i++){
        IndexDistance* tmp = (IndexDistance*)malloc(sizeof(IndexDistance));
        tmp->index = i;
        tmp->distance = matDis[indexSite][i];
        tabIndex[i]=tmp;
    }
    return tabIndex;
}

void deleteTabIndexDistance(IndexDistance** tab){
    for(int i=0; i<NB_SITE; i++){
        free(tab[i]);
    }
    free(tab);
}


//Troisiemement: tri insertion
void insertionElem(IndexDistance** tab, int j){
    IndexDistance* tmp = tab[j];
    int i = j-1;
    while(i>-1 && tab[i]->distance > tmp->distance){
        tab[i+1] = tab[i];
        i=i-1;
    }
    tab[i+1] = tmp;
}

void triInsertion (IndexDistance** tab, int len){
    int j=1;
    for(int j=1; j<len; j++){
        insertionElem(tab, j);
    }
}

IndexDistance** tri_distance(int site,float** matDis){
    IndexDistance** tabIndexDistances = creerTabIndex(matDis, site);
    triInsertion(tabIndexDistances, NB_SITE);
    return tabIndexDistances;
}


//Quatriemement : l'algorithme
Itineraire* deuxiemeItineraire(float LAT, float LONG, int comp){
    /*Le début est exactement le meme que les algorithmes précédents, ce qui change est le corps de la boucle while
    i.e la méthode dont on choisit le prochain site a visiter*/

    //Creation d'un itineraire vide
    Itineraire* res = creerItineraire();

    //Variables indispensables
    Site** tabSites = creerTabSites(NB_SITE);
    float** matDis = matDistances(tabSites, NB_SITE);


    //Trouver le premier site et l'ajouter à l'itinéraire
    int pointDepart = 0;
    float distance = calculDistance(LAT, LONG, tabSites[0]->LAT, tabSites[0]->LONG); //Distance entre 2 sites successives dans les calculs
    for(int i=1;i<NB_SITE;i++){
        float distanceNewSite = calculDistance(LAT, LONG, tabSites[i]->LAT, tabSites[i]->LONG);
        if (distanceNewSite < distance){
            distance = distanceNewSite;
            pointDepart=i;   
        }
    }
    res->pointDepart = pointDepart; 
    ajouteSite(res, tabSites[pointDepart], 0, 0);


    //L'Algorithme
    //Variables
    int candidatDebutBoucle = pointDepart;     //Index du dernier site visité dans l'itinéraire
    int deuxOptFait = 0;

    while(res->tempsRestant > res->tempsRetour){
        IndexDistance* sites[comp]; 
        //On trie les sites par distance croissante du dernier site visité
        IndexDistance** tabIndexDistances = tri_distance(candidatDebutBoucle, matDis);

        //Apres, on choisit de la liste triee les "comp" sites dont on a le droit de visiter (un site qu'on n'a pas déjà visiter et de catégorie differente du dernier site visité) 
        int j=0;
        for(int i=0; i<NB_SITE; i++){
            IndexDistance* tmp = tabIndexDistances[i];
            if(res->occurence[tmp->index]==0){
                if(categorieDifferente(tabSites[tmp->index], tabSites[candidatDebutBoucle])){
                    sites[j] = tmp;
                    j++;
                }
            }

            if(j==comp){
                break;
            }
        }

        int candidatCourant = -1;
        int maxPoints = -1;
        for(int i=0;i<comp;i++){
            int pts = calculPoints(tabSites[sites[i]->index], res->pays);
            if(pts>maxPoints){
                //Comme on a trouve le prochain site a visiter, on n'a qu'a verifier si on a bien le temps de le visiter et rentrer
                if(sites[i]->distance/VITESSE + TEMPSSITE + matDis[pointDepart][sites[i]->index]/VITESSE <= res->tempsRestant){
                    maxPoints = pts;
                    candidatCourant = sites[i]->index;
                }
            }
        }

        deleteTabIndexDistance(tabIndexDistances);

        if(candidatCourant != -1){ //Si candidatCourant = NULL, cela veut dire qu'il n'y a pas d'autres sites qu'on peut visiter et on rentre
            //Insertion du site le plus proche
            ajouteSite(res, tabSites[candidatCourant], matDis[candidatDebutBoucle][candidatCourant], matDis[res->pointDepart][candidatCourant]);

            //Mise A Jour candidatDebutBoucle
            candidatDebutBoucle = candidatCourant;

            //On applique le 2-opt derriere
            deuxOptFait = 0;
        }else{
            if(deuxOptFait == 0){
                res = deuxOpt(res);
                deuxOptFait =1;
            }else{
                res->tempsRestant -= res->tempsRetour;
                break;
            }
        }
    }

    //Les frees
    deleteTabSites(tabSites, NB_SITE);
    deleteMatDistance(matDis, NB_SITE);

    return res;
}