#include <stdio.h>
#include "erreur_lecture.h"

void afficher_msg_erreur(resultat_lecture const res)
{
    switch (res)
    {
        case OK:
            printf("Pas d'erreur! ✅\n");
            break;
        
        case ERREUR_LEXICALE:
            printf("Erreur lexicale ❌\n");
            break;
            
        case ERREUR_SYNTAXIQUE:
            printf("Erreur syntaxique ❌\n");
            break;
            
        case ERREUR_SEMANTIQUE:
            printf("Erreur semantique ❌\n");
            break;

        case ERREUR_INTERPRETATION:
            printf("Erreur d'interpretation ❌\n");
            break;

        case ERREUR_VARIABLE_INCONNUE:
            printf("Erreur Variable inconnue ❌\n");
            break;
            
        case ERREUR_BOUCLE_INFINIE:
            printf("Boucle Infinie ❌\n");
            break;

        default:
            break;
    }
}