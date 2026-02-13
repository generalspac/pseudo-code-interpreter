#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "table_symbole.h"
#include "debug.h"

void liberer_table(Symbole_tab *tab){
    for(int i = 0; i< tab->len; i++){
        free(tab->tab[i].variable);
    }
    free(tab->tab);
    free(tab);
}

Symbole_tab *init_table_vide()
{
    Symbole_tab *tab = (Symbole_tab*) malloc(sizeof(Symbole_tab));
    tab->tab = (Couple*) malloc(sizeof(Couple) * CAPACITE_INITIALE);
    tab->len = 0;
    tab->capacite = CAPACITE_INITIALE; 
    return tab;
}

int inserer(Symbole_tab *tab, Couple c){
    int indice = rechercher(*tab, c.variable);
    if (indice != -1){
        if(debug)printf("variable %s trouvee a l'indice %i; donc on remplace sa valeur par %f: \n", c.variable, indice, c.valeur);
        tab->tab[indice].valeur = c.valeur;
        
        return 1; // valeur remplacer.
    }

    if(tab->len >= tab->capacite){
        if(debug)printf("Tableau rempli.\nOn double sa capacité\n");
        tab->capacite *= 2;
        if(debug)printf("cap %i \n", tab->capacite);
        tab->tab = realloc(tab->tab, sizeof(Couple) * tab->capacite);
        if (!tab->tab) {
            printf("Erreur de realloc\n");
            return 0;
        } 
    }

    if(debug)printf("On insere %s dans la table\n", c.variable);    
    
    tab->tab[tab->len].valeur = c.valeur;
    tab->tab[tab->len].variable = c.variable;
    tab->len++; 
    return 1;
}

int rechercher(Symbole_tab tab, char* chaine){
    for(int i = 0; i < tab.len; i++){
        if(strcmp(tab.tab[i].variable, chaine) == 0)
            return i;
    }
    if(debug)printf("%s non trouvee dans le tableau\n", chaine);
    return -1;
}

resultat_lecture valeur_variable(Symbole_tab tab, char* variable, double *resultat){
    int indice = rechercher(tab, variable);
    if(indice != -1) { *resultat = tab.tab[indice].valeur; return OK;}
    
    return ERREUR_VARIABLE_INCONNUE;
}

void afficher_tab(Symbole_tab tab){
    if(tab.len <= 0) printf("tableau vide\n");

    for(int i = 0; i < tab.len; i++){
       printf("Variable %s: = %f\n", tab.tab[i].variable, tab.tab[i].valeur);
    }
}

int est_rempli(Symbole_tab tab){
    return (tab.len >= tab.capacite);
}

Couple new_couple(char* var, double val){
    Couple new;

    new.variable = strdup(var);
    new.valeur = val;

    return new;
}