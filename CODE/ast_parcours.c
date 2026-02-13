#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "ast_parcours.h"
#include "debug.h"



int est_feuille(Ast noeud);
resultat_lecture get_op_type(Ast noeud, TypeOperateur *typeOp);
resultat_lecture get_ast_type(Ast noeud, TypeAst *typeAst);
resultat_lecture interpreter_aff(Ast expr, Symbole_tab *ST_ARBRE);
resultat_lecture interpreter_lire(Ast expr, Symbole_tab *ST_ARBRE);
resultat_lecture interpreter_ecrire(Ast expr, Symbole_tab *ST_ARBRE);
resultat_lecture interpreter_si(Ast expr, Symbole_tab *ST_ARBRE);
resultat_lecture interpreter_tq(Ast expr, Symbole_tab *ST_ARBRE);
int evaluer_op_bool(Ast expr, Symbole_tab ST_ARBRE);
int evaluer_condition(Ast expr, Symbole_tab ST_ARBRE);

void aff_nat_op_comp(TypeOpComp op);


double get_max(double Vg, double Vd){
	if(fabs(Vg) >= fabs(Vd)) return fabs(Vg);
	return fabs(Vd);
}

double get_max_condition(Ast A, Symbole_tab ST_ARBRE){
	if(debug)	printf("Evaluation de la condition\n");
	double vd = 0, vg = 0;
	resultat_lecture res = OK;
	switch (A->nature)
	{
		case CONDITION:		return get_max(get_max_condition(A->gauche, ST_ARBRE) ,get_max_condition(A->droite, ST_ARBRE));
			
		case N_OPBOOL:		
			res = evaluation(A->gauche, &ST_ARBRE, &vg);
			if(res != OK) return 0;
			
			res = evaluation(A->droite, &ST_ARBRE, &vd);
			if(res != OK) return 0;
			
			return fabs(fabs(vg) - fabs(vd)) + 1; // + 1 pour prendre en compte les <= ou >=
		
		case N_NON:			return get_max_condition(A->gauche, ST_ARBRE);
			
		default:			return 0;
	}
}

double lire(){ 
	double val;
	printf("Entree la valeur a lire: ");
	
	scanf(" %lf", &val);
	return val;
}

void ecrire(double v) { printf(" %f\n\n", v); }

// ----------------------------------------------------------- //

int est_feuille(Ast noeud) { return (noeud->droite == NULL) && (noeud->gauche == NULL);}

resultat_lecture get_op_type(Ast noeud, TypeOperateur *typeOp){
	switch (noeud->operateur) {
		case N_PLUS: 	*typeOp = N_PLUS;	return OK;
		case N_MOINS: 	*typeOp = N_MOINS; 	return OK;
		case N_MUL: 	*typeOp = N_MUL; 	return OK;
		case N_DIV: 	*typeOp = N_DIV; 	return OK;
		case N_MODULO: 	*typeOp = N_DIV; 	return OK;
	} 
	return ERREUR_LEXICALE;
}

resultat_lecture get_ast_type(Ast noeud, TypeAst *typeAst){
	switch (noeud->nature){
		case OPERATION:		*typeAst = OPERATION;	return OK;
		case VALEUR: 		*typeAst = VALEUR;		return OK;
		case VARIABLE: 		*typeAst = VARIABLE;	return OK;
		case AFFECTATION: 	*typeAst = AFFECTATION;	return OK;
		case N_ECRIRE: 		*typeAst = N_ECRIRE;	return OK;
		case N_LIRE: 		*typeAst = N_LIRE;		return OK;
		case CONDITION:		*typeAst = CONDITION;	return OK;
		case N_OPBOOL:		*typeAst = N_OPBOOL;	return OK;
		case N_SI: 			*typeAst = N_SI;		return OK;
		case N_TQ: 			*typeAst = N_TQ;		return OK;
		case N_SEPINST: 	*typeAst = N_SEPINST;	return OK;

		default: printf("Probleme avec la nature du noeud\n"); return ERREUR_LEXICALE;
	}
	
}

resultat_lecture calculer(double vg, double vd, TypeOperateur op, double *resultat){
	switch (op)
	{
		case N_PLUS: 	*resultat = vg + vd; 	return OK;
		case N_MOINS: 	*resultat = vg - vd;	return OK;
		case N_MUL: 	*resultat = vg * vd;	return OK;
		case N_DIV: 
			if(vd == 0){ printf("ERREUR_SEMANTIQUE:division par 0\n"); return ERREUR_SEMANTIQUE; }
						*resultat = vg / vd; 	return OK;
		case N_MODULO: 
			if(vd == 0){ printf("ERREUR_SEMANTIQUE:division par 0\n"); return ERREUR_SEMANTIQUE; }
			*resultat = fmod(vg, vd);			return OK;
		default: return ERREUR_INTERPRETATION;
	}
}

