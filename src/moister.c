#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "common.h"
#include "moister.h"

///////////////////////////////////////////////////////////
// Data structures
///////////////////////////////////////////////////////////
typedef struct moister_record{
	char ID_station[DEFAULT_INPUT_SIZE]; 
	int moister;
	char longitude[DEFAULT_INPUT_SIZE] ; 
	char latitude[DEFAULT_INPUT_SIZE] ; 
}moister_record;

struct node {
  struct moister_record data; //node will store some data
  struct node *right_child; // right child
  struct node *left_child; // left child
  int height;
};
///////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////

void sortMoisterArray(moister_record records[], int size){
	int i,j;
	moister_record temp;
	for (i=0; i<size-1;i++){
		for (j=i+1; j<size;j++){
			if(records[i].moister>records[j].moister){
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

struct node* new_node__moister(moister_record record) {
  struct node *temp;
  temp = malloc(sizeof(struct node));
  temp -> data = record;
  temp -> left_child = NULL;
  temp -> right_child = NULL;

  return temp;
}

// insertion
struct node* insert__moister(struct node * root, moister_record record) {
  //searching for the place to insert
  if (root == NULL)
    return new_node__moister(record);
  else if (record.moister > root -> data.moister) // x is greater. Should be inserted to the right_child
    root -> right_child = insert__moister(root -> right_child, record);
  else // x is smaller and should be inserted to left_child
    root -> left_child = insert__moister(root -> left_child, record);
  return root;
}

// Calculate height
int height__moister(struct node *N) {
  if (N == NULL)
    return 0;
  return N->height;
}

int max__moister(int a, int b) {
  return (a > b) ? a : b;
}

// Create a AVL node 
struct node *newNodeAVL__moister(moister_record record) {
  struct node *temp;
  temp = malloc(sizeof(struct node));
  temp -> data = record;
  temp -> left_child = NULL;
  temp -> right_child = NULL;
  temp -> height = 1;
  return temp;
}

// Right rotate AVL
struct node *rightRotate__moister(struct node *y) {
  struct node *x = y->left_child;
  struct node *T2 = x->right_child;

  x->right_child = y;
  y->left_child = T2;

  y->height = max__moister(height__moister(y->left_child), height__moister(y->right_child)) + 1;
  x->height = max__moister(height__moister(x->left_child), height__moister(x->right_child)) + 1;

  return x;
}

// Left rotate AVL
struct node *leftRotate__moister(struct node *x) {
  struct node *y = x->right_child;
  struct node *T2 = y->left_child;

  y->left_child = x;
  x->right_child = T2;

  x->height = max__moister(height__moister(x->left_child), height__moister(x->right_child)) + 1;
  y->height = max__moister(height__moister(y->left_child), height__moister(y->right_child)) + 1;

  return y;
}

// Get the balance factor AVL
int getBalance__moister(struct node *N) {
  if (N == NULL)
    return 0;
  return height__moister(N->left_child) - height__moister(N->right_child);
}

// Insert node AVL
struct node *insertNodeAVL__moister(struct node *node, moister_record record) {
  // Find the correct position to insertNode the node and insertNode it
  if (node == NULL)
    return newNodeAVL__moister(record);
  // printf("Moister %d \n", node -> data.moister); // printing data at root
  if (record.moister < node->data.moister)
    node->left_child = insertNodeAVL__moister(node->left_child, record);
  else if (record.moister > node->data.moister)
    node->right_child = insertNodeAVL__moister(node->right_child, record);
  else
    return node;
  // printf("Moister %d \n", node -> data.moister); // printing data at root

  // Update the balance factor of each node and
  // Balance the tree
  node->height = 1 + max__moister(height__moister(node->left_child),
               height__moister(node->right_child));

  int balance = getBalance__moister(node);

  int compare_left = -1;
  int compare_right = -1;

  if (node->left_child != NULL){
    if(record.moister < node->left_child->data.moister)
      compare_left = 1;
    else if (record.moister > node->left_child->data.moister)
      compare_left = 0;
  }
    
  if (node->right_child != NULL){
    if(record.moister < node->right_child->data.moister)
      compare_right = 1;
    else if (record.moister > node->right_child->data.moister)
      compare_right = 0;
  }

  if (balance > 1 && compare_left==1)
    return rightRotate__moister(node);

  if (balance < -1 && compare_right == 0)
    return leftRotate__moister(node);

  if (balance > 1 && compare_left==0) {
    node->left_child = leftRotate__moister(node->left_child);
    return rightRotate__moister(node);
  }
  
  if (balance < -1 && compare_right == 1) {
    node->right_child = rightRotate__moister(node->right_child);
    return leftRotate__moister(node);
  }

  return node;
}

// Inorder Traversal
void inorder__moister(struct node *root) {
  if (root != NULL) // checking if the root is not null
  {
    inorder__moister(root -> left_child); // traversing left child
    printf(" %d \n", root -> data.moister); // printing data at root
    inorder__moister(root -> right_child); // traversing right child
  }
}

///////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////



///////////////////////////////////////////////////////////
// Write in output file
///////////////////////////////////////////////////////////
void write_output_tab__moister(char * output_file, char * header, moister_record records[], int row_count, int reversed_order){
  sortMoisterArray(records, row_count);
  FILE * ofp;
	ofp = fopen(output_file, "w");
	printf("Output File opened...\n");
	fprintf(ofp, "%s", header);
  if (reversed_order == 0){
    for (int i = 1; i < row_count; i++) {
        fprintf(ofp, "%s;%d;%s;%s", records[i].ID_station, records[i].moister, records[i].longitude, records[i].latitude);
    }
  }
  else{
    for (int i = row_count-1; i > 0; i--) {
        fprintf(ofp, "%s;%d;%s;%s", records[i].ID_station, records[i].moister, records[i].longitude, records[i].latitude);
    }
  }
	fclose(ofp);
	printf("Output File closed...\n");
  
}

void in_order_write__moister(FILE * ofp, struct node *root) {
  if (root != NULL) // checking if the root is not null
  {
    in_order_write__moister(ofp, root -> left_child); // traversing left child
    if (strcmp(root -> data.ID_station, "init") != 0){
    fprintf(ofp, "%s;%d;%s;%s", root -> data.ID_station, root -> data.moister, root -> data.longitude, root -> data.latitude);
    }
    in_order_write__moister(ofp, root -> right_child); // traversing right child
  }
}

void reversed_order_write__moister(FILE * ofp, struct node *root) {
  if (root != NULL) // checking if the root is not null
  {
    reversed_order_write__moister(ofp, root -> right_child); // traversing right child
    if (strcmp(root -> data.ID_station, "init") != 0){
      fprintf(ofp, "%s;%d;%s;%s", root -> data.ID_station, root -> data.moister, root -> data.longitude, root -> data.latitude);
    }
    reversed_order_write__moister(ofp, root -> left_child); // traversing left child
  }
}

void write_output_tree__moister(char * output_file, char * header, struct node * node_moister, int reversed_order){
  FILE * ofp;
	ofp = fopen(output_file, "w");
	printf("Output File opened...\n");
	fprintf(ofp, "%s", header);
  if (reversed_order==0){
      in_order_write__moister(ofp, node_moister);
  }
  else{
      reversed_order_write__moister(ofp, node_moister);
  }

  fclose(ofp);
	printf("Output File closed...\n");

}

///////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////


int main_moister(char* input_file, char* output_file, char* sort_algorithm, int reversed_order){
    printf("Moister main entred \n");

	int nb_lines = count_nb_lines_in_file(input_file);
	printf("nb lines in input files: %d\n", nb_lines);
  moister_record init_record;
  strcpy(init_record.ID_station, "init");
  init_record.moister = 0;
  strcpy(init_record.longitude, "init");
  strcpy(init_record.latitude, "init");

  moister_record* records;
  struct node* node_moister = new_node__moister(init_record);
  struct node* avl_node_moister = newNodeAVL__moister(init_record);

  if (strcmp(sort_algorithm, "tab") == 0){
      printf("Sort Method structure tab \n");
      records = malloc(nb_lines*sizeof(moister_record));
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
		return 2;
	}

	

	int row_count = 0;
	int nb_parsing_errors = 0;
	char _header_buffer[ROW_SIZE];
	char header[] = "ID_station;Humidite;Longitude;Latitude";
	char coords_buffer[DEFAULT_INPUT_SIZE];
	char moister_buffer[DEFAULT_INPUT_SIZE];

    while (row_count<nb_lines) {
		if (row_count==0){
			fscanf(ifp, "%s", _header_buffer);
		}
		else{
            char ID_station_buffer[DEFAULT_INPUT_SIZE];
	          char moister_buffer[DEFAULT_INPUT_SIZE];
            char longitude_buffer[DEFAULT_INPUT_SIZE];
            char latitude_buffer[DEFAULT_INPUT_SIZE];

            int nb_cols = fscanf(ifp, "%[^;];%[^;];%[^;];%s", ID_station_buffer, moister_buffer, longitude_buffer, latitude_buffer);

			if (nb_cols == 4){
          if (strcmp(sort_algorithm, "tab") == 0){
              strcpy(records[row_count].ID_station, ID_station_buffer);
              records[row_count].moister = atoi(moister_buffer);
              strcpy(records[row_count].longitude, longitude_buffer);
              strcpy(records[row_count].latitude, latitude_buffer);
          }
          else if (strcmp(sort_algorithm, "abr") == 0){
                  printf("Sort Method structure abr \n");
                  moister_record record;
                  strcpy(record.ID_station, ID_station_buffer);
                  record.moister = atoi(moister_buffer);
                  strcpy(record.longitude, longitude_buffer);
                  strcpy(record.latitude, latitude_buffer);
                  insert__moister(node_moister, record);
              }
          else if (strcmp(sort_algorithm, "avl") == 0){
                  moister_record record;
                  strcpy(record.ID_station, ID_station_buffer);
                  record.moister = atoi(moister_buffer);
                  strcpy(record.longitude, longitude_buffer);
                  strcpy(record.latitude, latitude_buffer);
                  avl_node_moister = insertNodeAVL__moister(avl_node_moister, record);
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
    write_output_tab__moister(output_file, header, records, row_count, reversed_order);
    free(records);
  }

  if (strcmp(sort_algorithm, "abr") == 0){
    write_output_tree__moister(output_file, header, node_moister, reversed_order);
    free(node_moister);
  }

  if (strcmp(sort_algorithm, "avl") == 0){
    write_output_tree__moister(output_file, header, avl_node_moister, reversed_order);
    free(avl_node_moister);
  }


  return 0;
}
