// les Includes

// #include "lecture_caracteres.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

#include "analyse_syntaxique.h"
#include "ast_construction.h"
#include "ast_parcours.h"
#include "table_symbole.h"
#include "debug.h"

Symbole_tab *TS;         // un tableau pour les affectations et leur valeurs.

double evaluer(double valg, Nature_Lexeme op, double vald);
resultat_lecture rec_op(Nature_Lexeme *operateur);
resultat_lecture Rec_eaep(double *resultat);

// ----------------------TP 3------------------------------//
resultat_lecture rec_eag(Ast *A);
resultat_lecture rec_seq_terme(Ast *A);
resultat_lecture rec_terme(Ast *A);
resultat_lecture rec_suite_seq_terme(Ast *A1, Ast *A);
resultat_lecture rec_seq_facteur(Ast *A);
resultat_lecture rec_suite_seq_facteur(Ast *A1, Ast *A);
resultat_lecture rec_facteur(Ast *A);

resultat_lecture rec_op1(TypeOperateur *op);
resultat_lecture rec_op2(TypeOperateur *op);

// ------------------------- Projet ---------------------------- //
resultat_lecture rec_prog(Ast *A);
resultat_lecture rec_seq_aff(Ast *A);
resultat_lecture rec_aff(Ast *A);
resultat_lecture rec_seq_inst(Ast *A);
resultat_lecture rec_suite_seq_inst(Ast *A1, Ast *A);
resultat_lecture rec_inst(Ast *A);


resultat_lecture rec_lecture(Ast *A);
resultat_lecture rec_ecrire(Ast *A);

resultat_lecture rec_si(Ast *A);
resultat_lecture rec_condition(Ast *A);
resultat_lecture rec_suite_condition(Ast op_gauche, Ast *A);
resultat_lecture rec_op_bool(Ast *A);

resultat_lecture get_noeud_op_comp(Nature_Lexeme nature, TypeOpComp *op);

resultat_lecture rec_tq(Ast *A);
// resultat_lecture rec_op_comp(Ast *A);

// -------------- avant debut projet -------------------- //
resultat_lecture calcul(Nature_Lexeme operateur, double *result, double new)
{
    switch (operateur)
    {
    case PLUS:
        printf("%f + %f= ", *result, new);
        *result += new; 

        break;
    case MOINS:
        printf("%f - %f= ", *result, new);
        *result -= new; 

        break;
    case MUL:
        printf("%f x %f=", *result, new);
        *result *= new;

        break;
    case DIV:
        printf("%f / %f =", *result, new);
        if(new != 0)
            *result /= new;
        else{
            printf("Erreur division par 0\n");
            return ERREUR_SEMANTIQUE;
        } 
        
        break;
    case MODULO:
        printf("%f %% %f =", *result, new);
        if(new != 0)
            *result = fmod(*result, new);
        else{
            printf("Erreur division par 0\n");
            return ERREUR_SEMANTIQUE;
        } 
        
        break;
    default:
        printf("Opértaeur inconnu!\n");
        return ERREUR_LEXICALE;
        break;
    }
    printf("%f\n", *result);
    return OK;
}


resultat_lecture Rec_eaep(double *resultat)
{
    double valg = 0, vald = 0;
    Nature_Lexeme op;
    resultat_lecture res = OK;

    if(debug) afficher(lexeme_courant());
    switch (lexeme_courant().nature)
    {
    
    case ENTIER:
        *resultat = lexeme_courant().valeur;
        res = avancer();
        
        break;
    case PARO:
        res = avancer() ;
        if(res != OK)
            return res;

        res = Rec_eaep(&valg);
        if(res != OK)
            return res;

        if (lexeme_courant().nature != PARF){
            res = rec_op(&op);
            if(res != OK)
            return res;
        
            res = Rec_eaep(&vald);
            if(res != OK)
                return res;

            *resultat = evaluer(valg, op, vald);
        } else *resultat = valg;
        
        
        if(lexeme_courant().nature == PARF)
            return avancer();
        else
            return ERREUR_SYNTAXIQUE;
        break;

    default:
        return ERREUR_SYNTAXIQUE;
    }
    return res;
}


