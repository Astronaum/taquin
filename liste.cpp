// Liste module

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "liste.h"


// Les fonctions locales du module

static Element* creerElement    ();
static void     insererApres    (Liste* li, Element* precedent, Objet* objet);
static Objet*   extraireApres   (Liste* li, Element* precedent);
static Element* elementCourant  (Liste* li);


// fct qui compare deux matrices(deux taquins) - Retourne 0 si au moins une case a une valeur différente
int comparerMatrice (Matrice matrice1, Matrice matrice2) {
  for (int i = 0; i < TAILLE; i++) {
        for (int j = 0; j < TAILLE; j++) {
            if (matrice1[i * TAILLE + j] != matrice2[i * TAILLE + j]) {
                return 0;
            }
        }
    }
    return 1;
}

static int comparerNoeud (Noeud* n1, Noeud* n2) {
    Matrice matrice1=n1->mat,matrice2=n2->mat;
  for (int i = 0; i < TAILLE; i++) {
        for (int j = 0; j < TAILLE; j++) {
            if (matrice1[i * TAILLE + j] != matrice2[i * TAILLE + j]) {
                return 0;
            }
        }
    }
    return 1;
}

static char* toChar (Objet* objet) {
  return (char*) objet;
}

// Initialiser liste
void initListe (Liste* li, int type, char* (*toString) (Objet*),
			   int (*comparerN) (Noeud*, Noeud*),int (*comparerM) (Matrice, Matrice)) {
  li->premier  = NULL;
  li->dernier  = NULL;
  li->courant  = NULL;
  li->nbElt    = 0;
  li->type     = type;
  li->toString = toString;
  li->comparerN = comparerN;
  li->comparerM = comparerM;
}

// Init par défaut
void initListe (Liste* li) {
  initListe (li, NONORDONNE, toChar, comparerNoeud,comparerMatrice);
}

Liste* creerListe (int type, char* (*toString) (Objet*),
		   int (*comparerN) (Noeud*, Noeud*),int (*comparerM) (int*, int*)) {
  Liste* li = (Liste*) malloc (sizeof (Liste));
  initListe (li, type, toString,comparerN, comparerM);
  return li;
}

Liste* creerListe (int type) {
  return creerListe (type, toChar, comparerNoeud,comparerMatrice);
}

Liste* creerListe () {
  return creerListe (NONORDONNE, toChar, comparerNoeud,comparerMatrice);
}

// Verifie si la liste est vide
booleen  listeVide (Liste* li) {
  return li->nbElt == 0;
}

// Retourne le nbr d'éléments de la liste
int nbElement (Liste* li) {
  return li->nbElt;
}

//---------------Insertion dans la liste------------------------------

//Insérer en tête de liste
void insererEnTeteDeListe (Liste* li, Objet* objet) {
  Element* nouveau   = creerElement();
  nouveau->reference = objet;
  nouveau->priorite = NULL;
  nouveau->suivant   = li->premier;
  li->premier        = nouveau;
  if (li->dernier == NULL) li->dernier = nouveau;
  li->nbElt++;
}

//Insérer en fin de liste
void insererEnFinDeListe (Liste* li,  Objet* objet) {
  insererApres (li, li->dernier, objet);
}


//---------------Outils de liste---------------------------------------

// Se positionner sur le premier élement
void ouvrirListe (Liste* li) {
  li->courant = li->premier;
}

// Verifie si on est sur le dernier élément
booleen finListe (Liste* li) {
  return li->courant==NULL;
}

//Fournit un pointeur sur l'objet courant et pointe sur le suivant(le nv courant)
Objet* objetCourant (Liste* li) {
  Element* ptc = elementCourant (li);
  return ptc==NULL ? NULL : ptc->reference;
}

void listerListe (Liste* li) {
  ouvrirListe (li);
  while (!finListe (li)) {
    Element* objet = (Element*) objetCourant (li);
    printf ("%s  %s\n",li->toString(objet->reference),li->toString(objet->priorite));
  }
}

void listerListe (Liste* li, void (*f) (Objet*)) {
  ouvrirListe (li);
  while (!finListe (li)) {
    Objet* objet = objetCourant (li);
    f (objet);
  }
}


