#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <ctype.h>
#include <string.h>
#include "lecteur_fichier.h"
#include "liste_chainee.h"
#include <unistd.h>
#include<time.h>

int main()
{
//CONSTRUCTION DE LA MACHINE DE TURING
	turing code = construction_programme();
	//affichage_programme(code); //cela permet de vérifier le bon chargement du programme si désiré
	if(code.alphabet == NULL)
	{
		return(0);
	}
	
//CRÉATION ET AFFICHAGE DE LA BANDE AVEC SON MOT VÉRIFIER PAR L'UTILISATEUR
	bande b = initialisation_bande(code);
	affichage(b);
	char boolean = 'N';
	while(boolean == 'N')
	{
		printf("\nCette saisie vous convient-elle? [O/N]\n");
		scanf(" %c",&boolean);
		boolean = toupper(boolean);
		while((boolean != 'O') && (boolean != 'N'))
		{
			printf("\nSaisie invalide, valeur attendue 'O' ou 'N'\n");
			scanf(" %c",&boolean);
		}
		if(boolean == 'N')
		{
			destruction_bande(b);
			bande b = initialisation_bande(code);
			affichage(b);
		}
	}
	
//CHOIX DE LA VITESSE D'EXECUTION D'AFFICHAGE DU PROGRAMME DANS TERMINAL
	int vitesse;
	printf("\nVeuillez selectionner une vitesse d'éxécution :\n\t 0 pour instantannée\n\t 1 pour très rapide\n\t 2 pour moyen\n\t 3 pour lent\n");
	scanf(" %d",&vitesse);
	while((vitesse != 0) & (vitesse != 1) & (vitesse != 2) & (vitesse != 3))
	{
		printf("\nSaisie incorrecte: valeur attendue 0,1,2 ou 3\n");
		scanf(" %d",&vitesse);
	}


	int etat = 0;
	char caractere_lu;

    	int temps_de_calcul = clock(); //début du chronomètre
//Le calcul s'effectue tant que le calcul n'est pas à l'état accepter(= -1) ou rejeter(= -2)
	while((etat != (-1)) && (etat != (-2)))	
	{
		caractere_lu = lecture(b);
		for(int i =0; i < code.nombre_de_transition; i++)
		{
			if((etat == code.transitions[i].etat) && (caractere_lu == code.transitions[i].lu))
			{
				b = ecriture(b,code.transitions[i].ecrit);
				if(code.transitions[i].mouvement == '<')
				{
					b = deplacement_gauche(b);
				}
				if(code.transitions[i].mouvement == '>')
				{
					b = deplacement_droite(b);
				}
				
				etat = code.transitions[i].etat_suivant;
				break;
			}
		}
		switch(vitesse)
		{
			case 1:
				affichage(b);
				usleep(50000);
				break;
			case 2:
				affichage(b);
				usleep(200000);
				break;
			case 3:
				affichage(b);
				sleep(1);
				break;
		}
	}



	affichage(b);
	printf("\n\n\n\t");
	printf("le temps de calcul sur la bande est de = %d ms", temps_de_calcul);
	printf("\n\n\n\t");
	printf("Fin du programme avec succès!");
	printf("\n\n\n\t");
	if(etat == -1)
	{
		printf("\tLA MACHINE A ACCEPTÉ.\n");
	}
	if(etat == -2)
	{
		printf("LA MACHINE A REFUSÉ.\n");
	}
	
//LIBERATION DE LA MÉMOIRE ET FIN DU PROGRAMME
	destruction_bande(b);
	destruction_programme(code);
	system("sleep 5");
	return(0);
}