resultat_lecture rec_op(Nature_Lexeme *operateur) {
    switch (lexeme_courant().nature)
    {
    case PLUS: *operateur   = PLUS;     break;
    case MOINS: *operateur  = MUL;      break;
    case MUL: *operateur    = MUL;      break;
    case DIV: *operateur    = DIV;      break;
    case MODULO: *operateur = MODULO;   break;
    
    default:
        return ERREUR_SYNTAXIQUE;
    }

    return avancer();
};

double evaluer(double valg, Nature_Lexeme op, double vald){
    double resultat = valg;
    resultat_lecture res = OK;    

    res = calcul(op, &resultat, vald);

    if(res != OK){
        afficher_msg_erreur(res);
        return -10000000;
    }
    return resultat;
}

// ---------------------- TP 3 ----------------------
void analyser_EAG(char *nom_fichier){
    // *resultat = 0; // initialisation du resultat pour eviter des erreurs de segmentations.
    
    resultat_lecture res = demarrer(nom_fichier);
    Ast arbre = NULL;

    // ---------------- TABLE DES SYMBOLES ------------------- //

    Symbole_tab *TS_Arbre = init_table_vide();
    TS = init_table_vide();

    // ------------------------------------------------------- //

    res = rec_prog(&arbre);

    if(debug) afficher_msg_erreur(res);

    if(debug) afficher_ast(arbre);
    if (res == OK)
    {
        if(debug)printf("On interprete\n");
        // interpreter(arbre);
        if(debug) afficher_ast(arbre);
        interpretation(arbre, TS_Arbre) ;
        if(debug)afficher_msg_erreur(res);
    }

    liberer_ast(&arbre);
    liberer_table(TS);
    liberer_table(TS_Arbre);
    arreter();
}

//------------------------- Programme --------------------------------//
resultat_lecture analyser_prog(char *nom_fichier, Ast *arbre){
    resultat_lecture res = demarrer(nom_fichier);
    res = rec_prog(arbre);
    arreter();
    return res;
}

resultat_lecture rec_prog(Ast *A){
    // un ensemble d'instruction
    return rec_seq_inst(A);
}

//--------------------------- Instructions -------------------------//

resultat_lecture rec_seq_inst(Ast *A){
    Ast A1 = NULL;
    resultat_lecture res = rec_inst(&A1);
    if(res != OK) return res;
    
    return rec_suite_seq_inst(&A1, A);
}

resultat_lecture rec_suite_seq_inst(Ast *A1, Ast *A){
    Ast A2 = NULL;
    resultat_lecture res = OK;
    if(debug)printf("On est sur la suite seq inst\n");

    if(lexeme_courant().nature == SEPINST){
        if(debug)afficher(lexeme_courant());
        res = avancer();    //pourquoi  erreur syntaxique ici(a<-12;)
        if(debug)printf("apres avancer\n");
        if(res != OK) return res;

        res = rec_seq_inst(&A2);

        if(res != OK) return res;
        *A = creer_seq_inst(*A1, A2); 
        
    } else {
        // epsilon pas de fils droit    
        *A = *A1; 
    }

    return res; // c'était res qui y était
}

resultat_lecture rec_inst(Ast *A){
    // printf("Dans la seq instruction\n");
    //resultat_lecture res = OK;
    switch (lexeme_courant().nature)
    {
        case CHAINE: return rec_seq_aff(A); // c'est une affectation  
            
        case LIRE:   return rec_lecture(A);
            
        case ECRIRE: return rec_ecrire(A);

        case SI:     return rec_si(A);

        case TQ:     return rec_tq(A);
        
        case ENTIER: return rec_eag(A);  // juste une EAG

        case ACCF:   if(debug)    printf("OK!\n"); return OK;
            
        default:     return ERREUR_SYNTAXIQUE;
    }
}