char* get_ast_nature(Ast expr){
	if(expr == NULL)
		exit(1);

	switch (expr->nature)
	{
		case VALEUR: 		return "ENTIER";
		case AFFECTATION: 	return "AFFECTATION";
		case OPERATION: 	return "OPERATION";
		case N_ECRIRE: 		return "N_ECRIRE";
		case N_LIRE: 		return "N_LIRE";
		case CONDITION: 	return "CONDITION";
		case N_OPBOOL: 		return "N_OPBOOL";
		case N_SI: 			return "N_SI";
		case N_TQ: 			return "N_TQ";
		case VARIABLE: 		return "VARIABLE";
		case N_SEPINST: 	return "N_SEPINST";
		
		default:
			return "INCONNU";
	}
}
//--------------------------------------------------------------------------------------------------
void aff_operateur(TypeOperateur op){
	switch (op) {
		case N_PLUS:
			printf("+") ;
			break;
		case N_MOINS:
			printf("-") ;
			break;
		case N_MUL:
			printf("*") ;
			break;
		case N_DIV:
			printf("/") ;
			break;
		case N_MODULO:
			printf("%%") ;
			break;
	} 
}

void aff_nat_op_comp(TypeOpComp op){
	switch (op)
	{
	case N_INF:	printf(" < ");	break;
	case N_SUP:	printf(" > ");	break;
	case N_EQ:	printf(" = ");	break;
	case N_IEQ:	printf(" <= ");	break;
	case N_SEQ:	printf(" >= ");	break;
	case N_NEQ:	printf(" != ");	break;
	
	default:
		printf("Operateur de comparaison inconnu\n");
		break;
	}
}

void aff_nat_op_bool(TypeOpBool op){
	switch (op)
	{
	case N_OU:	printf(" || ");	break;
	case N_ET:	printf(" && ");	break;
	case N_NON: printf("!"); break;
	
	default:
		printf("Operateur Booleen inconnu\n");
		break;
	}
}

void afficher_ast_indent(Ast expr, int indent);

void print_indent(int indent) {
	for(int i = 0; i < indent; i++)
		printf("    "); // 4 espaces par niveau
}

void afficher_ast(Ast expr) {
	int n = 0;
	afficher_ast_indent(expr, n);
}

void afficher_ast_indent(Ast expr, int indent) {
	if(expr == NULL)
		return;

	switch (expr->nature) {
		case OPERATION:
			printf("(");
			afficher_ast_indent(expr->gauche, indent);
			aff_operateur(expr->operateur);
			afficher_ast_indent(expr->droite, indent);
			printf(")");
			break;

		case VALEUR:
			printf("%f", expr->valeur);
			break;

		case VARIABLE:
			printf("%s", expr->variable);
			break;

		case AFFECTATION:
			print_indent(indent);
			printf("%s <- ", expr->gauche->variable);
			afficher_ast_indent(expr->droite, indent);
			printf(";\n");
			break;

		case N_LIRE:
			print_indent(indent);
			printf("Lire(%s);\n", expr->gauche->variable);
			break;

		case N_ECRIRE:
			print_indent(indent);
			printf("Ecrire(");
			afficher_ast_indent(expr->gauche, indent);
			printf(");\n");
			break;

		case N_SI:
			print_indent(indent);
			printf("Si(");
			afficher_ast_indent(expr->gauche, indent);
			printf(") {\n");
			afficher_ast_indent(expr->droite->gauche, indent + 1);
			print_indent(indent);
			printf("}");
			if(expr->droite->droite != NULL){
				printf(" Sinon {\n");
				afficher_ast_indent(expr->droite->droite, indent + 1);
				print_indent(indent);
				printf("}");
			}
			printf("\n");
			break;

		case N_TQ:
			print_indent(indent);
			printf("tant que (");
			afficher_ast_indent(expr->gauche, indent);
			printf(") {\n");
			afficher_ast_indent(expr->droite, indent + 1);
			print_indent(indent);
			printf("}\n");
			break;

		case CONDITION:
			if (!est_feuille(expr))
			{
				afficher_ast_indent(expr->gauche, indent);
				if(expr->droite != NULL){
					aff_nat_op_bool(expr->op_bool);
					afficher_ast_indent(expr->droite, indent);	
				}
			}
			
			break;
		case N_OPBOOL:
			afficher_ast_indent(expr->gauche, indent);
			aff_nat_op_comp(expr->op_comp);
			afficher_ast_indent(expr->droite, indent);
			break;

		case N_SEPINST:
			if(!est_feuille(expr)){
				afficher_ast_indent(expr->gauche, indent);
				if(expr->droite != NULL)
					afficher_ast_indent(expr->droite, indent);
			}
			break;

		default:
			print_indent(indent);
			printf("Cas non pris en charge dans l'affichage\n");
			break;
	}
}

