#ifndef _ERREUR_LECTURE_
#define _ERREUR_LECTURE_
typedef enum {
    OK,
    ERREUR_LEXICALE,
    ERREUR_SYNTAXIQUE,
    ERREUR_SEMANTIQUE,
    ERREUR_INTERPRETATION,
    ERREUR_VARIABLE_INCONNUE,
    ERREUR_BOUCLE_INFINIE
} resultat_lecture;


void afficher_msg_erreur(resultat_lecture const res);

#endif