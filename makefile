
all: Main clean

Projet_Turing: Projet_Turing.o
	gcc -o Projet_Turing.o Projet_Turing.c

Main: Main.o lecteur_fichier.o liste_chainee.o
	gcc -o Main Main.o lecteur_fichier.o liste_chainee.o

lecteur_fichier.o: lecteur_fichier.c
	gcc -o lecteur_fichier.o -c lecteur_fichier.c

liste_chainee.o: liste_chainee.c lecteur_fichier.h
	gcc -o liste_chainee.o -c liste_chainee.c

Main.o: Main.c lecteur_fichier.h liste_chainee.h
	gcc -o Main.o -c Main.c 

clean:
	rm -f lecteur_fichier.o liste_chainee.o Main.o Projet_Turing.o
