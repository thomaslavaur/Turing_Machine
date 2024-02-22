struct cellule
{
	char caractere;
	struct cellule* suiv;
	struct cellule* preced;
};

typedef struct cellule cellule;
typedef cellule* bande;

void affichage (bande b);

bande deplacement_gauche(bande b);

bande deplacement_droite(bande b);

bande ecriture(bande b,char c);

char lecture(bande b);

void destruction_bande(bande b);

int test_mot(char *mot,turing t);

bande initialisation_bande(turing t);
