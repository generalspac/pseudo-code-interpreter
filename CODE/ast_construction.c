#include <stdlib.h>
#include <stdio.h> 
#include <string.h>

#include "ast_construction.h"
#include "debug.h"

void liberer_ast(Ast *A) {
      if (*A == NULL) {
          return;
      }
  
      if ((*A)->gauche != NULL) {
          liberer_ast(&(*A)->gauche);
      }
  
      if ((*A)->droite != NULL) {
          liberer_ast(&(*A)->droite);
      }
  
      free(*A);
      *A = NULL;
  }
  
Ast creer_operation(TypeOperateur opr , Ast opde_gauche , Ast opde_droit) 
{
      if(debug)printf("Creation d'une op\n");
      Ast expr ;
      expr = (Ast) malloc (sizeof(NoeudAst));
      if(expr==NULL){
            printf("erreur d'allocation , Ast creer_operation\n");
            return NULL;
      }
      expr->nature = OPERATION;
      expr->operateur = opr;
      expr->gauche = NULL;
      expr->droite = NULL;
      expr->valeur = 0;

      if (opde_gauche == NULL || opde_droit == NULL) {
         printf("ERREUR_EXPRESSION dans la création de l'operation\n") ;
         free(expr);
         return NULL;
      } else {
         expr->gauche = opde_gauche;
         expr->droite = opde_droit;
      } ;
      return expr ;
}

Ast creer_valeur(double val) { 
     if(debug) printf("Creation d'une valeur\n");
      Ast expr ;
      expr = (Ast) malloc (sizeof(NoeudAst));
       if(expr==NULL){
            printf("erreur d'allocation , Ast creer_valeur\n");
            return NULL;
      }
      expr->nature = VALEUR;
      expr->valeur = val;

      expr->gauche = NULL;
      expr->droite = NULL;

      return expr ;
}

Ast creer_variable(char *var) { 
      if(debug)printf("Creation d'une var %s\n", var);
      Ast expr = NULL;
      expr = (Ast) malloc(sizeof(NoeudAst));
      if(expr == NULL) printf("Not OK creer_variable\n");
      expr->nature = VARIABLE;

      if(debug) printf("avant transfert de la chaine dans arbre(creer_variable\n)");
      strcpy(expr->variable, var);
      expr->gauche = NULL;
      expr->droite = NULL;
      expr->valeur = 0;
      return expr ;
}

Ast creer_seq_inst(Ast opde_gauche, Ast opde_droit){
      if(debug)printf("Creation d'une seq\n");
      Ast expr ;
      expr = (Ast) malloc (sizeof(NoeudAst));
       if(expr==NULL){
            printf("erreur d'allocation , Ast creer_seq_inst\n");
            return NULL;
      }
      expr->nature = N_SEPINST;

      expr->gauche = NULL;
      expr->droite = NULL;
      expr->valeur = 0;
      
      if (opde_gauche == NULL) {
         printf("ERREUR_EXPRESSION\n") ;
         free(expr);
	   return NULL;
       } else {
         expr->gauche = opde_gauche;
         expr->droite = opde_droit;
      }
      return expr ;
}

Ast creer_fin_inst(){
      Ast ast = (Ast) malloc(sizeof(NoeudAst));
       if(ast==NULL){
            printf("erreur d'allocation , Ast creer_fin_inst\n");
            return NULL;
      }
      ast->nature = N_SEPINST;

      ast->gauche = NULL;
      ast->droite = NULL;
      ast->valeur = 0;
      return ast;
}

// cree un noeud N_AFF de fils Ag et Ad.
Ast creer_aff(Ast opde_gauche, Ast opde_droit){
      if(debug)printf("Creation d'une aff de la var %s \n", opde_gauche->variable);
      Ast expr ;
      
      expr = (Ast) malloc (sizeof(NoeudAst));
       if(expr==NULL){
            printf("erreur d'allocation , Ast creer_aff\n");
            return NULL;
      }
      expr->nature = AFFECTATION;
      
      expr->gauche = NULL;
      expr->droite = NULL;
      expr->valeur = 0;

      if (opde_gauche == NULL || opde_droit == NULL) {
         printf("ERREUR_EXPRESSION sur la création AFF\n");
         free(expr);
         return NULL;
      } else {
         expr->gauche = opde_gauche;
         
         expr->droite = opde_droit;
      } ;
      if(debug) printf("fin de creation affectation\n");
      return expr ;
}

