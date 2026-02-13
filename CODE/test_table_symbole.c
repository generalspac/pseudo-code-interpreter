#include <stdio.h> 

#include "table_symbole.h"



int main() {

 char *txt = "null";
 Symbole_tab *tab = init_table_vide();
 Couple c = new_couple("X", 32);
 Couple c1 = new_couple("int", 72);
 Couple c2 = new_couple("y", 62);
 Couple c3 = new_couple("z", 59);
 Couple c4 = new_couple("bale", 382);
 Couple c5 = new_couple("foire", 2);
 Couple c6 = new_couple(txt, 0);

 inserer(tab, c);
 inserer(tab, c1);
 inserer(tab, c2);
 inserer(tab, c3);
 inserer(tab, c4);
 inserer(tab, c5);
 inserer(tab, c6);

 rechercher(*tab ,"bale");
 
 afficher_tab(*tab);

 inserer(tab, c5);

 printf("Nouvel affichage: \n");

 afficher_tab(*tab);
 return 0 ;

}