// ---------------------- AFFECTATION ------------------------ //
resultat_lecture rec_seq_aff(Ast *A){
    // ma grammaire pour seq aff
    /*
        seq_aff:    aff seq_aff
        seq_aff:    epsilon
    */

    switch (lexeme_courant().nature)
    {
    case CHAINE: return rec_aff(A);
    
    default:
        return OK; // epsilon
    }
}

resultat_lecture rec_aff(Ast *A){

    /*
        aff:        idf <- eag ;
    */

    if(debug)printf("Dans la rec_aff\n");
    // double v;
    char idf[100];//une variable doit avoir au max 100 caracteres
    Ast Ag = NULL, Ad = NULL;
    resultat_lecture res = OK;
    
    if(debug)   afficher(lexeme_courant());
    if(lexeme_courant().nature != CHAINE){
        printf("Une chaine de caractere attendu\nErreur d'affectation\n");
        return ERREUR_SYNTAXIQUE;
    }
    if(debug)   printf("Duplication de l'idf\n");
    
    strcpy(idf, lexeme_courant().chaine);

    if(debug)   printf("Idf = %s\n", idf);
    Ag = creer_variable(idf);
    res = avancer();

    if(res != OK){ return res; }
    if(lexeme_courant().nature != AFF) return ERREUR_SYNTAXIQUE;
    res = avancer();
    
    if(debug) afficher_msg_erreur(res);
    if(res != OK){return res; }
    

    res = rec_eag(&Ad);
    if(debug) afficher_msg_erreur(res);
    if (res != OK){return res; }
    
    if(debug)afficher(lexeme_courant());

    *A = creer_aff(Ag, Ad);

    if(lexeme_courant().nature != SEPINST) { return ERREUR_SYNTAXIQUE; }// une affectation est forcement suivie d'un point virgule.
    // return avancer();
    
    return res;   // pour commencé  avec un ';'
}

// ----------------------- Lire et Ecrire -------------------- //

resultat_lecture rec_lecture(Ast *A){
    Ast Ag;
    resultat_lecture res = avancer();
    char idf[100];
    
    if(res != OK) return res;
    if(lexeme_courant().nature != PARO) return ERREUR_SYNTAXIQUE;
    
    res = avancer();
    if(res != OK) return res;
    if(lexeme_courant().nature != CHAINE) return ERREUR_SYNTAXIQUE;
    
    strcpy(idf, lexeme_courant().chaine);
    Ag = creer_variable(idf);
    res = avancer();
    if(res != OK) return res;

    *A = creer_lire(Ag);

    if(lexeme_courant().nature != PARF) return ERREUR_SYNTAXIQUE;
    return avancer();
}

resultat_lecture rec_ecrire(Ast *A){
    // Ecrire (X) ;
    Ast Ag;
    resultat_lecture res = avancer();
    
    if(res != OK) return res;
    if(lexeme_courant().nature != PARO) return ERREUR_SYNTAXIQUE;
    
    if(debug)afficher(lexeme_courant());
    
    res = avancer();
    if(res != OK) return res;
    
    if(debug)afficher(lexeme_courant());
    
    res = rec_eag(&Ag);
    if(res != OK) return res;

    if(debug)afficher(lexeme_courant());

    if(lexeme_courant().nature != PARF) return ERREUR_SYNTAXIQUE;

    *A = creer_ecrire(Ag);    
    return avancer();   
}

// ----------------------- SI condition -------------------- //

