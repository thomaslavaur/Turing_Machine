#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <unistd.h>
#include <sys/ioctl.h> //Permet de connaitre la taille de la console
#include <string.h>
#include "lecteur_fichier.h"
#include "liste_chainee.h"

/*
	"liste_chainee.c" gère la bande de la machine de Turing.
	La bande est une liste double chainée simulant la bande finie de Turing.
	On peut l'afficher en fonction du calcul sur le mot choisi par l'utilisateur.
*/
void affichage (bande b)//affichage de la bande dans le terminal.
{
	int largeur_ecran;
	struct winsize ecran;
	ioctl(0, TIOCGWINSZ, &ecran);
	largeur_ecran = ecran.ws_col;

	char tableau[101]; // au maximum on suppose que l'écran ne dépassera pas 101 caractère
	for(int i =0;i<101;i++)
	{
		tableau[i]='_';
	}
	if(b !=NULL)
	{
		tableau[50]=b->caractere;
		int i = 0;
		int j = 0;
		bande temp = b;
		while((temp->suiv !=NULL) & (i<50))
		{
			i +=1;
			temp = temp->suiv;
			tableau[50+i]=temp->caractere;
		};
		temp = b;
		while((temp->preced !=NULL) & (j<50))
		{
			j +=1;
			temp = temp->preced;
			tableau[50-j]=temp->caractere;
		};

	}

	system("clear");
	int nombre_de_caractere_affiche = ((int)largeur_ecran/8);
	for(int i=0;i<largeur_ecran;i++){
		printf("-");
	}
	printf("\n  ");
	for(int i=0;i<nombre_de_caractere_affiche;i++){
		printf("    ");
	}
	printf("|\n  ");
	for(int i=0;i<nombre_de_caractere_affiche;i++){
		printf("    ");
	}
	printf("v");
	printf("\n|");
	for(int i=0;i<(largeur_ecran/4)-1;i++){
		printf(" %c |",tableau[50 - nombre_de_caractere_affiche + i]);
	}
	printf("\n");
	for(int i=0;i<largeur_ecran;i++){
		printf("-");
	}
	printf("\n");

}

bande deplacement_gauche(bande b)
{
	if(b->preced != NULL)
	{
		b = b->preced;
	}
	else
	{
		b->preced = (bande) malloc(sizeof(cellule));
		b->preced->suiv = b;
		b->preced->preced = NULL;
		b->preced->caractere = '_';
		b = b->preced;
	}
	return(b);
}

bande deplacement_droite(bande b)
{
	if(b->suiv != NULL)
	{
		b = b->suiv;
	}
	else
	{
		b->suiv = (bande) malloc(sizeof(cellule));
		b->suiv->preced = b;
		b->suiv->suiv = NULL;
		b->suiv->caractere = '_';
		b = b->suiv;
	}
	return(b);
}

bande ecriture(bande b, char c)
{
	b->caractere = c;
	return(b);
}

char lecture(bande b)
{
	char c = b->caractere;
	return(c);
}

bande creation()
{
	bande b;
	b = (bande) malloc(sizeof(cellule));
	b->caractere = '_';
	b->suiv = NULL;
	b->preced = NULL;
	return(b);
}

void destruction_bande(bande b)
{
	bande sav = b->preced;
	bande temp = b;
	while(temp != NULL)
	{
		temp = b->suiv;
		free(b);
		b = temp;
	}
	temp = sav;
	while(temp != NULL)
	{
		temp = sav->preced;
		free(sav);
		sav = temp;
	}
}

//vérifie que le mot en entrée correspond au bon alphabet indiqué par la table de transition.
int test_mot(char *mot,turing t)
{
	int test_lettre;
	for(int i=0;i<strlen(mot);i++){
		test_lettre=0;

		for(int j =0 ; j<t.cardinal_de_l_alphabet ; j++)
		{
			if((mot[i] == t.alphabet[j]) )
			{
				test_lettre = 1;
			}
		}
		if(mot[i] == '_')
		{
			test_lettre = 1;
		}
		if(test_lettre==0){
			return 0;
		}
	}
	return 1;
}

//Chargement sur la bande du mot entré par l'utilisateur 
bande initialisation_bande(turing t)
{
	int vider_le_buffer; //Permet de vidder le buffer pour un scanf qui fonctionne
	printf("Veuillez appuyez sur entré pour continuer...\n");
	while((vider_le_buffer=getchar()) != EOF && vider_le_buffer != '\n');

	bande b = creation();
	printf("Rappel des lettre de l'alphabet de ce programme :\n");
	for(int i = t.cardinal_de_l_alphabet; i>=0; i--)
	{
		printf("%c ",t.alphabet[i]);
	}
	printf("\n\nchoix valeur pour le calcul : ");
	char valeur[50];
	scanf("%s",valeur);
	printf("choix : %s ",valeur);

	int validite = test_mot(valeur,t);
	while(validite!=1){
		printf(" \nErreur de saisie !\n Entrer une nouvelle valeur : ");
		scanf("%s",valeur);
		validite=test_mot(valeur,t);
	}
	printf("Le mot %s est valide !\n",valeur);
	sleep(2);
	
	int compteur = 0;
	
	for(int i=0;i<strlen(valeur);i++){
		ecriture(b,valeur[i]);
		b = deplacement_droite(b);
		compteur++;
	}
	//On replace la tête de lecture au début du mot
	b = deplacement_gauche(b);
	for(int i = 0; i<compteur-1 ; i++)
	{
		b = deplacement_gauche(b);
	}
	return(b);
}