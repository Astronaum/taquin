
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "liste.h"

static booleen  trouveS =false;
static int noeudsVisite=0;
static int flimit;
#define TAILLE 3


//Fct retourne le nbr de positions différentes entre le tq init et but sans prendre en considération
// la case 0
static int H(Matrice matrice, Matrice matriceBut) {
    int cases=0;

    for (int i = 0; i < TAILLE; i++) {
        for (int j = 0; j < TAILLE; j++) {
            if (matrice[i * TAILLE + j] != matriceBut[i * TAILLE + j] && matrice[i * TAILLE + j] != 0) {
                cases++;
            }
        }
    }
    return cases;
}

//Initialisation de la racine qui représente l'état initiale du puzzle
Noeud* creerRacine(Matrice matrice,Matrice but){

Noeud* noeud = new Noeud;
noeud->mat=matrice;
noeud->parent=NULL;
noeud->action=NULL;
int* h=(int*)malloc(sizeof(int));
*h=H(matrice,but);
noeud->f=h;
noeud->cout=0;


return noeud;
}


//Vérifie si on respecte la taille de la matrice
static bool estValide(int i, int j) {
   return (i >= 0 && i < TAILLE && j >= 0 && j < TAILLE);
}


//Déterminer les déplacements possibles
static Matrice actionsPossibles(Matrice taquin,Matrice tailleActions) {
   int i, j, k = 0;
   Matrice actions = (int*)malloc(4 * sizeof(int));

   for (i = 0; i < TAILLE; i++) {
      for (j = 0; j < TAILLE; j++) {
         if (taquin[i * TAILLE + j] == 0) {
            if (estValide(i - 1, j)) {
               actions[k++] = 0; // Haut
            }
            if (estValide(i, j + 1)) {
               actions[k++] = 1; // Droite
            }
            if (estValide(i + 1, j)) {
               actions[k++] = 2; // Bas
            }
            if (estValide(i, j - 1)) {
               actions[k++] = 3; // Gauche
            }
            break;
         }
      }
   }
    *tailleActions=k;
   return actions;
}

//echanger deux cases adjacentes
static void echanger(Matrice a, Matrice b) {
   int temp = *a;
   *a = *b;
   *b = temp;
}

//effectuer les déplacements
static Matrice appliquerAction(Matrice taquin, int action) {
   int i, j;
   Matrice resultat = (int*)malloc(TAILLE * TAILLE * sizeof(int));
   for (i = 0; i < TAILLE; i++) {
      for (j = 0; j < TAILLE; j++) {
         resultat[i * TAILLE + j] = taquin[i * TAILLE + j];}}
   for (i = 0; i < TAILLE; i++) {
      for (j = 0; j < TAILLE; j++) {

         if (taquin[i * TAILLE + j] == 0) {
            switch (action) {
               case 0: // Haut
                  if (estValide(i - 1, j)) {
                     echanger(&resultat[i * TAILLE + j], &resultat[(i - 1) * TAILLE + j]);
                  }
                  break;
               case 1: // Droite
                  if (estValide(i, j + 1)) {
                     echanger(&resultat[i * TAILLE + j], &resultat[i * TAILLE + j + 1]);
                  }
                  break;
               case 2: // Bas
                  if (estValide(i + 1, j)) {
                     echanger(&resultat[i * TAILLE + j], &resultat[(i + 1) * TAILLE + j]);
                  }
                  break;
               case 3: // Gauche
                  if (estValide(i, j - 1)) {
                     echanger(&resultat[i * TAILLE + j], &resultat[i * TAILLE + j - 1]);
                  }
                  break;
            }
            break;
         }
      }
   }
   return resultat;
}

//Affichage d'un noeud (matrice)
static void afficherMatrice(Matrice matrice) {
   printf("------------------------------\n");
   for (int i = 0; i < 3; i++) {
      for (int j = 0; j < 3; j++) {
         printf("%d|", *(matrice + i * 3 + j));
      }
      printf("\n");
   }


}


