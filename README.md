Prérequis:
	- installer gnuplot (sudo apt install gnuplot) <br />
	- installer make (sudo apt install gnuplot)<br />
	- installer gcc (sudo apt install gnuplot)<br />
	
Pour lancer le programme: <br />
Il suffit de compiler et de rendre le script executable en utilisant le Makefile en lançant la commande un make<br />
Une application nommée application va aparaitre dans le répertoire courant.<br />
Puis il faudra lancer un des scripts ci-dessous suivants pour tester le pogramme.<br />
Cela générera un répertoire dans lequel se trouvera un sous-repertoire nommé du nom de la variable souhaité.<br />
3 fichiers csv seont générés: <br />
    - Variable_data.csv contenant les données d'entrée le programme C<br />
    - Variable_output_data.csv contenant les données en sortie du programme C<br />
    - Variable_polt_data.csv contenant les données de plot pour gnuplot<br />
1 fichier .png contenant le graphique<br />
<br />
Noter que si une variable est nulle (non présente), la ligne est supprimée. Ceci justifie que potentiellement qu'un fichier aie plus de lignes pour une variable que pour une autre.<br />
<br />
<br />
Note: Si plusieurs modes sont entrés, seul le dernier sera pris en compte<br />
<br />
Cas classiques:<br />
./script_meteo.sh -m -f sample.csv --tab<br />
./script_meteo.sh -m -f sample.csv --abr<br />
./script_meteo.sh -m -f sample.csv --avl -r<br />
./script_meteo.sh -m -f sample.csv --avl -r -d -F<br />
./script_meteo.sh -m -f sample.csv --avl -r -d 2010-01-02 2010-06-05 -F<br />
<br />
<br />
<br />
./script_meteo.sh -m -f sample.csv --tab<br />
./script_meteo.sh -h -f sample.csv --abr<br />
./script_meteo.sh -w -f sample.csv --avl -r<br />
<br />
./script_meteo.sh -t -f sample.csv<br />
./script_meteo.sh -t -1 -f sample.csv<br />
./script_meteo.sh -p2 -f sample.csv<br />
./script_meteo.sh -p -3 -f sample.csv<br />

Cas erreurs:<br />
./script_meteo.sh -m -1 -f sample.csv<br />
./script_meteo.sh -m -f sample.csv -F -G<br />
./script_meteo.sh -h -f sample.csv -d 201-01-02 2010-06-05<br />
./script_meteo.sh -m<br />
./script_meteo.sh -e<br />
