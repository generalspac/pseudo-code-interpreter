#include <stdio.h> 

#include "ast_construction.h"
#include "ast_parcours.h"
// #include "table_symbole.h"


int main() {

 Ast ast1, ast2, ast3, ast4, ast5, ast6, ast7, ast8, ast9, ast10, ast11, ast12, ast13, ast14, ast15, ast16, ast17, ast18, ast;
 TypeOperateur op = N_PLUS;
 Symbole_tab *tab = init_table_vide(); 
 ast8 = NULL;
 ast = NULL;
 ast1 = NULL;
 ast2 = NULL;
 ast3 = NULL;
 ast4 = NULL;
 ast5 = NULL;
 ast6 = NULL;
 ast7 = NULL;
 ast9 = NULL;
 ast10 = NULL;
 ast11 = NULL;
 ast12 = NULL;
 ast13 = NULL;
 ast14 = NULL;
 ast15 = NULL;
 ast16 = NULL;
 ast17 = NULL;
 ast18 = NULL;

 // création des variables:

 ast1 = creer_variable("X");
 ast2 = creer_variable("Y");
 ast3 = creer_variable("Z");

 // création des feuilles Y = X % Z et Ecr Y

 ast4 = creer_operation(N_MODULO, ast1, ast3);
 ast5 = creer_aff(ast2, ast4);

 // création des deux seq1

 ast6 = creer_seq_inst(ast4, ast5);

 // création de Lire Z
 ast7 = creer_lire(ast3);

 // création de la seq2

 ast8 = creer_seq_inst(ast7, ast6);

 // création de l'affectation X <- 10 + 5

 ast9 = creer_valeur(10);
 ast10 = creer_valeur(5);
 ast11 = creer_operation(op, ast9, ast10);
 ast12 = creer_aff(ast1, ast11);

 // créeation de la troisième seq

 ast = creer_seq_inst(ast12, ast8);

 // Test la condition si
 ast15 = creer_lire(ast1);
 ast16 = creer_ecrire(ast1);
 ast17 = creer_seq_inst(ast15, ast16);

 ast14 = creer_op_bool(ast9, N_INF, ast10);

 ast18 = creer_si(ast14, ast, ast17);
//  afficher_ast(ast);

//  afficher_ast(ast18);
 
//  interpretation(ast, tab);
 
 interpretation(ast18, tab);

//  ast14 = creer_variable("X");
//  ast13 = creer_ecrire(ast1);

 interpretation(ast13, tab);

 printf("On libere la memoire avant de finir\n");
 liberer_ast(&ast);
 liberer_ast(&ast13);
 liberer_ast(&ast18);
 liberer_table(tab);
 
 return 0 ;

}