// Fournit un pointeur sur l'objet cherché
Objet* chercherUnObjet (Liste* li, Noeud* objetCherche) {
  booleen trouve = faux;
  Noeud* objet;
  ouvrirListe (li);
  while (!finListe (li) && !trouve) {
    objet  = (Noeud*)objetCourant (li);
    trouve = li->comparerN (objetCherche, objet) == 0;
  }
  return trouve ? objet : NULL;
}

// Extraire en tête de liste
Objet* extraireEnTeteDeListe (Liste* li) {
  Element* extrait = li->premier;
  if (!listeVide(li)) {
    li->premier = li->premier->suivant;
    if (li->premier==NULL) li->dernier=NULL;
    li->nbElt--;
  }
  return extrait != NULL ? extrait->reference : NULL;
}

// Extraire en fin de liste
Objet* extraireEnFinDeListe (Liste* li) {
  Objet* extrait;
  if (listeVide(li)) {
    extrait = NULL;
  } else if (li->premier == li->dernier) {
    extrait = extraireEnTeteDeListe (li);
  } else {
    Element* ptc = li->premier;
    while (ptc->suivant != li->dernier) ptc = ptc->suivant;
    extrait = extraireApres (li, ptc);
  }
  return extrait;
}

// Extraire un objet
booleen extraireUnObjet (Liste* li, Objet* objet) {
  Element* precedent = NULL;
  Element* ptc       = NULL;
  booleen trouve = faux;
  ouvrirListe (li);
  while (!finListe (li) && !trouve) {
    precedent = ptc;
    ptc       = elementCourant (li);
    trouve = (ptc->reference == objet) ? vrai : faux;
  }
  if (!trouve) return faux;

  Objet* extrait = extraireApres (li, precedent);
  return vrai;
}

//  détruire la liste
void detruireListe (Liste* li) {
  ouvrirListe (li);
  while (!finListe (li)) {
    Element* ptc = elementCourant (li);
    free (ptc);
  }
  initListe (li);
}

// Recopie une liste dans un autre
void  recopierListe (Liste* l1, Liste* l2) {
  detruireListe (l1);
  *l1 = *l2;
  initListe (l2);
}



// Créer un element
static Element* creerElement () {
  return (Element*) malloc (sizeof (Element));
  //return new Element();
}

static void insererApres (Liste* li, Element* precedent, Objet* objet) {
  if (precedent == NULL) {
    insererEnTeteDeListe (li, objet);
  } else {
    Element* nouveau   = creerElement();
    nouveau->reference = objet;
    nouveau->suivant   = precedent->suivant;
    precedent->suivant = nouveau;
    if (precedent == li->dernier) li->dernier = nouveau;
    li->nbElt++;
  }
}

static Objet* extraireApres (Liste* li, Element* precedent) {
  if (precedent == NULL) {
    return extraireEnTeteDeListe (li);
  } else {
    Element* extrait = precedent->suivant;
    if (extrait != NULL) {
      precedent->suivant = extrait->suivant;
      if (extrait == li->dernier) li->dernier = precedent;
      li->nbElt--;
    }
    return extrait != NULL ? extrait->reference : NULL;
  }
}

static Element* elementCourant (Liste* li) {
  Element* ptc = li->courant;
  if (li->courant != NULL) {
    li->courant = li->courant->suivant;
  }
  return ptc;
}


static Element* ppremier (Liste* li) {
  return li->premier;
}

Objet* pDernier (Liste* li) {
  return li->dernier == NULL ? NULL : li->dernier->reference;
}

static Element* psuivant (Element* elt) {
  return elt!=NULL ? elt->suivant : NULL;
}


// Teste si les deux noeuds sont en ordre
static booleen enOrdre (Noeud* objet1, Noeud* objet2, booleen ordreCroissant,
                 int (*comparerN) (Noeud*, Noeud*)) {
  booleen ordre = comparerN (objet1, objet2) < 0;
  if (!ordreCroissant) ordre = !ordre;
  return ordre;
}

