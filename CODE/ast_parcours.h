#ifndef __AST_PARCOURS__
#define __AST_PARCOURS__

#include "table_symbole.h"
#include "type_ast.h"
#include "erreur_lecture.h"


void afficher_ast(Ast expr) ;
// affiche l'arbre abstrait de l'expression arithmetique expr

resultat_lecture evaluation(Ast expr, Symbole_tab *ST_ARBRE, double *resultat) ;
// calcule la valeur de l'expression arithmetique expr
// FONCTION A COMPLETER !

// interprete l'AST du programme
resultat_lecture interpretation(Ast expr, Symbole_tab *ST_ARBRE);

resultat_lecture interpreter(Ast expr, Symbole_tab *ST_ARBRE) ;

#endif

