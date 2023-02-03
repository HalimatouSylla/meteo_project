#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "common.h"
#include "temperature.h"

///////////////////////////////////////////////////////////
// Data structures
///////////////////////////////////////////////////////////
typedef struct temperature_record{
	char ID_station[DEFAULT_INPUT_SIZE]; 
  char date[DEFAULT_INPUT_SIZE];
  char temperature[DEFAULT_INPUT_SIZE];
	char temperature_min[DEFAULT_INPUT_SIZE];
  char temperature_max[DEFAULT_INPUT_SIZE];
	char longitude[DEFAULT_INPUT_SIZE] ; 
	char latitude[DEFAULT_INPUT_SIZE] ; 
}temperature_record;

struct node {
  struct temperature_record data; //node will store some data
  struct node *right_child; // right child
  struct node *left_child; // left child
  int height;
};
///////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////


int comparator__temperature(temperature_record first_record, temperature_record second_record, int mode){
    // postive if first_record > second_record depending on mode
    int compare;
    if(mode == 1){
      compare = strcmp(first_record.ID_station, second_record.ID_station);
    }
    else if(mode == 2){
      compare = strcmp(first_record.date, second_record.date);
    }
    else if(mode == 3){
      compare = strcmp(first_record.date, second_record.date);
      if (compare == 0){
        compare = strcmp(first_record.ID_station, second_record.ID_station);
      }
    }
    return compare;
}

void sortTemperatureArray(temperature_record records[], int size, int mode){
	int i,j, compare;
	temperature_record temp;
	for (i=0; i<size-1;i++){
		for (j=i+1; j<size;j++){
      compare = comparator__temperature(records[i], records[j], mode);
			if(compare > 0){
				temp = records[i];
				records[i] = records[j];
				records[j] = temp;
			}
		}
	}
}


///////////////////////////////////////////////////////////
// ABR & AVL implementation
///////////////////////////////////////////////////////////
//function to create a node

struct node* new_node__temperature(temperature_record record) {
  struct node *temp;
  temp = malloc(sizeof(struct node));
  temp -> data = record;
  temp -> left_child = NULL;
  temp -> right_child = NULL;

  return temp;
}

// insertion
struct node* insert__temperature(struct node * root, temperature_record record, int mode) {
  //searching for the place to insert
  if (root == NULL)
    return new_node__temperature(record);
  
  int compare = comparator__temperature(record, root -> data, mode);
  if (compare > 0) // x is greater. Should be inserted to the right_child
    root -> right_child = insert__temperature(root -> right_child, record, mode);
  else // x is smaller and should be inserted to left_child
    root -> left_child = insert__temperature(root -> left_child, record, mode);
  return root;
}

// Calculate height
int height__temperature(struct node *N) {
  if (N == NULL)
    return 0;
  return N->height;
}

int max__temperature(int a, int b) {
  return (a > b) ? a : b;
}

// Create a AVL node 
struct node *newNodeAVL__temperature(temperature_record record) {
  struct node *temp;
  temp = malloc(sizeof(struct node));
  temp -> data = record;
  temp -> left_child = NULL;
  temp -> right_child = NULL;
  temp -> height = 1;
  return temp;
}

// Right rotate AVL
struct node *rightRotate__temperature(struct node *y) {
  struct node *x = y->left_child;
  struct node *T2 = x->right_child;

  x->right_child = y;
  y->left_child = T2;

  y->height = max__temperature(height__temperature(y->left_child), height__temperature(y->right_child)) + 1;
  x->height = max__temperature(height__temperature(x->left_child), height__temperature(x->right_child)) + 1;

  return x;
}

// Left rotate AVL
struct node *leftRotate__temperature(struct node *x) {
  struct node *y = x->right_child;
  struct node *T2 = y->left_child;

  y->left_child = x;
  x->right_child = T2;

  x->height = max__temperature(height__temperature(x->left_child), height__temperature(x->right_child)) + 1;
  y->height = max__temperature(height__temperature(y->left_child), height__temperature(y->right_child)) + 1;

  return y;
}

// Get the balance factor AVL
int getBalance__temperature(struct node *N) {
  if (N == NULL)
    return 0;
  return height__temperature(N->left_child) - height__temperature(N->right_child);
}

// Insert node AVL
struct node *insertNodeAVL__temperature(struct node *node, temperature_record record, int mode) {
  // Find the correct position to insertNode the node and insertNode it
  if (node == NULL)
    return newNodeAVL__temperature(record);
  // printf("Temperature %d \n", node -> data.temperature); // printing data at root
  int compare = comparator__temperature(record, node -> data, mode);
  if (compare < 0)
    node->left_child = insertNodeAVL__temperature(node->left_child, record, mode);
  else if (compare > 0)
    node->right_child = insertNodeAVL__temperature(node->right_child, record, mode);
  else
    return node;
  // printf("Temperature %d \n", node -> data.temperature); // printing data at root