resultat_lecture evaluation(Ast expr, Symbole_tab *ST_ARBRE, double *resultat) {
	if(expr==NULL){printf("ast NULL dans evaluation\n");return ERREUR_INTERPRETATION;}
      // A COMPLETER !
	  double vg, vd;
	  TypeAst nature;
	  //  TypeOperateur opType;
	  resultat_lecture res = get_ast_type(expr, &nature);
	  if(res != OK) return res;

	 if(debug) printf("\nNature: %s\n", get_ast_nature(expr));
	  if (res != OK){
		printf("erreur d'evaluation\n");
		// return -1; // erreur d'évaluation: on essayera une meilleure approche plus tard!
		return res;
	  }
	  switch (nature)
	  {
		case VALEUR: *resultat = expr->valeur; return OK; // feuille
		case VARIABLE:
			if(debug)	afficher_tab(*ST_ARBRE); 
			return valeur_variable(*ST_ARBRE, expr->variable, resultat); // feuille
		
		case OPERATION: // operateur
			res = evaluation(expr->gauche, ST_ARBRE, &vg);
			if(res != OK) return res;

			res = evaluation(expr->droite, ST_ARBRE, &vd);
			if(res != OK) return res;
			
			return calculer(vg, vd, expr->operateur, resultat);

		default:
			printf("Operation impossible\n");
			return ERREUR_INTERPRETATION ;
	}
}

//fonction principale d'interpretation
resultat_lecture interpreter(Ast expr, Symbole_tab *ST_ARBRE){
	resultat_lecture res = OK;
	
	if(debug)printf("Interpretation de l'arbre de nature: %s\n", get_ast_nature(expr));
	if(expr == NULL)
		return ERREUR_INTERPRETATION;

	if(debug)	afficher_ast(expr);

	switch (expr->nature)
	{
		case N_SEPINST:
			if(!est_feuille(expr)){
				res = interpreter(expr->gauche, ST_ARBRE);
				if(res != OK) return res;
			
				if(debug)	printf("On continue sur la seq_inst suivante\n");
				
				if(expr->droite != NULL)
					return interpreter(expr->droite, ST_ARBRE);
			}  
			
			return res;
		
		case AFFECTATION: 	return interpreter_aff(expr, ST_ARBRE);

		case N_LIRE: 		return interpreter_lire(expr, ST_ARBRE);

		case N_ECRIRE: 		return interpreter_ecrire(expr, ST_ARBRE);
		
		case N_SI: 			return interpreter_si(expr, ST_ARBRE);

		case N_TQ: 			return interpreter_tq(expr, ST_ARBRE);
		
		default:			return ERREUR_INTERPRETATION;

	}
}

resultat_lecture interpretation(Ast expr, Symbole_tab *ST_ARBRE){

	resultat_lecture res = interpreter(expr, ST_ARBRE);

	if(debug)	afficher_msg_erreur(res);
	if(res != OK) return res;

	if(debug)	afficher_tab(*ST_ARBRE);
	return res;
}


// ------------------------------------------------------------ //

resultat_lecture interpreter_tq(Ast expr, Symbole_tab *ST_ARBRE){
	resultat_lecture res = OK;
	
	if(expr == NULL || expr->gauche == NULL || expr->droite == NULL){
		printf("Erreur d'interpretation de la condition si\n");
		return ERREUR_INTERPRETATION;
	}

	double max_boucle = get_max_condition(expr->gauche, *ST_ARBRE);
	if(debug) printf("max_boucle: %f\n", max_boucle);

	while (evaluer_condition(expr->gauche, *ST_ARBRE) && res == OK){
		// si vrai
		if(debug)	printf("Condition Vrai\n");

		res = interpreter(expr->droite, ST_ARBRE);
		
		if(res != OK) return res;
		
		max_boucle--;
		if(max_boucle < 0)  res = ERREUR_BOUCLE_INFINIE;
	}
	
	return res;
}