resultat_lecture rec_si(Ast *A){
    // SI (condition) { seq_inst } SINON { seq_inst }
    if(debug)printf("Dans la condition Si:\n");
    resultat_lecture res = avancer();
    Ast condition = NULL, alors = NULL, sinon = NULL;
    
    if(res != OK) return res;
    if(lexeme_courant().nature !=  PARO) return ERREUR_SYNTAXIQUE; // Cette condition est aussi vérifier par rec_condition
    
    res = rec_condition(&condition); // doit sortir après avoir avancé sur le PARF
    if(res != OK) return res;
    if(debug)printf("la condition a ete creee\n");

    if(lexeme_courant().nature !=  ACCO) return ERREUR_SYNTAXIQUE;
    res = avancer();

    if(debug)printf("Creation du block alors\n");

    res = rec_seq_inst(&alors);

    if(debug)afficher(lexeme_courant());
    if(res != OK) return res;
    if(debug)printf("le block alors a ete cree\n");

    if(debug)afficher(lexeme_courant());
    if(lexeme_courant().nature !=  ACCF) return ERREUR_SYNTAXIQUE;
    res = avancer();
    if(debug)printf("Creation du block sinon\n");
    
    if(debug)afficher(lexeme_courant());
    if(res != OK) return res;
    
    if(lexeme_courant().nature !=  SINON){
        *A = creer_si(condition, alors, sinon);
        return res; // pas besoin de sinon
    }

    res = avancer();
    if(res != OK) return res;
    if(lexeme_courant().nature !=  ACCO) return ERREUR_SYNTAXIQUE;
    res = avancer();

    if(debug)afficher(lexeme_courant());
    res = rec_seq_inst(&sinon);


    if(res != OK) return res;
    if(debug)afficher(lexeme_courant());
    if(debug)printf("le block sinon a ete cree\n");

    if(lexeme_courant().nature !=  ACCF) return ERREUR_SYNTAXIQUE;

    *A = creer_si(condition, alors, sinon);
    
    return avancer();
}

resultat_lecture rec_condition(Ast *A){
    // condition: (op_bool suite_condition)
    if(debug)printf("Dans la condition:\n");
    resultat_lecture res = OK;
    Ast op_gauche = NULL;
    
    if(lexeme_courant().nature !=  PARO) return ERREUR_SYNTAXIQUE;

    res = rec_op_bool(&op_gauche);
    if(res != OK) return res;

    if(lexeme_courant().nature != PARF) { res = rec_suite_condition(op_gauche, A); }
    else {
        *A = op_gauche; // pas de suite
    }
    if(res != OK) return res;

    if(lexeme_courant().nature !=  PARF) return ERREUR_SYNTAXIQUE;
    return avancer();
}

resultat_lecture rec_suite_condition(Ast Ag, Ast *A){
    // suite_condition: Et/OU op_bool suite_condition | epsilon
    resultat_lecture res = OK;
    Ast Ad = NULL, As = NULL;
    TypeOpBool op;
    if(debug)printf("Suite de la condition\n");
    if(lexeme_courant().nature == ET || lexeme_courant().nature == OU)
    {
        if(lexeme_courant().nature == ET) op = N_ET;
        else op = N_OU;

        res = rec_op_bool(&Ad);
        
       if(debug) printf("\nApres Boule\n");
        
       if(debug)  afficher(lexeme_courant());     

            
       if(debug)  printf("\nSuite de la condition\n");
        res = rec_suite_condition(Ad, &As);
        if(res != OK) return res;        
        
        *A = creer_condition(Ag, op, As);
    } else { *A = Ag; /* epsilon */ }
    
   if(debug)  afficher(lexeme_courant());
    return res;
}