void initTaquin (Taquin* tq,Noeud* racine,Matrice but, void (*afficher) (Matrice),Matrice (*Actions) (Matrice,Matrice),Matrice (*Resultat) (Matrice,int),
			   int   (*Test) (Matrice,Matrice),int   (*H) (Matrice,Matrice)) {
  tq->racine = racine;
  tq->but = but;
  tq->afficher = afficher;
  tq->Actions = Actions;
  tq->Resultat = Resultat;
  tq->Test= Test;
  tq->H =H;

}

//lire les valeurs du tq init et but
static void lireMatrice(Matrice matrice) {
    printf("Entrez les valeurs de la matrice :\n");
    for (int i = 0; i < TAILLE; i++) {
        for (int j = 0; j < TAILLE; j++) {
            printf("M [%d][%d] : ", i, j);
            scanf("%d", &matrice[i * TAILLE + j]);
        }
    }
}

//Initialisation du taquin
void initTaquin (Taquin* tq,Noeud* racine,Matrice but) {
  initTaquin (tq,racine ,but,afficherMatrice, actionsPossibles,appliquerAction,comparerMatrice,H);
}

Noeud*   Noeud_fils	(Taquin* taquin,Noeud* parent,int action )
{
    Noeud* noeud = (Noeud*)malloc(sizeof(Noeud));
    noeud->mat=taquin->Resultat(parent->mat,action);
    noeud->parent=parent;
    noeud->action=action;
    Matrice h=(int*)malloc(sizeof(int));
    *h=parent->cout+1+taquin->H(noeud->mat,taquin->but);
    noeud->f=h;
    return noeud;
}

//créer taquin
Taquin* CreerTaquin (Noeud* racine,Matrice but)
{

      Taquin* tq = (Taquin*) malloc (sizeof (Taquin));

    initTaquin (tq,racine,but);
    return tq;



}

static void solution(Noeud* noeud){
    Matrice mat;
    Liste* li =creerListe(1);
    bool condition=true;
    insererEnTeteDeListe(li,noeud);
    printf("\n\n\n------------Solution------------\n\n\n");
    while (condition){
        noeud=noeud->parent;
        if(noeud!=NULL){
            insererEnTeteDeListe(li,noeud);}
        else condition=false;
    }

    Noeud* extraite= (Noeud*) extraireEnTeteDeListe(li);
    mat=extraite->mat;

    afficherMatrice(mat);

    while(!listeVide(li)){
        Noeud* extraite= (Noeud*) extraireEnTeteDeListe(li);
        mat=extraite->mat;
        afficherMatrice(mat);
    }
    detruireListe(li);
}


static void A (Taquin* tq,Liste* li)
{
    Liste* explore = creerListe();
    insererEnFinDeListe(li,tq->racine,tq->racine->f);

    Matrice tailleActions=(int*)malloc(sizeof(int));
    Matrice mat=NULL;
    Matrice actions;
    booleen trouve_Frontier;
    booleen trouve_explore;
    booleen t;
        printf("--------------A*--------------\n");

    while(!listeVide(li)){
        Noeud* extraite= (Noeud*) extraireEnTeteDeListe(li);
        noeudsVisite++;
        mat=extraite->mat;

    if(tq->Test(mat,tq->but))
    {
    solution( (Noeud*)extraite);
    trouveS=true;
    return;
    }

    insererEnTeteDeListe(explore,mat);
    actions=tq->Actions(mat,tailleActions);
    for(int i=0;i<*tailleActions;i++)
    {
        Noeud* fils =Noeud_fils(tq,(Noeud*)extraite,*(actions+i));

        trouve_Frontier=chercherParNoeud( li, fils);

        trouve_explore=chercherParMatrice(explore, fils->mat);

        if (!trouve_explore && !trouve_Frontier){
                insererEnOrdre(li,fils,fils->f);
        }
        else if(trouve_Frontier){
               Noeud* noeudf= (Noeud*)chercherUnObjet (li,fils);

               if((noeudf->f)>(fils->f))
               {
                 t=extraireUnObjet (li,noeudf );
                 insererEnOrdre(li,fils,fils->f);
               }
            }
        }
    }

    detruireListe(explore);
}

