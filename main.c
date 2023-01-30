#include <stdio.h>
#include <string.h>
#define DEFAULT_INPUT_SIZE 100
#define ROW_SIZE 2048

typedef struct {
	char ID_station[DEFAULT_INPUT_SIZE]; 
	char moister[DEFAULT_INPUT_SIZE];
} dict;

int count_nb_lines_in_file(char* input_file){
	FILE * ifp;
	ifp = fopen(input_file, "r");

	if (ifp == NULL){
		printf("Could not read file\n fopen error");
		return 2; // TODO: set constants
	}

	char buffer[ROW_SIZE];
	int nb_lines =0; 
	while(fgets(buffer, ROW_SIZE, ifp)){
		nb_lines++;
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

		else if (strcmp(argv[i], "--tab") == 0){
			strcpy(sort_algorithm, "tab");
		}
		else if (strcmp(argv[i], "--abr") == 0){
			strcpy(sort_algorithm, "abr");
		}
		else if (strcmp(argv[i], "--avl") == 0){
			strcpy(sort_algorithm, "avl");
		}
		else {
			*valid_option = -10;
		}
	}
}

int main(int argc, char *argv[]) {

	int valid_option = 0;
	int reversed_order = 0;
	char sort_algorithm[DEFAULT_INPUT_SIZE] = "avl";
	char input_file[DEFAULT_INPUT_SIZE];
	char output_file[DEFAULT_INPUT_SIZE];
	manage_input_options(
						&argc,
						argv,
						sort_algorithm,
						input_file, 
						output_file,  
						&reversed_order,
						&valid_option);
	if (valid_option!=2){
		printf("Bad option combination\n");
		return 1;
	}
	// necessary variables
	printf("Good option combination\n");
	printf("input_file: %s\n", input_file);
	printf("output_file: %s\n", output_file);
	printf("sort_algorithm: %s\n", sort_algorithm);


	int nb_line = count_nb_lines_in_file(input_file);
	printf("nb lines: %d\n", nb_line);
	FILE * ifp;
	ifp = fopen(input_file, "r");
	dict values[100];

	if (ifp == NULL){
		printf("Could not read file\n fopen error");
		return 2; // TODO: set constants
	}

	char buffer[ROW_SIZE];
	int row_count = 0;
	int field_count = 0;
	int i=0;
	printf("Reading file...\n");
	while(fgets(buffer, ROW_SIZE, ifp)){
		field_count = 0; 
		row_count++;
		if (row_count == 1)// skip header
			continue;		
		
		char *field = strtok(buffer, ";");
		while(field){
			if (field_count == 0){
				strcpy(values[i].ID_station, field);
			}
			if (field_count == 1){
				strcpy(values[i].moister, field);
			}
			field = strtok(NULL, ";");
			field_count++;
		}
		i++;
	}
    fclose(ifp);
	printf("File closed...");
	
	return 0;
}