resultat_lecture rec_op_bool(Ast *A){
    // op_bool: EAG op_comp EAG | !(EAG op_comp EAG)
   if(debug)  printf("Dans rec op_Bool\n");

    if(debug) afficher(lexeme_courant());
    resultat_lecture res = avancer();
    Ast op_gauche = NULL, op_droite = NULL, A_NON = NULL;
    TypeOpComp op_comp;
    int non = 0;
    
    if(res != OK) return res;
    if(debug) afficher(lexeme_courant());

    if(lexeme_courant().nature == NON){
       if(debug)  printf("Il y a un Non Derriere\n");
        res = avancer();
       if(debug)  afficher(lexeme_courant());
        if(res != OK) return res;

       if(debug)  printf("Il y a un Non Derriere\n");
        if(lexeme_courant().nature != PARO) return ERREUR_SYNTAXIQUE;

        non = 1; //non=TRUE
        res = avancer(); // pour avancer sur le PARO
    }

    res = rec_eag(&op_gauche);
   if(debug)  afficher(lexeme_courant());
   if(debug)  printf("Cote gauche OK\n");
    
    if(res != OK) return res;
   if(debug)  printf("Cote gauche OK\n");

   if(debug)  printf("Recuperation de la nature de l'op de comparaison\n");
    res = get_noeud_op_comp(lexeme_courant().nature, &op_comp);  // on recupère la nature de l'operateur de comparaison
 
   if(debug)  afficher(lexeme_courant());
    res = avancer();
    if(res != OK) return res;

  if(debug)   printf("\nOK!\n");
  if(debug)    afficher(lexeme_courant());
    res = rec_eag(&op_droite);
    if(res != OK) return res;

    if(non == 1){
        if(debug) printf("On cree le non\n");
       if(debug)  afficher(lexeme_courant());
        if(lexeme_courant().nature != PARF) return ERREUR_SYNTAXIQUE;

        res = avancer(); // pour avancer sur la PARF de la negation
        if(debug) afficher(lexeme_courant());

        A_NON = creer_op_bool(op_gauche, op_comp, op_droite);
        *A = creer_non(A_NON);
    } else  { *A = creer_op_bool(op_gauche, op_comp, op_droite); }

    if(debug) afficher(lexeme_courant());
    if(debug) printf("Cretation OP_BOOL FINI\n");
    return OK;
}

resultat_lecture get_noeud_op_comp(Nature_Lexeme nature, TypeOpComp *op){

    switch (nature)
    {
          case SUP:   *op =  N_SUP;     return OK;
          case INF:   *op =  N_INF ;    return OK;
          case SEQ:   *op =  N_SEQ;     return OK;
          case IEQ:   *op =  N_IEQ;     return OK;
          case EQ:    *op =  N_EQ;      return OK;
          case NEQ:   *op =  N_NEQ;     return OK;
          default:    printf("Operateur inconnu\n"); return ERREUR_SYNTAXIQUE; // On trouvera un autre moyen plus tard.
    }
}

// ----------------------- TANT QUE ---------------------- //
// tq : TQ (CONDITION) { seq_inst }

resultat_lecture rec_tq(Ast *A){
    resultat_lecture res = avancer();
    Ast Acond = NULL, Ablock = NULL;

    if(res != OK) return res;
    res = rec_condition(&Acond);

   if(debug)  printf("Condition Tq Ok\n");
    if(res != OK) return res;
  
    if(lexeme_courant().nature != ACCO) return ERREUR_SYNTAXIQUE;

    res = avancer();
    if(res != OK) return res;

    res = rec_seq_inst(&Ablock);

   if(debug)  printf("Block Tq Ok\n");

   if(debug)  afficher(lexeme_courant());
    if(res != OK) return res;

    if(lexeme_courant().nature != ACCF) return ERREUR_SYNTAXIQUE;

   if(debug)  afficher(lexeme_courant());
    *A = creer_tq(Acond, Ablock);

    return avancer();
}

// -------------------------------------------------------------- //
resultat_lecture rec_eag(Ast *A) { return rec_seq_terme(A); }

resultat_lecture rec_seq_terme(Ast *A){
   if(debug)  printf("Dans rec_seq_terme\n");
    Ast A1;
    resultat_lecture res  = rec_terme(&A1);
    if(res != OK) return res;
    return rec_suite_seq_terme(&A1, A);
}