//Inserer dans la liste un noeud, en respectant l'ordre
void insererEnOrdre (Liste* li, Noeud* objet) {
  if (listeVide (li) ) {   // liste vide
    insererEnTeteDeListe (li, objet);
  } else {
    Element* ptc = li->premier;
    if ( enOrdre (objet,(Noeud*) ptc->reference, li->type==1, li->comparerN) ) {
            //Inserer avant le 1er element
      insererEnTeteDeListe (li, objet);
    } else {
      booleen  trouve = faux;
      Element* prec   = NULL;
      while (ptc != NULL && !trouve) {
        prec   = ptc;
        ptc    = ptc->suivant;
        if (ptc!=NULL) trouve = enOrdre ((Noeud*)objet,(Noeud*) ptc->reference, li->type==1, li->comparerN);
      }
      insererApres (li, prec, objet);
    }
  }
}

static void insererApres (Liste* li, Element* precedent, Objet* objet,Objet* priorite) {
  if (precedent == NULL) {
    insererEnTeteDeListe (li, objet,priorite);
  } else {
    Element* nouveau   = creerElement();
    nouveau->reference = objet;

     nouveau->priorite = priorite;
    nouveau->suivant   = precedent->suivant;
    precedent->suivant = nouveau;
    if (precedent == li->dernier) li->dernier = nouveau;
    li->nbElt++;
  }
}
static booleen enOrdreCout (int* objet1, int* objet2) {
  booleen ordre =  (*objet1)< (*objet2) ;
  return ordre;
}
void insererEnOrdre (Liste* li, Objet* objet, Objet* priorite) {
  if (listeVide (li) ) {
    insererEnTeteDeListe (li, objet,priorite);
  } else {
    Element* ptc = li->premier;
    if ( enOrdreCout ((int*)priorite,(int*) ptc->priorite))
    {
      insererEnTeteDeListe (li, objet, priorite);
    } else {
      booleen  trouve = faux;
      Element* prec   = NULL;
      while (ptc != NULL && !trouve) {
        prec   = ptc;
        ptc    = ptc->suivant;
        if (ptc!=NULL) trouve = enOrdreCout ((int*)priorite,(int*) ptc->priorite);
      }
      insererApres (li, prec, objet,priorite);
    }
  }
}

void insererEnTeteDeListe (Liste* li, Objet* objet,Objet* priorite) {
  Element* nouveau   = creerElement();

  nouveau->reference = objet;
  nouveau->priorite = priorite;
  nouveau->suivant   = li->premier;
  li->premier        = nouveau;
  if (li->dernier == NULL) li->dernier = nouveau;
  li->nbElt++;
}




void insererEnFinDeListe (Liste* li,  Objet* objet, Objet* priorite) {
  insererApres (li, li->dernier, objet,priorite);
}

booleen chercherParMatrice (Liste* li, Objet* objetCherche) {
  booleen trouve = faux;
  Objet* objet;
  ouvrirListe (li);
  while (!finListe (li) && !trouve){
    objet  = objetCourant (li);
    trouve = li->comparerM ((Matrice)objetCherche, (Matrice)objet) == 1;
  }
  return trouve ;
}
booleen chercherParNoeud (Liste* li, Objet* objetCherche) {
  booleen trouve = faux;
  Objet* objet;
  ouvrirListe (li);
  while (!finListe (li) && !trouve){
    objet  = objetCourant (li);
    trouve = li->comparerN ((Noeud*)objetCherche, (Noeud*)objet) == 1;
  }
  return trouve ;
}

//Insertion en tenant compte du cout
void insererEnOrdreCout (Liste* li, int* accesseur,int* intcout) {
  if (listeVide (li) ) {
    insererEnTeteDeListe (li, accesseur);
  } else {
    Element* ptc = li->premier;
    if ( enOrdreCout(intcout+(*accesseur),intcout+ *((int*)ptc->reference))) {
      insererEnTeteDeListe (li, accesseur);
    } else {
      booleen  trouve = faux;
      Element* prec   = NULL;
      while (ptc != NULL && !trouve) {
        prec   = ptc;
        ptc    = ptc->suivant;
        if (ptc!=NULL) trouve =enOrdreCout(intcout+(*accesseur),intcout+ *((int*)ptc->reference));
      }

      insererApres (li, prec, accesseur);
    }
  }
}

