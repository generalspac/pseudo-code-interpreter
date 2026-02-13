/* ------------------------------------------------------------------------
-- paquetage analyse_lexicale
--
-- analyse lexicale d'une expression arithmetique
--
-- J-P. Peyrin, G. Serasset (version initiale) : janvier 1999
-- P.Habraken - 18/01/2010
--
-- 10 juin 2006 - PH :
-- remplacement record a champs variants par record "plat"
-- remplacement traitement iteratif par automate
--
-- Aout 2016 - LM : version C
------------------------------------------------------------------------ */

#include <string.h>
#include <stdlib.h> 
#include <stdio.h>
#include <stdbool.h> 

#include "analyse_lexicale.h"
#include "lecture_caracteres.h"
#include "debug.h"

   /* --------------------------------------------------------------------- */


/* Les variables et fonctions locales au module */

   Lexeme lexeme_en_cours ;	/* le lexeme courant */

   void ajouter_caractere (char *s, char c);
   Nature_Caractere nature_caractere (char c);
   int est_separateur(char c ) ;
   int est_separateur_inst(char c ) ;
   int est_chiffre(char c ) ;
   int est_symbole(char c ) ;
   int est_virgule(char c ) ;
   int est_lettre(char c);
   int est_par(char c);
   int est_acc(char c);
   int est_car_reserve(char c) { return (est_acc(c) || est_par(c) || est_virgule(c) || est_symbole(c));}
   resultat_lecture reconnaitre_lexeme();
   double convertir_chaine_en_nombre(const char *chaine);

   Nature_Lexeme get_nature_chaine(char *str);

   /* --------------------------------------------------------------------- */

   resultat_lecture demarrer(char *nom_fichier) {
      demarrer_car(nom_fichier);
      return avancer();
   }
   /*--------------------------------------------*/
   int est_commentaire(char c);

   /* --------------------------------------------------------------------- */

   resultat_lecture avancer() { return reconnaitre_lexeme(); } 

   /* --------------------------------------------------------------------- */

   Lexeme lexeme_courant() { return lexeme_en_cours; } 

   /* --------------------------------------------------------------------- */

   int fin_de_sequence() { return lexeme_en_cours.nature == FIN_SEQUENCE; }

   /* --------------------------------------------------------------------- */

   void arreter() { arreter_car() ; }

   /* --------------------------------------------------------------------- */


   // reconnaissance d'un nouveau lexeme
   // etat initial : le caractere courant est soit separateur 
   //                soit le 1er caractere d'un lexeme
   // etat final : 
   //       - un nouveau lexeme est reconnu dans lexeme_en_cours
   //       - le caractere courant est soit la fin de fichier, 
   //		soit un separateur,  soit le 1er caractere d'un lexeme

