all: compile_project clear

compile_project:src
	gcc -o application src/*.c

executable_script: script_meteo.sh
	chmod u+x script_meteo.sh
clear:
	rm -r data_generation
