#!/bin/bash
if [[ ! -x "./main" ]]; then
    echo " fichier main introuvable"
    exit 1
fi


auto=1 # 1 = mode automatique, 0 = mode interactif

if [ "$auto" == "1" ]; then
    for fichier in $(ls f_test/Tests/test*.txt 2>/dev/null | sort -V); do
        if [[ -f $fichier ]]; then
            echo "Test avec le fichier : $fichier"
            echo "contenu du fichier:"
            cat $fichier
            ./main "$fichier"
        fi
        read -p "➡️   Pour continuer tapez Entree: " reponse
        if [ "$reponse" != "" ]; then
            echo "🛑 Test interrompu."
            exit 0
        fi
        echo "------------------------------------------------------------------------"
    done
else
    for fichier in $(ls f_test/Tests/test*.txt 2>/dev/null | sort -V); do
        if [[ -f $fichier ]]; then
            echo "Test avec le fichier : $fichier"
            echo "contenu du fichier:"
            cat $fichier
            ./main "$fichier"
        fi
        
        read -p "➡️   Pour reprendre avec le fichier courant, tapez 'r': " reponse1
        while [ "$reponse1" == "r" ]; do
            ./main "$fichier"
            read -p "➡️   Pour reprendre avec le fichier courant, tapez 'r': " reponse1
        done

        read -p "➡️   Pour continuer avec le fichier suivant, tapez 'y': " reponse
        if [ "$reponse" != "y" ]; then
            echo "🛑 Test interrompu."
            exit 0
        fi
        
        echo "------------------------------------------------------------------------"

    done
fi
echo "✅ Tous les tests on ete effectues"