resultat_lecture reconnaitre_lexeme() {
	typedef enum {E_INIT, E_ENTIER, E_FIN} Etat_Automate ;
	Etat_Automate etat=E_INIT;

	//   bool virgule_detectée = false;
	resultat_lecture res = OK;
	 
	 // on commence par lire et ignorer les separateurs et commentaires
    while (est_separateur(caractere_courant()) || est_commentaire(caractere_courant())) {
		if (est_commentaire(caractere_courant())) {
			while (caractere_courant() != '\n') {
				avancer_car();
			}
    } 
		else {
			avancer_car();
		}
}

     lexeme_en_cours.chaine[0] = '\0' ;

     // on utilise ensuite un automate pour reconnaitre et construire le prochain lexeme

    while (etat != E_FIN && res == OK) {

	    switch (etat) {

		  case E_INIT: // etat initial

			switch(nature_caractere(caractere_courant())) {

				case C_FIN_SEQUENCE: 
             		lexeme_en_cours.nature = FIN_SEQUENCE;
					res = OK;
                	etat = E_FIN;
					break ;
				
				case POINT_VIRG:
					lexeme_en_cours.ligne = numero_ligne();
					lexeme_en_cours.colonne = numero_colonne();
					ajouter_caractere(lexeme_en_cours.chaine, caractere_courant());
					lexeme_en_cours.nature = SEPINST;
					etat = E_FIN;
					avancer_car();
					break ;
				
				case LETTRE: //---------------------------------------
					
                	lexeme_en_cours.ligne = numero_ligne();
                	lexeme_en_cours.colonne = numero_colonne();
					
					ajouter_caractere (lexeme_en_cours.chaine, caractere_courant()) ;
					lexeme_en_cours.nature = get_nature_chaine(lexeme_en_cours.chaine);
					
					if(lexeme_en_cours.nature != CHAINE)
					{
						avancer_car();
						lexeme_en_cours.ligne = numero_ligne();
						lexeme_en_cours.colonne = numero_colonne();

						if(est_separateur(caractere_courant()))
						{
							// si la chaine est finie, c'est un mot reservé et il est accepter.
							etat = E_FIN;
							continue;
						} 

						// sinon puis qu'on vient d'avancer,
						// pour éviter de perdre un caractère sur le nom de la variable,
						// on rajoute le caractère courant à la chaine
						// si celui ci est une lettre
						if(nature_caractere(caractere_courant()) == LETTRE) { ajouter_caractere (lexeme_en_cours.chaine, caractere_courant()); }
						else { etat = E_FIN; continue; }
					}

					avancer_car() ;
					if(est_separateur(caractere_courant()) || nature_caractere(caractere_courant()) != LETTRE) etat = E_FIN;
					// au cas où on est est à la fin de la chaine et elle n'est pas un mot réserver telque plus ou fois ....
					// ou le caractère suivant est n'est pas une lettre

					break ; 

				case CHIFFRE: 
					lexeme_en_cours.nature = ENTIER;
                	lexeme_en_cours.ligne = numero_ligne();
                	lexeme_en_cours.colonne = numero_colonne();
		     		ajouter_caractere (lexeme_en_cours.chaine, caractere_courant()) ;
					lexeme_en_cours.valeur = caractere_courant() - '0';
               		etat = E_ENTIER;
		   			avancer_car() ;
					break ;

				case SYMBOLE: 
					// virgule_detectée = false; // faux une fois un symbole lu
		       		lexeme_en_cours.ligne = numero_ligne();
               		lexeme_en_cours.colonne = numero_colonne();
		       		ajouter_caractere (lexeme_en_cours.chaine, caractere_courant()) ;
               		
					switch (caractere_courant()) {
					 case '+':	lexeme_en_cours.nature = PLUS;		break;
               		 case '-':	lexeme_en_cours.nature = MOINS;		break;
               		 case '*':	lexeme_en_cours.nature = MUL;		break;
                     case '/':	lexeme_en_cours.nature = DIV;		break;
					 case '%':	lexeme_en_cours.nature = MODULO;	break;
					 case '=':	lexeme_en_cours.nature = EQ;		break;
					 case '&':	lexeme_en_cours.nature = ET;		break;
					 case '#':  lexeme_en_cours.nature = DIESE;		break;
					 case '|':	lexeme_en_cours.nature = OU;		break;

					 case '!':
						lexeme_en_cours.nature = NON;
						avancer_car();
						if(caractere_courant() == '='){
							lexeme_en_cours.ligne = numero_ligne();
							lexeme_en_cours.colonne = numero_colonne();
							lexeme_en_cours.nature = NEQ;
							ajouter_caractere (lexeme_en_cours.chaine, caractere_courant());
						}  else { etat = E_FIN; continue; }
			   			break;

					 case '>':
               			lexeme_en_cours.nature = SUP;
						avancer_car();
						if(caractere_courant() == '='){
							lexeme_en_cours.ligne = numero_ligne();
							lexeme_en_cours.colonne = numero_colonne();
							lexeme_en_cours.nature = SEQ;
							ajouter_caractere (lexeme_en_cours.chaine, caractere_courant());
						} else { etat = E_FIN; continue; }
			   			break;

					 case '<':
						lexeme_en_cours.nature = INF;
						avancer_car();
						if(caractere_courant() == '-'){
							lexeme_en_cours.ligne = numero_ligne();
							lexeme_en_cours.colonne = numero_colonne();
							lexeme_en_cours.nature = AFF;
							ajouter_caractere (lexeme_en_cours.chaine, caractere_courant()) ;
						} else if(caractere_courant() == '='){
							lexeme_en_cours.ligne = numero_ligne();
							lexeme_en_cours.colonne = numero_colonne();
							lexeme_en_cours.nature = IEQ;
							ajouter_caractere (lexeme_en_cours.chaine, caractere_courant());
						}  else { etat = E_FIN; continue; }
			   			break;

		       		  default:
						if(debug)printf("Erreur_Lexicale") ;
				 		res = ERREUR_LEXICALE;
						break;
				 	} ;

					etat = E_FIN;
		   			avancer_car() ;
					break ;

				case PAR:
					lexeme_en_cours.ligne = numero_ligne();
               		lexeme_en_cours.colonne = numero_colonne();
		       		ajouter_caractere (lexeme_en_cours.chaine, caractere_courant()) ;
               		
					switch (caractere_courant())
					{
						case '(':
							lexeme_en_cours.nature = PARO;
							break;
							
						case ')':
							lexeme_en_cours.nature = PARF;
							break;
					}
					etat = E_FIN;
		   			avancer_car() ;
					break;

				case ACC:
					lexeme_en_cours.ligne = numero_ligne();
               		lexeme_en_cours.colonne = numero_colonne();
		       		ajouter_caractere (lexeme_en_cours.chaine, caractere_courant()) ;
               		
					switch (caractere_courant())
					{
					case '{':
						lexeme_en_cours.nature = ACCO;
						break;
						
					case '}':
						lexeme_en_cours.nature = ACCF;
						break;
					}
					etat = E_FIN;
		   			avancer_car() ;
					break;

				default:
		           printf("Erreur_Lexicale\n") ;
					res = ERREUR_LEXICALE;
		           break;
		} ;
		break ;
      
		case E_ENTIER:  // reconnaissance d'un entier
			switch(nature_caractere(caractere_courant())) {

				case CHIFFRE:
		  			ajouter_caractere (lexeme_en_cours.chaine, caractere_courant()) ;
					lexeme_en_cours.valeur = convertir_chaine_en_nombre(lexeme_en_cours.chaine);
                  	avancer_car ();
					if (nature_caractere(caractere_courant()) != CHIFFRE && nature_caractere(caractere_courant()) != VIRGULE)
					{
						etat = E_FIN;
					}
					
					break;
				
				case VIRGULE: //---------------------------------------
					lexeme_en_cours.ligne = numero_ligne();
                	lexeme_en_cours.colonne = numero_colonne();
		     		
					ajouter_caractere(lexeme_en_cours.chaine, caractere_courant());
					
                  	avancer_car ();
					if (nature_caractere(caractere_courant()) != CHIFFRE) { etat = E_FIN; }
					break ;

				default:
                  	etat = E_FIN;
          	} ;

	    case E_FIN:  // etat final
		break ;
	    
	  } ; // fin du switch(etat)
	} ; // fin du while (etat != fin)

	return res;
   }

   /* --------------------------------------------------------------------- */

   // cette fonction ajoute le caractere c a la fin de la chaine s 
   // (la chaine s est donc modifiee)
 
   void ajouter_caractere (char *s, char c) {
	int l ;
	
	l = strlen(s) ;
	s[l] = c ;
	s[l+1] = '\0' ;
   } ;

   /* --------------------------------------------------------------------- */

   Nature_Caractere nature_caractere (char c) {
	   if (fin_de_sequence_car(c)) 	return C_FIN_SEQUENCE;
	   if (est_chiffre(c)) 		   	return CHIFFRE;
	   if (est_symbole(c))			return SYMBOLE;
	   if (est_virgule(c)) 			return VIRGULE;
	   if(est_lettre(c)) 			return LETTRE;
	   if(est_par(c)) 				return PAR;
	   if(est_acc(c)) 				return ACC;
	   if(est_separateur_inst(c)) 	return POINT_VIRG;
	   								return ERREUR_CAR ;
   }
   /* --------------------------------------------------------------------- */

   // vaut vrai ssi c designe un caractere separateur
   int est_separateur(char c) { 
      return c == ' ' || c == '\t' || c == '\n' || c == '\r' ;
   }

	
   int est_separateur_inst(char c ) { return c == ';';}
   int est_commentaire(char c){ return c=='#';}
   /* --------------------------------------------------------------------- */

   // vaut vrai ssi c designe un caractere chiffre
   int est_chiffre(char c) {
      return c >= '0' && c <= '9' ;
   }

   /* --------------------------------------------------------------------- */

   // vaut vrai ssi c designe un caractere correspondant a un symbole arithmetique
   int est_symbole(char c)  {
      switch (c) {
        case '+':  case '-':  case '*': case '/': case '%':
		case '<':  case '>':  case '=': 
		case '!':  case '&':  case '|':
            return 1;

        default:
            return 0;
      } 
   }

   /* --------------------------------------------------------------------- */

   // vaut vrai ssi c designe un .
   int est_virgule(char c ){ return c == '.';}

   int est_lettre(char c)
   {
		return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z');
   }


   int est_par(char c){ return (c == '(') || (c == ')');}

   int est_acc(char c){ return (c == '{') || (c == '}');}

   /* --------------------------------------------------------------------- */

   // renvoie la chaine de caracteres correspondant a la nature du lexeme
   char *Nature_vers_Chaine (Nature_Lexeme nature) {
		switch (nature) {
			case ENTIER: 	return "ENTIER" ;
			case PLUS: 	 	return "PLUS" ;
			case MOINS:  	return "MOINS" ;            
			case MUL:	 	return "MUL" ;              
			case DIV:	 	return "DIV" ;              
			case VIRG:   	return "VIRG" ;
			case SEPINST:  	return "SEPINST" ;
			case CHAINE: 	return "CHAINE";
			case LIRE: 		return "LIRE";
			case ECRIRE:	return "ECRIRE";
			case SI:		return "SI";
			case SINON:		return "SINON";
			case ALORS:		return "ALORS";
			case TQ:		return "TQ";
			case ET:		return "ET";
			case OU:		return "OU";
			case NON:		return "NON";
			case PARF: 		return "PARF";
			case PARO: 		return "PARO";
			case ACCF: 		return "ACCF";
			case ACCO: 		return "ACCO";
			case MODULO:	return "MODULO";
			case AFF: 		return "AFF";
			case INF: 		return "INF";
			case SUP: 		return "SUP";
			case IEQ: 		return "IEQ";
			case SEQ: 		return "SEQ";
			case EQ:  		return "EQ";
			case NEQ:  		return "NEQ";
			case DIESE:		return "#";
			case FIN_SEQUENCE: return "FIN_SEQUENCE" ;     
			default:		return "ERREUR" ;            
		} ;
   } 

   /* --------------------------------------------------------------------- */

   // affiche a l'ecran le lexeme l
   void afficher(Lexeme l) {

      switch (l.nature) {
         case FIN_SEQUENCE: 
            break;
         default: 
            printf("(ligne %d, ", l.ligne);
            printf("colonne %d) : ",l.colonne);
	    printf("[") ;
            printf("nature = %s", Nature_vers_Chaine(l.nature)) ;
            printf(", chaine = %s, ", l.chaine) ;
            switch(l.nature) {
                 case ENTIER:
                      printf("valeur = %f", l.valeur);
                 default:
                      break;
            } ;
	    printf("]") ;
	} ;
      printf("\n");
   }

   /* --------------------------------------------------------------------- */

