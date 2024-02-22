#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "lecteur_fichier.h"
#include <dirent.h>
#include <sys/types.h>
/*
	"lecteur_fichier.c" permet de choisir le programme de calcul et de générer la machine de Turing 
	permettant d'effectuer le calcul sur le mot écrit sur la bande.
*/
turing construction_programme()
{
	turing t;
	FILE *fichier;
//les structures dirent permettent d'entrée dans un repertoire.	
	struct dirent *nom_programme;		
	struct dirent *nom_choix_programme;
	int i=0;
	
// choix du programme et ouvrir le fichier suivant le chemin d'accès
	char * nom = "./programmes/";
	char *temp=(char*)malloc(50);
	printf("\nChoix du programme à charger : ");
	      
// ouverture du repertoire des programmes de calcul et affichage de ces derniers
        DIR *repertoire_programmes = opendir("./programmes/" );
        printf("\n");
        while (nom_programme = readdir(repertoire_programmes)) {
        	if(nom_programme->d_type == 8){
       			i++;
       	 		printf("\t\t\t\t%d : %s\n",i ,nom_programme->d_name);
       	 	}        	
        }
        closedir(repertoire_programmes);
        
//Choix par l'utilisateur du programme de calcul à charger	
	int choix = 0;
	printf("Entrer le numéro du programme : ");
	scanf("%d",&choix);
	while (choix<1 || choix > i){
		printf("Erreur, entrer à nouveau le numéro du programme choisi : ");
		scanf("%d",&choix);
	}
	i=0;
	
        DIR *choix_programme = opendir("./programmes/" );
        printf("\n");
        while(i!=choix){
        	nom_choix_programme=readdir(choix_programme);
        	if(nom_choix_programme->d_type == 8){
        		i++;
        	}
        }
        closedir(choix_programme);

	char * chemin = (char*)malloc(50);
	strcpy(chemin,nom);
	strcat(chemin,nom_choix_programme->d_name);
	closedir(repertoire_programmes);

//chargement de la table de transition du programme de calcul choisi.
	fichier = fopen(chemin,"r");
	char caractere;
	int compteur = 0;
	int nombre_de_ligne = 0;
	t.cardinal_de_l_alphabet = 0;
	t.nombre_de_transition = 0;
	if ( fichier == NULL )
	{
		printf ( "Fichier introuvable!\n" );
		t.alphabet = NULL;
		t.transitions = NULL;
		return(t);
	}
	else
	{
		while(! feof(fichier) )
		{
			caractere = fgetc(fichier);
			if((nombre_de_ligne == 0) && (caractere != ',') && (caractere != '_') && (caractere != '\r'))
			{
				if(caractere != '\n'){
				t.cardinal_de_l_alphabet++;
				}
				else
				{
					nombre_de_ligne++; //permet de savoir s'il s'agit de la première ligne ou non PAS DE COMMENTAIRE SUR LA PREMIERE LIGNE
				}
			}
			if(nombre_de_ligne != 0)
			{
				if((caractere != '/') && (caractere != '\n') && (caractere != '\r') && (caractere != EOF)){
					t.nombre_de_transition++;
					while((caractere != '\n') && (caractere != EOF) && (caractere != '\r')){caractere = fgetc(fichier);}
				}
				if((caractere != '\n') && (caractere != EOF)){
					while((caractere != '\n') && (caractere != EOF) && (caractere != '\r')){caractere = fgetc(fichier);}
				}
				nombre_de_ligne++;
			}		
		}
		fseek(fichier,0,SEEK_SET); // On se replace au début pour une seconde passe

		t.alphabet = (char*) calloc(t.cardinal_de_l_alphabet,sizeof(char));
		t.transitions = (transition *) calloc(t.nombre_de_transition,sizeof(transition));
		int nombre_de_transition_compteur = 0;
		int cardinal_de_l_alphabet_compteur = 0;
		int ligne = 0;
		while( !feof(fichier))
		{
			caractere = fgetc(fichier);
			if(ligne== 0)
			{
				if((caractere != ',') && ( caractere != '\n') && (caractere != '_'))
				{
					t.alphabet[cardinal_de_l_alphabet_compteur] = caractere;
					cardinal_de_l_alphabet_compteur++;
				}
				if(caractere == '\n')
				{
					ligne++;
				}
			}
			else
			{
				if((caractere != '/') && (caractere != '\n') && (caractere != '\r') && (caractere != EOF)){
					t.transitions[nombre_de_transition_compteur].etat = (int) caractere - 48; //convertie un chiffre caractère en entier
					fgetc(fichier);
					caractere = fgetc(fichier);
					t.transitions[nombre_de_transition_compteur].lu = caractere;
					fgetc(fichier);
					caractere = fgetc(fichier);
					t.transitions[nombre_de_transition_compteur].ecrit = caractere;
					fgetc(fichier);
					caractere = fgetc(fichier);
					t.transitions[nombre_de_transition_compteur].mouvement = caractere;
					fgetc(fichier);
					fscanf(fichier,"%d",&t.transitions[nombre_de_transition_compteur].etat_suivant);
					nombre_de_transition_compteur += 1;
				}
				else
				{
					while((caractere != '\n') && (caractere != EOF) && (caractere != '\r')){caractere = fgetc(fichier);}
				}
			}
		}
	}
	free(temp);
	free(chemin);
	fclose(fichier);
	return(t);
}

void destruction_programme(turing t)  // fonction libératrice de la mémoire après calcul
{
	free(t.transitions);
	free(t.alphabet);
	t.alphabet = NULL;
	t.transitions = NULL;
}

void affichage_programme(turing t) // fonction affichant le programme charger 
{
	printf("\nLe cardinal de l'alphabet vaut %d et le nombre de transitions : %d",t.cardinal_de_l_alphabet,t.nombre_de_transition);
	printf("\n");
	for(int i = 0; i<t.cardinal_de_l_alphabet; i++)
	{
		printf("%c",t.alphabet[i]);
	}
	printf("\n");
	for(int i =0; i<t.nombre_de_transition; i++)
	{
		printf("%d\t%c\t%c\t%c\t%d",t.transitions[i].etat,t.transitions[i].lu,t.transitions[i].ecrit,t.transitions[i].mouvement,t.transitions[i].etat_suivant);
		printf("\n");
	}
}
