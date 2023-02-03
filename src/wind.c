#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "common.h"
#include "wind.h"

///////////////////////////////////////////////////////////
// Data structures
///////////////////////////////////////////////////////////
typedef struct wind_record{
	char ID_station[DEFAULT_INPUT_SIZE]; 
  char wind_direction[DEFAULT_INPUT_SIZE];
	char wind_speed[DEFAULT_INPUT_SIZE];
	char longitude[DEFAULT_INPUT_SIZE] ; 
	char latitude[DEFAULT_INPUT_SIZE] ; 
}wind_record;

struct node {
  struct wind_record data; //node will store some data
  struct node *right_child; // right child
  struct node *left_child; // left child
  int height;
};
///////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////

void sortWindArray(wind_record records[], int size){
	int i,j;
	wind_record temp;
	for (i=0; i<size-1;i++){
		for (j=i+1; j<size;j++){
      int compare = strcmp(records[i].ID_station, records[j].ID_station);
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

struct node* new_node__wind(wind_record record) {
  struct node *temp;
  temp = malloc(sizeof(struct node));
  temp -> data = record;
  temp -> left_child = NULL;
  temp -> right_child = NULL;

  return temp;
}

// insertion
struct node* insert__wind(struct node * root, wind_record record) {
  //searching for the place to insert
  if (root == NULL)
    return new_node__wind(record);
  int compare = strcmp(record.ID_station, root -> data.ID_station);
  if (compare > 0) // x is greater. Should be inserted to the right_child
    root -> right_child = insert__wind(root -> right_child, record);
  else // x is smaller and should be inserted to left_child
    root -> left_child = insert__wind(root -> left_child, record);
  return root;
}

// Calculate height
int height__wind(struct node *N) {
  if (N == NULL)
    return 0;
  return N->height;
}

int max__wind(int a, int b) {
  return (a > b) ? a : b;
}

// Create a AVL node 
struct node *newNodeAVL__wind(wind_record record) {
  struct node *temp;
  temp = malloc(sizeof(struct node));
  temp -> data = record;
  temp -> left_child = NULL;
  temp -> right_child = NULL;
  temp -> height = 1;
  return temp;
}

// Right rotate AVL
struct node *rightRotate__wind(struct node *y) {
  struct node *x = y->left_child;
  struct node *T2 = x->right_child;

  x->right_child = y;
  y->left_child = T2;

  y->height = max__wind(height__wind(y->left_child), height__wind(y->right_child)) + 1;
  x->height = max__wind(height__wind(x->left_child), height__wind(x->right_child)) + 1;

  return x;
}

// Left rotate AVL
struct node *leftRotate__wind(struct node *x) {
  struct node *y = x->right_child;
  struct node *T2 = y->left_child;

  y->left_child = x;
  x->right_child = T2;

  x->height = max__wind(height__wind(x->left_child), height__wind(x->right_child)) + 1;
  y->height = max__wind(height__wind(y->left_child), height__wind(y->right_child)) + 1;

  return y;
}

// Get the balance factor AVL
int getBalance__wind(struct node *N) {
  if (N == NULL)
    return 0;
  return height__wind(N->left_child) - height__wind(N->right_child);
}

// Inorder Traversal
void inorder__wind(struct node *root) {
  if (root != NULL) // checking if the root is not null
  {
    inorder__wind(root -> left_child); // traversing left child
    printf(" %s \n", root -> data.ID_station); // printing data at root
    inorder__wind(root -> right_child); // traversing right child
  }
}
// Insert node AVL
struct node *insertNodeAVL__wind(struct node *node, wind_record record) {
  // Find the correct position to insertNode the node and insertNode it
  if (node == NULL){
    return newNodeAVL__wind(record);
    }
  // printf("Wind %d \n", node -> data.wind); // printing data at root
  int compare = strcmp(record.ID_station, node -> data.ID_station);
  if (compare < 0)
    node->left_child = insertNodeAVL__wind(node->left_child, record);
  else if (compare > 0)
    node->right_child = insertNodeAVL__wind(node->right_child, record);
  else
    return node;
  // printf("Wind %d \n", node -> data.wind); // printing data at root

  // Update the balance factor of each node and
  // Balance the tree
  node->height = 1 + max__wind(height__wind(node->left_child),
               height__wind(node->right_child));
  int balance = getBalance__wind(node);

  int compare_left = 0;
  int compare_right = 0;

  if (node->left_child != NULL)
    compare_left = strcmp(record.ID_station, node->left_child->data.ID_station);
  if (node->right_child != NULL)
    compare_right = strcmp(record.ID_station, node->right_child->data.ID_station);

  if (balance > 1 && compare_left <= 0)
    return rightRotate__wind(node);

  if (balance < -1 && compare_right >= 0)
    return leftRotate__wind(node);

  if (balance > 1 && compare_left > 0) {
    node->left_child = leftRotate__wind(node->left_child);
    return rightRotate__wind(node);
  }

  if (balance < -1 && compare_right < 0) {
    node->right_child = rightRotate__wind(node->right_child);
    return leftRotate__wind(node);
  }

  return node;
}



///////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////



///////////////////////////////////////////////////////////
// Write in output file
///////////////////////////////////////////////////////////
void write_output_tab__wind(char * output_file, char * header, wind_record records[], int row_count, int reversed_order){
  sortWindArray(records, row_count);
  FILE * ofp;
	ofp = fopen(output_file, "w");
	printf("Output File opened...\n");
	fprintf(ofp, "%s", header);
  if (reversed_order == 0){
    for (int i = 1; i < row_count; i++) {
        fprintf(ofp, "%s;%s;%s;%s;%s", records[i].ID_station, records[i].wind_direction, records[i].wind_speed, records[i].longitude, records[i].latitude);
    }
  }
  else{
    for (int i = row_count-1; i > 0; i--) {
        fprintf(ofp, "%s;%s;%s;%s;%s", records[i].ID_station, records[i].wind_direction, records[i].wind_speed, records[i].longitude, records[i].latitude);
    }
  }
	fclose(ofp);
	printf("Output File closed...\n");
  
}

void in_order_write__wind(FILE * ofp, struct node *root) {
  if (root != NULL) // checking if the root is not null
  {
    in_order_write__wind(ofp, root -> left_child); // traversing left child
    if (strcmp(root -> data.ID_station, "init") != 0){
    fprintf(ofp, "%s;%s;%s;%s;%s", root -> data.ID_station, root -> data.wind_direction, root -> data.wind_speed, root -> data.longitude, root -> data.latitude);
    }
    in_order_write__wind(ofp, root -> right_child); // traversing right child
  }
}

void reversed_order_write__wind(FILE * ofp, struct node *root) {
  if (root != NULL) // checking if the root is not null
  {
    reversed_order_write__wind(ofp, root -> right_child); // traversing right child
    if (strcmp(root -> data.ID_station, "init") != 0){
      fprintf(ofp, "%s;%s;%s;%s;%s", root -> data.ID_station, root -> data.wind_direction, root -> data.wind_speed, root -> data.longitude, root -> data.latitude);
    }
    reversed_order_write__wind(ofp, root -> left_child); // traversing left child
  }
}

void write_output_tree__wind(char * output_file, char * header, struct node * node_wind, int reversed_order){
  FILE * ofp;
	ofp = fopen(output_file, "w");
	printf("Output File opened...\n");
	fprintf(ofp, "%s", header);
  if (reversed_order==0){
      in_order_write__wind(ofp, node_wind);
  }
  else{
      reversed_order_write__wind(ofp, node_wind);
  }

  fclose(ofp);
	printf("Output File closed...\n");

}

///////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////


int main_wind(char* input_file, char* output_file, char* sort_algorithm, int reversed_order){
    printf("Wind main entred \n");

	int nb_lines = count_nb_lines_in_file(input_file);
	printf("nb lines in input files: %d\n", nb_lines);
  wind_record init_record;
  strcpy(init_record.ID_station, "init");
  strcpy(init_record.wind_direction, "init");
  strcpy(init_record.wind_speed, "init");
  strcpy(init_record.longitude, "init");
  strcpy(init_record.latitude, "init");

  wind_record* records;
  struct node* node_wind = new_node__wind(init_record);
  struct node* avl_node_wind = newNodeAVL__wind(init_record);

  if (strcmp(sort_algorithm, "tab") == 0){
      printf("Sort Method structure tab \n");
      records = malloc(nb_lines*sizeof(wind_record));
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
	char header[] = "ID_station;Direction_vent;Vitesse_vent;Longitude;Latitude";
	char coords_buffer[DEFAULT_INPUT_SIZE];
	char wind_buffer[DEFAULT_INPUT_SIZE];
  
  while (row_count<nb_lines) {
		if (row_count==0){
			fscanf(ifp, "%s", _header_buffer);
		}
		else{
            char ID_station_buffer[DEFAULT_INPUT_SIZE];
	          char wind_direction_buffer[DEFAULT_INPUT_SIZE];
            char wind_speed_buffer[DEFAULT_INPUT_SIZE];
            char longitude_buffer[DEFAULT_INPUT_SIZE];
            char latitude_buffer[DEFAULT_INPUT_SIZE];


            int nb_cols = fscanf(ifp, "%[^;];%[^;];%[^;];%[^;];%s", ID_station_buffer, wind_direction_buffer, wind_speed_buffer, longitude_buffer, latitude_buffer);

			if (nb_cols == 5){
          if (strcmp(sort_algorithm, "tab") == 0){
              strcpy(records[row_count].ID_station, ID_station_buffer);
              strcpy(records[row_count].wind_direction, wind_direction_buffer);
              strcpy(records[row_count].wind_speed, wind_speed_buffer);
              strcpy(records[row_count].longitude, longitude_buffer);
              strcpy(records[row_count].latitude, latitude_buffer);
          }
          else if (strcmp(sort_algorithm, "abr") == 0){
                  printf("Sort Method structure abr \n");
                  wind_record record;
                  strcpy(record.ID_station, ID_station_buffer);
                  strcpy(record.wind_direction, wind_direction_buffer);
                  strcpy(record.wind_speed, wind_speed_buffer);
                  strcpy(record.longitude, longitude_buffer);
                  strcpy(record.latitude, latitude_buffer);
                  insert__wind(node_wind, record);
              }
          else if (strcmp(sort_algorithm, "avl") == 0){
                  wind_record record;
                  strcpy(record.ID_station, ID_station_buffer);
                  strcpy(record.wind_direction, wind_direction_buffer);
                  strcpy(record.wind_speed, wind_speed_buffer);
                  strcpy(record.longitude, longitude_buffer);
                  strcpy(record.latitude, latitude_buffer);
                  avl_node_wind = insertNodeAVL__wind(avl_node_wind, record);
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
    write_output_tab__wind(output_file, header, records, row_count, reversed_order);
    free(records);
  }

  if (strcmp(sort_algorithm, "abr") == 0){
    write_output_tree__wind(output_file, header, node_wind, reversed_order);
    free(node_wind);
  }

  if (strcmp(sort_algorithm, "avl") == 0){
    write_output_tree__wind(output_file, header, avl_node_wind, reversed_order);
    free(avl_node_wind);
  }


  return 0;
}
