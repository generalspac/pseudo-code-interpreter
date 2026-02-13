
/* Exemple d'utilisation de l'analyse lexicale : 
    Affichage d'une sequence de lexemes            */

    #include <stdio.h>
    #include <stdlib.h>
    
    #include "analyse_syntaxique.h"
    #include "ast_parcours.h"
    #include "ast_construction.h"
    #include "erreur_lecture.h"
    #include "table_symbole.h"
    

   
    
    int main (int argc, char *argv[]) {
        //int debug=0;
       
       char *fichier = NULL;
       Ast arbre = NULL;
       Symbole_tab *table_des_variables = init_table_vide();
       resultat_lecture res = OK;
       if(argc < 2){
            // fichier = (char*) malloc(sizeof(char));
            printf("Erreur d'usage: le programme a besoin du nom de fichier de test en argument\n");
            printf("Entrez ce nom svp :");
            scanf(" %s", fichier);
            
        } else {
            fichier = argv[1];
        }
                
        printf("\n---------------------------- Analyse du PROGRAMME:----------------------------\n");
        res = analyser_prog(fichier, &arbre);
        
        printf("\n---------------------------- Analyse terminee ----------------------------\n");

        afficher_msg_erreur(res);
        if(res != OK){
            liberer_table(table_des_variables);
            liberer_ast(&arbre);
            //free(fichier);
            return 1;
        }

        printf("\n------------------------Interpretation du PROGRAMME:------------------------\n");
        res = interpretation(arbre, table_des_variables);

        printf("\n------------------------Interpretation terminee ------------------------\n");

        afficher_msg_erreur(res); 

        liberer_table(table_des_variables);
        liberer_ast(&arbre);
     
       return 0 ;
    }
    