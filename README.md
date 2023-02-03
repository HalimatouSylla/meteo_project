Pour lancer le programme: 
Il suffit de compiler en utilisant le Makefile en lançant la commande un make
Une application nommée application va aparaitre dans le répertoire courant.
Puis il faudra lancer un des scripts ci-dessous suivants pour tester le pogramme.
Cela générera un répertoire dans lequel se trouvera un sous-repertoire nommé du nom de la variable souhaité.
3 fichiers csv seont générés: 
    - Variable_data.csv contenant les données d'entrée le programme C
    - Variable_output_data.csv contenant les données en sortie du programme C
    - Variable_polt_data.csv contenant les données de plot pour gnuplot
1 fichier .png contenant le graphique

Noter que si une variable est nulle (non présente), la ligne est supprimée. Ceci justifie que potentiellement qu'un fichier aie plus de lignes pour une variable que pour une autre.


Note: Si plusieurs modes sont entrés, seul le dernier sera pris en compte

Cas classiques:
./script_meteo.sh -m -f sample.csv --tab
./script_meteo.sh -m -f sample.csv --abr
./script_meteo.sh -m -f sample.csv --avl -r
./script_meteo.sh -m -f sample.csv --avl -r -d -F
./script_meteo.sh -m -f sample.csv --avl -r -d 2010-01-02 2010-06-05 -F



./script_meteo.sh -m -f sample.csv --tab
./script_meteo.sh -h -f sample.csv --abr
./script_meteo.sh -w -f sample.csv --avl -r

./script_meteo.sh -t -f sample.csv
./script_meteo.sh -t -1 -f sample.csv
./script_meteo.sh -p2 -f sample.csv
./script_meteo.sh -p -3 -f sample.csv

Cas erreurs:
./script_meteo.sh -m -1 -f sample.csv
./script_meteo.sh -m -f sample.csv -F -G
./script_meteo.sh -h -f sample.csv -d 201-01-02 2010-06-05
./script_meteo.sh -m
./script_meteo.sh -e