  // Update the balance factor of each node and
  // Balance the tree
  node->height = 1 + max__temperature(height__temperature(node->left_child),
               height__temperature(node->right_child));

  int balance = getBalance__temperature(node);

  int compare_left = 0;
  int compare_right = 0;

  if (node->left_child != NULL)
    compare_left = comparator__temperature(record, node->left_child->data, mode);
  if (node->right_child != NULL)
    compare_right = comparator__temperature(record, node->right_child->data, mode);

  if (balance > 1 && compare_left <= 0)
    return rightRotate__temperature(node);

  if (balance < -1 && compare_right >= 0)
    return leftRotate__temperature(node);

  if (balance > 1 && compare_left >= 0) {
    node->left_child = leftRotate__temperature(node->left_child);
    return rightRotate__temperature(node);
  }

  if (balance < -1 && compare_right <= 0) {
    node->right_child = rightRotate__temperature(node->right_child);
    return leftRotate__temperature(node);
  }

  return node;
}

// Inorder Traversal
void inorder__temperature(struct node *root) {
  if (root != NULL) // checking if the root is not null
  {
    inorder__temperature(root -> left_child); // traversing left child
    printf(" %s \n", root -> data.date); // printing data at root
    printf(" %s \n", root -> data.ID_station); // printing data at root
    inorder__temperature(root -> right_child); // traversing right child
  }
}

///////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////



///////////////////////////////////////////////////////////
// Write in output file
///////////////////////////////////////////////////////////
void write_output_tab__temperature(char * output_file, char * header, temperature_record records[], int row_count, int reversed_order, int mode){
  sortTemperatureArray(records, row_count, mode);
  FILE * ofp;
	ofp = fopen(output_file, "w");
	printf("Output File opened...\n");
	fprintf(ofp, "%s", header);
  if (reversed_order == 0){
    for (int i = 1; i < row_count; i++) {
        fprintf(ofp, "%s;%s;%s;%s;%s;%s;%s", records[i].ID_station, records[i].date, records[i].temperature, records[i].temperature_min, records[i].temperature_max, records[i].longitude, records[i].latitude);
    }
  }
  else{
    for (int i = row_count-1; i > 0; i--) {
        fprintf(ofp, "%s;%s;%s;%s;%s;%s;%s", records[i].ID_station, records[i].date, records[i].temperature, records[i].temperature_min, records[i].temperature_max, records[i].longitude, records[i].latitude);
    }
  }
	fclose(ofp);
	printf("Output File closed...\n");
  
}

void in_order_write__temperature(FILE * ofp, struct node *root) {
  if (root != NULL) // checking if the root is not null
  {
    in_order_write__temperature(ofp, root -> left_child); // traversing left child
    if (strcmp(root -> data.ID_station, "init") != 0){
      fprintf(ofp, "%s;%s;%s;%s;%s;%s;%s", root -> data.ID_station, root -> data.date, root -> data.temperature, root -> data.temperature_min, root -> data.temperature_max, root -> data.longitude, root -> data.latitude);
    }
    in_order_write__temperature(ofp, root -> right_child); // traversing right child
  }
}

void reversed_order_write__temperature(FILE * ofp, struct node *root) {
  if (root != NULL) // checking if the root is not null
  {
    reversed_order_write__temperature(ofp, root -> right_child); // traversing right child
    if (strcmp(root -> data.ID_station, "init") != 0){
      fprintf(ofp, "%s;%s;%s;%s;%s;%s;%s", root -> data.ID_station, root -> data.date, root -> data.temperature, root -> data.temperature_min, root -> data.temperature_max, root -> data.longitude, root -> data.latitude);
    }
    reversed_order_write__temperature(ofp, root -> left_child); // traversing left child
  }
}

void write_output_tree__temperature(char * output_file, char * header, struct node * node_temperature, int reversed_order){
  FILE * ofp;
	ofp = fopen(output_file, "w");
	printf("Output File opened...\n");
	fprintf(ofp, "%s", header);
  if (reversed_order==0){
      in_order_write__temperature(ofp, node_temperature);
  }
  else{
      reversed_order_write__temperature(ofp, node_temperature);
  }

  fclose(ofp);
	printf("Output File closed...\n");

}

///////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////


