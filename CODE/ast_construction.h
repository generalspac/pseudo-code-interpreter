
#ifndef __AST_CONSTRUCTION__
#define __AST_CONSTRUCTION__

#include "type_ast.h"

Ast creer_operation(TypeOperateur opr , Ast opde_gauche , Ast opde_droit) ;
// renvoie un arbre abstrait de nature OPERATEUR,  d'operateur opr 
// et ayant opde_gauche et opde_droit pour fils gauche et droit

Ast creer_valeur(double val) ; 
// renvoie un arbre abstrait "feuille", de nature OPERATION  
// et de valeur val

Ast creer_variable(char *var) ;

Ast creer_ecrire(Ast ast) ;

Ast creer_lire(Ast ast) ;

// cree un noeud N_SEPINST comme fils gauche et d'inst comme fils droit ou epsilon 
Ast creer_seq_inst(Ast opde_gauche, Ast opde_droit);

// cree un noeud N_AFF de fils Ag et Ad.
Ast creer_aff(Ast opde_gauche, Ast opde_droit);

// cree juste un ';' à la fin d'une instruction qui n' a pas de suite d'instructiion.
Ast creer_fin_inst();

// libère la memoire
void liberer_ast(Ast *A);

Ast creer_condition(Ast op_gauche, TypeOpBool op, Ast op_droite);

Ast creer_op_bool(Ast op_gauche, TypeOpComp op, Ast op_droite);

Ast creer_si(Ast condition, Ast alors, Ast sinon);

Ast creer_tq(Ast condition, Ast faire);

Ast creer_non(Ast A);

#endif
