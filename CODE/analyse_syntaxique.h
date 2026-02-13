

#include "analyse_lexicale.h"
#include "type_ast.h"

/*
    -- e.i : indifferent
    -- e.f : une EAG a ete lue dans le fichier de nom nom_fichier
    -- un message est affiche pour indiquer si cette expression contenait
    -- ou non une erreur de syntaxe
*/
void analyser_EAG(char *nom_fichier);

/*
    -- e.i : indifferent
    -- e.f : Un PROG a ete lu dans le fichier de nom nom_fichier
    -- un message est affiche pour indiquer si cette expression contenait une erreur de syntaxe
*/
resultat_lecture analyser_prog(char *nom_fichier, Ast *arbre);
