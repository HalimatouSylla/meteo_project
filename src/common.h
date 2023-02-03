#ifndef COMMON
#define COMMON
#define ROW_SIZE 2048
#define DEFAULT_INPUT_SIZE 100
///////////////////////////////////////////////////////////
// Functions
///////////////////////////////////////////////////////////
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
///////////////////////////////////////////////////////////
#endif