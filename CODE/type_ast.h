
#ifndef __TYPE_AST__
#define __TYPE_AST__ 

typedef enum { OPERATION, VALEUR, VARIABLE, N_SEPINST, AFFECTATION, N_ECRIRE, N_LIRE, N_SI, N_TQ, CONDITION, N_OPBOOL } TypeAst ;
typedef enum { N_PLUS, N_MUL, N_MOINS, N_DIV, N_MODULO } TypeOperateur ;

// pour la condition
typedef enum { N_INF, N_SUP, N_EQ, N_NEQ, N_SEQ, N_IEQ } TypeOpComp;

typedef enum { N_ET, N_OU, N_NON } TypeOpBool;

typedef struct noeud { 
	TypeAst 		nature ;
	TypeOperateur 	operateur ;
	TypeOpComp		op_comp;
	TypeOpBool		op_bool;
	struct noeud 	*gauche, *droite ;
	double 			valeur ;
	char 			variable[100];
} NoeudAst ;


typedef NoeudAst *Ast ;


#endif  