int main_temperature(char* input_file, char* output_file, char* sort_algorithm, int reversed_order, int mode){
    printf("Temperature main entred \n");

	int nb_lines = count_nb_lines_in_file(input_file);
	printf("nb lines in input files: %d\n", nb_lines);
  temperature_record init_record;
  strcpy(init_record.ID_station, "init");
  strcpy(init_record.temperature, "init");
  strcpy(init_record.temperature_min, "init");
  strcpy(init_record.temperature_max, "init");
  strcpy(init_record.longitude, "init");
  strcpy(init_record.latitude, "init");

  temperature_record* records;
  struct node* node_temperature = new_node__temperature(init_record);
  struct node* avl_node_temperature = newNodeAVL__temperature(init_record);

  if (strcmp(sort_algorithm, "tab") == 0){
      printf("Sort Method structure tab \n");
      records = malloc(nb_lines*sizeof(temperature_record));
  }
  else if (strcmp(sort_algorithm, "abr") == 0){
    printf("Sort Method structure abr \n");
  }
  else if (strcmp(sort_algorithm, "avl") == 0){
    printf("Sort Method structure avl \n");
  } 

	printf("Input File opened...\n");
	FILE * ifp;
	ifp = fopen(input_file, "r");
	if (ifp == NULL){
		printf("Could not read file\n fopen error");
		return 2; // TODO: set constants
	}

	
	int row_count = 0;
	int nb_parsing_errors = 0;
	char _header_buffer[ROW_SIZE];
	char header[] = "ID_station;Date;Temperature;Temperature_min;Temperature_max;Longitude;Latitude";
	char coords_buffer[DEFAULT_INPUT_SIZE];
	char temperature_buffer[DEFAULT_INPUT_SIZE];

    while (row_count<nb_lines) {
		if (row_count==0){
			fscanf(ifp, "%s", _header_buffer);
		}
		else{
            char ID_station_buffer[DEFAULT_INPUT_SIZE];
            char date_buffer[DEFAULT_INPUT_SIZE];
	          char temperature_buffer[DEFAULT_INPUT_SIZE];
            char temperature_min_buffer[DEFAULT_INPUT_SIZE];
            char temperature_max_buffer[DEFAULT_INPUT_SIZE];
            char longitude_buffer[DEFAULT_INPUT_SIZE];
            char latitude_buffer[DEFAULT_INPUT_SIZE];
            int nb_cols = fscanf(ifp, "%[^;];%[^;];%[^;];%[^;];%[^;];%[^;];%s", ID_station_buffer, date_buffer, temperature_buffer, temperature_min_buffer, temperature_max_buffer, longitude_buffer, latitude_buffer);

			if (nb_cols == 7){
          if (strcmp(sort_algorithm, "tab") == 0){
              strcpy(records[row_count].ID_station, ID_station_buffer);
              strcpy(records[row_count].date, date_buffer);
              strcpy(records[row_count].temperature, temperature_buffer);
              strcpy(records[row_count].temperature_min, temperature_min_buffer);
              strcpy(records[row_count].temperature_max, temperature_max_buffer);
              strcpy(records[row_count].longitude, longitude_buffer);
              strcpy(records[row_count].latitude, latitude_buffer);
          }
          else if (strcmp(sort_algorithm, "abr") == 0){
                  printf("Sort Method structure abr \n");
                  temperature_record record;
                  strcpy(record.ID_station, ID_station_buffer);
                  strcpy(record.date, date_buffer);
                  strcpy(record.temperature, temperature_buffer);
                  strcpy(record.temperature_min, temperature_min_buffer);
                  strcpy(record.temperature_max, temperature_max_buffer);
                  strcpy(record.longitude, longitude_buffer);
                  strcpy(record.latitude, latitude_buffer);
                  insert__temperature(node_temperature, record, mode);

              }
          else if (strcmp(sort_algorithm, "avl") == 0){
                  temperature_record record;
                  strcpy(record.ID_station, ID_station_buffer);
                  strcpy(record.date, date_buffer);
                  strcpy(record.temperature, temperature_buffer);
                  strcpy(record.temperature_min, temperature_min_buffer);
                  strcpy(record.temperature_max, temperature_max_buffer);
                  strcpy(record.longitude, longitude_buffer);
                  strcpy(record.latitude, latitude_buffer);
                  avl_node_temperature = insertNodeAVL__temperature(avl_node_temperature, record, mode);


              }
        }
        else{
          nb_parsing_errors++;
        }	
			}
      row_count++;	
		}
		

	printf("nb lines treated %d\n", row_count);
	printf("nb lines not treated (parsing errors) %d\n", nb_parsing_errors);
  fclose(ifp);
	printf("Input File closed...\n");

  if (strcmp(sort_algorithm, "tab") == 0){
    printf("Sorting...\n");
    write_output_tab__temperature(output_file, header, records, row_count, reversed_order, mode);
    free(records);
  }

  if (strcmp(sort_algorithm, "abr") == 0){
    write_output_tree__temperature(output_file, header, node_temperature, reversed_order);
    free(node_temperature);
  }

  if (strcmp(sort_algorithm, "avl") == 0){
    write_output_tree__temperature(output_file, header, avl_node_temperature, reversed_order);
    free(avl_node_temperature);
  }


  return 0;
}