void AEtoile(){
    printf(" Saisir Etat Initial\n");
    Matrice Init = (int*)malloc(9*sizeof(int));
    lireMatrice(Init);


    printf(" Saisir Etat But\n");
    Matrice but = (int*)malloc(9*sizeof(int));
    lireMatrice(but);



    Noeud* racine= creerRacine(Init,but);

    Taquin* tq = CreerTaquin(racine,but);



    Liste* li=creerListe(1);

    A (tq, li);

    if(trouveS)
    {

        trouveS=faux;
    }
    else
    {
        printf("\nLe noeud est introuvable");
    }

    printf("\nNoeuds visites  : %d",noeudsVisite);
    noeudsVisite=0;
detruireListe(li);

}

static void IDA (Taquin* tq,Liste* li)
{
    Liste* explore = creerListe();
    insererEnFinDeListe(li,tq->racine,tq->racine->f);

    Matrice tailleActions=(int*)malloc(sizeof(int));
    Matrice mat=NULL;
    Matrice actions;
    booleen trouve_Frontier;
    booleen trouve_explore;
    booleen t;


    while(!listeVide(li)){
        Noeud* extraite= (Noeud*) extraireEnTeteDeListe(li);
        noeudsVisite++;
        mat=extraite->mat;
    if (*(extraite->f)>flimit)
    {   flimit=*(extraite->f);
        detruireListe(explore);
        return;
    }
    afficherMatrice(mat);
    if(tq->Test(mat,tq->but))
    {
        solution( (Noeud*)extraite);
        trouveS=true;
        flimit=INT_MAX;
    return;
    }

    insererEnTeteDeListe(explore,mat);
    actions=tq->Actions(mat,tailleActions);
    for(int i=0;i<*tailleActions;i++)
    {
        Noeud* fils =Noeud_fils(tq,(Noeud*)extraite,*(actions+i));

        trouve_Frontier=chercherParNoeud( li, fils);

        trouve_explore=chercherParMatrice(explore, fils->mat);

        if (!trouve_explore && !trouve_Frontier){
                insererEnOrdre(li,fils,fils->f);
        }
        else if(trouve_Frontier){
               Noeud* noeudf= (Noeud*)chercherUnObjet (li,fils);

               if((noeudf->f)>(fils->f))
               {
                 t=extraireUnObjet (li,noeudf );
                 insererEnOrdre(li,fils,fils->f);
               }

    }

    }



}
flimit= INT_MAX;
detruireListe(explore);
}

void IDAEtoile(){
    printf(" Saisir Etat initial\n");
    Matrice Init = (int*)malloc(9*sizeof(int));
    lireMatrice(Init);

    printf(" Saisir Etat But\n");
    Matrice but = (int*)malloc(9*sizeof(int));
    lireMatrice(but);

    printf("/n/n--------------IDA*--------------\n");

    Noeud* racine= creerRacine(Init,but);

    Taquin* tq = CreerTaquin(racine,but);
    int i=0;

    flimit=*(racine->f);
    Liste* li=creerListe(1);
    while(flimit != INT_MAX){
    printf("\n\n-----Iteration %d || Limite =%d-----\n\n",i,flimit);
        detruireListe(li);
        IDA (tq, li);
    }
    if(trouveS)
    {

        trouveS=faux;
    }
    else
    {
        printf("\nLe noeud est introuvable");
    }

    printf("\nNoeuds visites : %d",noeudsVisite);
    noeudsVisite=0;
detruireListe(li);

}