Ast creer_ecrire(Ast ast) {
      if(debug)printf("creation ecrire\n");
      if(ast == NULL){
       printf("ERREUR_EXPRESSION sur la creation de l'ecriture\n") ;
	 return NULL;
      }
      if(debug)printf("Creation d'une ecr de la var %s \n", ast->variable);
      Ast A = (Ast) malloc (sizeof(NoeudAst));
       if(A==NULL){
            printf("erreur d'allocation , Ast creer_ecrire\n");
            return NULL;
      }
      A->gauche = ast;
      // A->droite = creer_fin_inst();
      A->nature = N_ECRIRE;
      A->droite = NULL;
       if(debug)printf("fin creation ecrire\n");
      return A;
}

Ast creer_lire(Ast ast){
      if(debug)printf("Creation d'une lect de la var %s \n", ast->variable);
      Ast A = (Ast) malloc (sizeof(NoeudAst));
       if(A==NULL){
            printf("erreur d'allocation , Ast creer_lire\n");
            return NULL;
      }
      A->nature = N_LIRE;
      A->gauche = ast;
      // A->droite = creer_fin_inst(); 
      A->droite = NULL;
      return A;
}

// ---------------------------------- Si Condition --------------------------------- //
Ast creer_si(Ast condition, Ast alors, Ast sinon){
      if(condition == NULL || alors == NULL){
            printf("Pas de CONDINTION ou N_TRUE\n");
            return NULL;
      }
      
      Ast ast = (Ast) malloc(sizeof(NoeudAst));
       if(ast==NULL){
            printf("erreur d'allocation , Ast creer_si(noeud principale\n");
            return NULL;
      }
      Ast ast_alors_sinon = (Ast) malloc(sizeof(NoeudAst)); // sera aussi supprimer lors de la libération de la mémoire
       if(ast_alors_sinon==NULL){
            printf("erreur d'allocation , Ast creer_si(noeud ast_alors_sinon \n");
            return NULL;
      }

      ast->nature = N_SI;
      ast->gauche = condition;
      ast_alors_sinon->gauche = alors;
      ast_alors_sinon->droite = sinon;
      ast->droite = ast_alors_sinon;

      if(debug)printf("Fin creation Si\n");
      
      return ast;
}


Ast creer_condition(Ast op_gauche, TypeOpBool op, Ast op_droite){
      Ast A = (Ast) malloc(sizeof(NoeudAst));
       if(A==NULL){
            printf("erreur d'allocation , Ast creer_condition\n");
            return NULL;
      }
      if(op_gauche == NULL || op_droite == NULL){
            if(debug)printf("Pas de Suite de condition\n");
      }

      A->valeur = -1;
      // A->operateur = NULL;
      A->nature = CONDITION;
      A->droite = op_droite;
      A->gauche = op_gauche;
      A->op_bool = op; //operation ET OU NON

      return A;
}

Ast creer_op_bool(Ast op_gauche, TypeOpComp op, Ast op_droite){
      if(debug)printf("Creation d'une operation de condition booleenne\n");
      Ast A = (Ast) malloc(sizeof(NoeudAst));
       if(A==NULL){
            printf("erreur d'allocation , Ast creer_op_bool\n");
            return NULL;
      }
      if(op_gauche == NULL || op_droite == NULL){
            if(debug)printf("Pas d'élement de comparaison\n");
            return NULL;
      }

      A->valeur = -1;
      // A->operateur = NULL;
      A->nature = N_OPBOOL;
      A->droite = op_droite;
      A->gauche = op_gauche;
      A->op_comp =  op; // operateur de comparaison

      if(debug)printf("fin de la creation\n");
      return A;
}

// ---------------------------------- Tant que ------------------------------------ //
Ast creer_tq(Ast condition, Ast faire){
      if(condition == NULL || faire == NULL){
           if(debug) printf("Pas de CONDINTION ou N_TRUE\n");
            return NULL;
      }
      
      Ast ast = (Ast) malloc(sizeof(NoeudAst));
       if(ast==NULL){
            printf("erreur d'allocation , Ast creer_tq\n");
            return NULL;
      }

      ast->nature = N_TQ;
      ast->gauche = condition;
      ast->droite = faire;

      if(debug)printf("Fin creation Tq\n");
      
      return ast;     
}

Ast creer_non(Ast A){
      Ast ast = (Ast) malloc(sizeof(NoeudAst));
       if(ast==NULL){
            printf("erreur d'allocation , Ast creer_non\n");
            return NULL;
      }

      ast->nature = N_NON;
      ast->gauche = A;
      ast->droite=NULL;
      
      return ast;
}