resultat_lecture rec_suite_seq_terme(Ast *Ag, Ast *A){
   if(debug)  printf("Dans rec_suite_seq_terme\n");
    resultat_lecture res = OK;
    Ast Ad, A1;
    TypeOperateur op;

    switch (lexeme_courant().nature)
    {
        case PLUS: case MOINS:
            res = rec_op1(&op);
            if(res != OK) return res;
            
            res = rec_terme(&Ad);
            if(res != OK) return res;

            A1 = creer_operation(op, *Ag, Ad);
            return rec_suite_seq_terme(&A1, A);
        
        default: *A = *Ag; return OK; // rien, epsilon!
    }
}

resultat_lecture rec_terme(Ast *A){ return rec_seq_facteur(A);}

resultat_lecture rec_facteur(Ast *A){
    resultat_lecture res = OK;
    char idf[256];

   if(debug)  printf("Dans rec_facteur\n");
    // double val;
    // int trouver = -1;  // la fonction rechercher retourne -1 si elle ne trouve pas la variable
    
    switch (lexeme_courant().nature)
    {
        case ENTIER: 
            *A = creer_valeur(lexeme_courant().valeur);
            return avancer();

        case PARO: res = avancer();
            
            if(res != OK) return res;
            res = rec_eag(A);
            
            if(lexeme_courant().nature == PARF)
                return avancer();
            else return ERREUR_SYNTAXIQUE; // erreur syntaxe;
        // case MOINS: return rec_facteur(A);
        case CHAINE:
            strcpy(idf, lexeme_courant().chaine);
            *A = creer_variable(idf);
            return avancer();

        // case PARF: printf("Une parf sans paro\n"); return OK;
            break;
        default:
            return ERREUR_SYNTAXIQUE; // erreur de syntaxe
    }
}

resultat_lecture rec_seq_facteur(Ast *A){
    Ast A1;
    resultat_lecture res = rec_facteur(&A1);    
    if(res != OK) return res;
    
    return rec_suite_seq_facteur(&A1, A);
}

resultat_lecture rec_suite_seq_facteur(Ast *Ag, Ast *A){
    resultat_lecture res = OK;
    Ast Ad, A1;
    TypeOperateur op;
    // Nature_Lexeme op = rec_op2();
    switch (lexeme_courant().nature)
    {
        case MUL: case DIV: case MODULO:
            res = rec_op2(&op);
            // res = avancer();
            
            if(res != OK) return res;
            res = rec_facteur(&Ad);
            
            if(res != OK) return res;
            A1 = creer_operation(op, *Ag, Ad);
            return rec_suite_seq_facteur(&A1, A);
        
        default:
            *A = *Ag; return OK; // rien, epsilon!
    }
}

resultat_lecture rec_op1(TypeOperateur *op){
   if(debug)  afficher(lexeme_courant());
    switch (lexeme_courant().nature)
    {
        case PLUS: *op = N_PLUS; return avancer();
        case MOINS: *op = N_MOINS; return avancer();
        case MUL: case DIV: case MODULO: return ERREUR_SYNTAXIQUE;
        
        default: return ERREUR_LEXICALE;
    }
}

resultat_lecture rec_op2(TypeOperateur *op){
   if(debug)  afficher(lexeme_courant());
    switch (lexeme_courant().nature)
    {
        case MUL: *op = N_MUL;      return avancer();
        case DIV: *op = N_DIV;      return avancer();
        case MODULO: *op = N_MODULO;   return avancer();
        case PLUS: case MOINS: return ERREUR_SYNTAXIQUE;
        
        default: return ERREUR_LEXICALE;
    }
}
// reconnaissance d'un nouveau lexeme
   // etat initial : le caractere courant est soit separateur 
   //                soit le 1er caractere d'un lexeme
   // etat final : 
   //       - un nouveau lexeme est reconnu dans lexeme_en_cours
   //       - le caractere courant est soit la fin de fichier, 
   //		soit un separateur,  soit le 1er caractere d'un lexeme