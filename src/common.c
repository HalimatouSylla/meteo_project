#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "common.h"

int count_nb_lines_in_file(char* input_file);
void manage_input_options(
						int* argc,
						char** argv,
						char* sort_algorithm,
						char* input_file, 
						char* output_file,  
						int* reversed_order,
						int* valid_option);
int get_mode_from_file_name(char* input_file);


int get_mode_from_file_name(char* input_file){
	if (strstr(input_file, "1") != NULL){
		return 1;

	}
	else if (strstr(input_file, "2") != NULL){
		return 2;

	}
	else if (strstr(input_file, "3") != NULL){
		return 3;
	}
	else
		return 0;
}


int count_nb_lines_in_file(char* input_file){
	FILE * ifp;
	ifp = fopen(input_file, "r");

	if (ifp == NULL){
		printf("Could not read file\n fopen error");
		return 2; // TODO: set constants
	}
	char c;
	int nb_lines =0; 
	for(c = getc(ifp); c!=EOF; c = getc(ifp)){
		if (c == '\n'){
			nb_lines++;
		}
		
	}
	return nb_lines;
}

void manage_input_options(
						int* argc,
						char** argv,
						char* sort_algorithm,
						char* input_file, 
						char* output_file,  
						int* reversed_order,
						int* valid_option){
for (int i = 1; i < *argc; i++) {
		
		if (strcmp(argv[i], "-f") == 0){
			if (i+1 < *argc){
				*valid_option += 1;
				strcpy(input_file, argv[i+1]);
				i++;
			}
		}

		else if (strcmp(argv[i], "-o") == 0){
			
			if (i+1 < *argc){
				*valid_option += 1;
				strcpy(output_file, argv[i+1]);
				i++;
			}
		}

		else if (strcmp(argv[i], "-r") == 0){
			*reversed_order = 1;
		}

		else if (strcmp(argv[i], "tab") == 0){
			strcpy(sort_algorithm, "tab");
		}
		else if (strcmp(argv[i], "abr") == 0){
			strcpy(sort_algorithm, "abr");
		}
		else if (strcmp(argv[i], "avl") == 0){
			strcpy(sort_algorithm, "avl");
		}
		else {
			*valid_option = -10;
		}
	}
}