all: compile_project clear

compile_project:src
	gcc -o application src/*.c

clear:
	rm -r data_generation