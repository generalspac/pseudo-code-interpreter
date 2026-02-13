
/* Exemple d'utilisation de l'analyse lexicale : 
    Affichage d'une sequence de lexemes            */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "generateur_aleatoire.h"
// #include "analyse_syntaxique.h"

int main (int argc, char *argv[]) {
   int longueur = 11;
   int nombre_gen = 10;

   srand(time(NULL));
    if(argc == 3){
      longueur = atoi(argv[1]);
      nombre_gen = atoi(argv[2]);
    }
    
    char nom_fichier[256];

    while (nombre_gen > 1)
    {
      sprintf(nom_fichier, "./Tests/entree_al%i.txt", nombre_gen);
      int res = rec_generateur_aleatoire(nom_fichier, longueur);   
      if(res == 1) printf("Generation Reussi\n");
      else printf("Generation non Reussi\n");
      nombre_gen--;
    }
   return 0 ;
}
