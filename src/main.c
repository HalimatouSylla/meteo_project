#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "common.h"
#include "moister.h"
#include "height.h"
#include "wind.h"
#include "temperature.h"
#include "pressure.h"



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
	int mode = get_mode_from_file_name(input_file);
	// necessary variables
	printf("Good option combination\n");
	printf("#####################################\n");
	printf("input_file: %s\n", input_file);
	printf("output_file: %s\n", output_file);
	printf("sort_algorithm: %s\n", sort_algorithm);
	printf("reversed_order: %d\n", reversed_order);
	printf("mode: %d\n", mode);
	printf("#####################################\n");

	int output_code = 0;
	// variable detection is based on file name
	if (strstr(input_file, "moister") != NULL){
		int output_code = main_moister(input_file, output_file, sort_algorithm, reversed_order);
		printf("Moister sorting done\n");
	}
	else if (strstr(input_file, "height") != NULL){
		int output_code = main_height(input_file, output_file, sort_algorithm, reversed_order);
		printf("Height sorting done\n");
	}
	else if (strstr(input_file, "wind") != NULL){
		int output_code = main_wind(input_file, output_file, sort_algorithm, reversed_order);
		printf("Wind sorting done\n");
	}
	else if (strstr(input_file, "temperature") != NULL){
		int output_code = main_temperature(input_file, output_file, sort_algorithm, reversed_order, mode);
		printf("Temperature sorting done\n");
	}	
	else if (strstr(input_file, "pressure") != NULL){
		int output_code = main_pressure(input_file, output_file, sort_algorithm, reversed_order, mode);
		printf("Pressure sorting done\n");
	}	

	return output_code;
}


