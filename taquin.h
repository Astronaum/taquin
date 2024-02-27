typedef int* Matrice;
typedef int  booleen;
#define TAILLE 3
typedef void Objet;

typedef struct noeud
{
    Matrice               mat;
    struct noeud*       parent;
    int   action;  // 0 haut 1 droit 2 bas 4 gauche
    Matrice    f;
    int cout;
} Noeud;

typedef struct
{
    Noeud* racine;
    Matrice but;
    void (*afficher) (Matrice);
    Matrice   (*Actions) (Matrice,Matrice);
    Matrice (*Resultat) (Matrice,int);
    int   (*Test) (Matrice,Matrice);
    int   (*H) (Matrice,Matrice);
} Taquin;

// Cr�er Racine, l'etat initial du taquin
Noeud* creerRacine(Matrice matrice);

// Init tq et cr�ation tq
void initTaquin (Taquin* tq,Noeud* racine,Matrice but, char* (*afficher) (Matrice),Matrice   (*Actions) (Matrice),Matrice (*Resultat) (Matrice,int),
			   bool   (*Test) (Matrice,Matrice),int   (*H) (Matrice,Matrice));
void initTaquin (Taquin* tq,Noeud* racine,Matrice but);
Taquin* CreerTaquin (Noeud* racine,Matrice but);

//Cr�er noeud fils qui repr�sente le r�sultat d'un d�placement
Noeud*     Noeud_fils	(Taquin* taquin,Noeud* parent,int action );

void AEtoile ();
void IDAEtoile();







