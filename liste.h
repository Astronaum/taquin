// liste.h
#include "taquin.h"

#define faux   0
#define vrai   1
typedef int    booleen;

typedef void Objet;

#define NONORDONNE  0
#define CROISSANT   1
#define DECROISSANT 2

// Structure d'un �l�menent de la liste
typedef struct element {
  Objet*          reference;   // Ref sur l'objet de l'element
  Objet*          priorite;
  struct element* suivant;     // Pointeur sur l'element suivant
} Element;

// le type Liste
typedef struct {
  Element* premier; // premier �l�ment de la liste
  Element* dernier; // dernier �l�ment de la liste
  Element* courant; // �l�ment en cours de traitement (parcours de liste)
  int      nbElt;   // nombre d'�l�ments dans la liste
  int      type;    // 0:simple, 1:croissant, 2:d�croissant
  char*    (*toString) (Objet*);
  int      (*comparerN) (Noeud*, Noeud*);
  int      (*comparerM) (Matrice, Matrice);
} Liste;

void     initListe              (Liste* li, int type, char* (*toString) (Objet*),
				 int (*comparer) (Objet*, Objet*) );
void     initListe              (Liste* li);
Liste*   creerListe             (int type, char* (*toString) (Objet*),
				 int (*comparer) (Objet*, Objet*) );
Liste*   creerListe             (int type);
Liste*   creerListe             ();

booleen  listeVide              (Liste* li);
int      nbElement              (Liste* li);

void     insererEnTeteDeListe   (Liste* li, Objet* objet);
void     insererEnFinDeListe    (Liste* li, Objet* objet);
void     insererEnTeteDeListe   (Liste* li, Objet* objet, Objet* priority);
void     insererEnFinDeListe    (Liste* li, Objet* objet, Objet* priority);
void insererEnOrdre (Liste* li, Objet* objet, Objet* priority);
// parcours de liste
void     ouvrirListe            (Liste* li);
booleen  finListe               (Liste* li);
Objet*   objetCourant           (Liste* li);
void     listerListe            (Liste* li);
void     listerListe            (Liste* li, void (*f) (Objet*));

Objet* chercherUnObjet (Liste* li, Noeud* objetCherche);
Objet*   extraireEnTeteDeListe  (Liste* li);
Objet*   extraireEnFinDeListe   (Liste* li);
booleen  extraireUnObjet        (Liste* li, Objet* objet);

void     detruireListe          (Liste* li);
void     recopierListe          (Liste* l1, Liste* l2);

booleen chercherParMatrice (Liste* li, Objet* objetCherche);
booleen chercherParNoeud (Liste* li, Objet* objetCherche);



void     insererEnOrdre         (Liste* li, Objet* objet);

int comparerMatrice (Matrice matrice1, Matrice matrice2) ;

