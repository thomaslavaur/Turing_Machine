struct transition
{
	int etat;
	char lu;
	char ecrit;
	char mouvement;
	int etat_suivant;
};

typedef struct transition transition;

struct turing
{
	char* alphabet;
	transition* transitions;
	int cardinal_de_l_alphabet;
	int nombre_de_transition;
};

typedef struct turing turing;

turing construction_programme(void);

void destruction_programme(turing t);

void affichage_programme(turing t);
