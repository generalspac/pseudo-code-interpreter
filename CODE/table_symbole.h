#ifndef _TABLE_SYMBOLE_
#define _TABLE_SYMBOLE_

#include "erreur_lecture.h"

    #define CAPACITE_INITIALE 100 

    typedef struct
    {
        double valeur ;
	    char* variable;
    } Couple;
    
    typedef struct { 
	    Couple *tab;
        int len;        // nombre de variable dans le tableau
        int capacite;   // Capacité actuelle de la table
    } Symbole_tab;

    // initialise un tableau de couple(varible, valeur) vide
    Symbole_tab* init_table_vide();

    // liberer la mémoire d'une table déjà initialisée
    void liberer_table(Symbole_tab *tab);

    // insère ou remplace le couple c dans le tableau tab
    int inserer(Symbole_tab *tab, Couple c);

    // retourne l'indice de la variable chaine ou -1 si elle ne se trouve pas dans tab
    int rechercher(Symbole_tab tab, char* chaine);

    // retourne la valeur de la variable ou -100000 si elle ne se trouve pas dans tab
    resultat_lecture valeur_variable(Symbole_tab tab, char* variable, double *resultat);
    
    // affiche les noms des variables contenues dans tab et leurs  valeurs
    void afficher_tab(Symbole_tab tab);

    // verifie s'il y a de l'espace dans le tableau
    int est_rempli(Symbole_tab tab);

    // creer un nouveau couple de nom var et de valeur val.
    Couple new_couple(char* var, double val);

#endif