double convertir_chaine_en_nombre(const char *chaine) { 
	double valeur = 0.0; 
	double facteur = 1.0; 
	bool apres_point = false; 
	for (int i = 0; chaine[i] != '\0'; i++) { 
		char caractere = chaine[i]; 
		if (caractere == '.') { apres_point = true; } 
		else if (caractere >= '0' && caractere <= '9') { 
			if (apres_point) { 
				facteur /= 10.0; 
				valeur += (caractere - '0') * facteur; 
			} else { 
				valeur = valeur * 10 + (caractere - '0'); 
			}
		} 
	} 
	return valeur;
}
//renvoie Nature_Lexeme correspondant a chaine
Nature_Lexeme get_nature_chaine(char *str){
	if(strcmp(str, "plus") == 0) 		return PLUS;
	if(strcmp(str, "moins") == 0) 		return MOINS;
	if(strcmp(str, "fois") == 0) 		return MUL;
	if(strcmp(str, "diviser") == 0) 	return DIV;
	if(strcmp(str, "modulo") == 0) 		return MODULO;
	if(strcmp(str, "Ecrire") == 0) 		return ECRIRE;
	if(strcmp(str, "Lire") == 0) 		return LIRE;
	if(strcmp(str, "Si") == 0) 			return SI;
	if(strcmp(str, "Sinon") == 0) 		return SINON;
	if(strcmp(str, "Alors") == 0) 		return ALORS;
	if(strcmp(str, "Tq") == 0) 			return TQ;
	if(strcmp(str, "Et") == 0) 			return ET;
	if(strcmp(str, "Ou") == 0) 			return OU;
	if(strcmp(str, "Non") == 0)	 		return NON;
	if(strcmp(str, "Different") == 0)	return NEQ;
	if(strcmp(str, "#") == 0)	return DIESE;

	return CHAINE;
}

void affiche_operateur(Nature_Lexeme op)
{
	switch (op)
	{
	case PLUS: printf("Operateur: +\n"); break;
	case MOINS: printf("Operateur: -\n"); break;
	case MUL: printf("Operateur: *\n"); break;
	case DIV: printf("Operateur: /\n"); break;
	case MODULO: printf("Operateur: %% \n"); break;
	
	default: printf("Operateur Inconnu!\n"); break;
	}
}