resultat_lecture interpreter_si(Ast expr, Symbole_tab *ST_ARBRE){
	if(expr == NULL || expr->gauche == NULL || expr->droite == NULL || expr->droite->gauche == NULL){
		printf("Erreur d'interpretation de la condition si\n");
		return ERREUR_INTERPRETATION;
	}

	if(debug)printf("Interpretation de la condition SI\n");
	int v = evaluer_condition(expr->gauche, *ST_ARBRE);
	
	if(v == -1) return ERREUR_INTERPRETATION;
	if(v){
		// si vrai
		return interpreter(expr->droite->gauche, ST_ARBRE);
	} else {
		// si faux
		if(expr->droite->droite != NULL)
			return interpreter(expr->droite->droite, ST_ARBRE);
		return OK;
	}
}

int evaluer_condition(Ast expr, Symbole_tab ST_ARBRE){
	if(debug)	printf("Evaluation de la condition\n");
	
	switch (expr->nature)
	{
		case CONDITION:
			switch (expr->op_bool)
			{
				case N_ET: return evaluer_op_bool(expr->gauche, ST_ARBRE) && evaluer_condition(expr->droite, ST_ARBRE);

				case N_OU:	return evaluer_op_bool(expr->gauche, ST_ARBRE) || evaluer_condition(expr->droite, ST_ARBRE);
				
				default:	return -1;
			}
			break;

		case N_OPBOOL:		return evaluer_op_bool(expr, ST_ARBRE);
		
		case N_NON:			return !(evaluer_condition(expr->gauche, ST_ARBRE));
			
		default:			return 0;
	}
}

int evaluer_op_bool(Ast expr, Symbole_tab ST_ARBRE){
	double valg, vald;
	if(expr->gauche == NULL || expr->droite == NULL) return -1; // Erreur

	resultat_lecture res = evaluation(expr->gauche, &ST_ARBRE, &valg);

	if(res != OK) return res;
	res = evaluation(expr->droite, &ST_ARBRE, &vald);

	if(res != OK) return res;
	switch (expr->op_comp)
	{
	case N_EQ: 		return valg == vald;
	case N_NEQ: 	return valg != vald;
	case N_SUP: 	return valg >  vald;
	case N_SEQ: 	return valg >= vald;
	case N_INF: 	return valg <  vald;
	case N_IEQ: 	return valg <= vald;	
	default:		return -1;
	}
}

resultat_lecture interpreter_aff(Ast expr, Symbole_tab *ST_ARBRE){
	if(expr == NULL || expr->gauche == NULL || expr->droite == NULL){
		printf("Erreur d'interpretation de l'affectation\n");
		return ERREUR_INTERPRETATION;
	}
	
	char *idf = expr->gauche->variable;
	double v;
	resultat_lecture res = evaluation(expr->droite, ST_ARBRE, &v);
	
	if(res != OK) return res;

	Couple c = new_couple(idf, v);
	inserer(ST_ARBRE, c);
	
	
	if(debug)afficher_tab(*ST_ARBRE);

	return OK;
}

resultat_lecture interpreter_lire(Ast expr, Symbole_tab *ST_ARBRE){
	if(expr == NULL || expr->gauche == NULL){
		printf("Erreur d'interpretation de l'affectation\n");
		return ERREUR_INTERPRETATION;
	}
	
	double v = lire();
	char *idf = expr->gauche->variable;

	if(debug) printf("%s = %f\n", idf, v);

	Couple c = new_couple(idf, v);
	inserer(ST_ARBRE, c);
	
	if(debug)afficher_tab(*ST_ARBRE);
	return OK;
}

resultat_lecture interpreter_ecrire(Ast expr, Symbole_tab *ST_ARBRE){
	if(debug)printf("dans interpreter ecrire\n");
	// afficher_tab(ST_ARBRE);
	if(expr == NULL || expr->gauche == NULL){
		printf("Erreur d'interpretation de l'ecriture\n");
		return ERREUR_INTERPRETATION;
	}

	// if(debug)printf("Ecriture de %s\n", expr->gauche->variable);


	double v ;
	resultat_lecture res = evaluation(expr->gauche, ST_ARBRE, &v);
	
	
	if(res != OK) return res;
	
	if(expr->gauche->nature == VARIABLE){
		printf("la valeur de la variable %s =", expr->gauche->variable);
	} else {
		afficher_ast(expr);
		printf("L'affichage donne:");
	}
	ecrire(v);
	return OK;
}

