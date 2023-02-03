#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "common.h"
#include "height.h"

///////////////////////////////////////////////////////////
// Data structures
///////////////////////////////////////////////////////////
typedef struct height_record{
	char ID_station[DEFAULT_INPUT_SIZE]; 
	int height;
	char longitude[DEFAULT_INPUT_SIZE] ; 
	char latitude[DEFAULT_INPUT_SIZE] ; 
}height_record;

struct node {
  struct height_record data; //node will store some data
  struct node *right_child; // right child
  struct node *left_child; // left child
  int height;
};
///////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////

void sortHeightArray(height_record records[], int size){
	int i,j;
	height_record temp;
	for (i=0; i<size-1;i++){
		for (j=i+1; j<size;j++){
			if(records[i].height>records[j].height){
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

struct node* new_node__height(height_record record) {
  struct node *temp;
  temp = malloc(sizeof(struct node));
  temp -> data = record;
  temp -> left_child = NULL;
  temp -> right_child = NULL;

  return temp;
}

// insertion
struct node* insert__height(struct node * root, height_record record) {
  //searching for the place to insert
  if (root == NULL)
    return new_node__height(record);
  else if (record.height > root -> data.height) // x is greater. Should be inserted to the right_child
    root -> right_child = insert__height(root -> right_child, record);
  else // x is smaller and should be inserted to left_child
    root -> left_child = insert__height(root -> left_child, record);
  return root;
}

// Calculate height
int height__height(struct node *N) {
  if (N == NULL)
    return 0;
  return N->height;
}

int max__height(int a, int b) {
  return (a > b) ? a : b;
}

// Create a AVL node 
struct node *newNodeAVL__height(height_record record) {
  struct node *temp;
  temp = malloc(sizeof(struct node));
  temp -> data = record;
  temp -> left_child = NULL;
  temp -> right_child = NULL;
  temp -> height = 1;
  return temp;
}

// Right rotate AVL
struct node *rightRotate__height(struct node *y) {
  struct node *x = y->left_child;
  struct node *T2 = x->right_child;

  x->right_child = y;
  y->left_child = T2;

  y->height = max__height(height__height(y->left_child), height__height(y->right_child)) + 1;
  x->height = max__height(height__height(x->left_child), height__height(x->right_child)) + 1;

  return x;
}

// Left rotate AVL
struct node *leftRotate__height(struct node *x) {
  struct node *y = x->right_child;
  struct node *T2 = y->left_child;

  y->left_child = x;
  x->right_child = T2;

  x->height = max__height(height__height(x->left_child), height__height(x->right_child)) + 1;
  y->height = max__height(height__height(y->left_child), height__height(y->right_child)) + 1;

  return y;
}

// Get the balance factor AVL
int getBalance__height(struct node *N) {
  if (N == NULL)
    return 0;
  return height__height(N->left_child) - height__height(N->right_child);
}

// Insert node AVL
struct node *insertNodeAVL__height(struct node *node, height_record record) {
  // Find the correct position to insertNode the node and insertNode it
  if (node == NULL)
    return newNodeAVL__height(record);
  // printf("Height %d \n", node -> data.height); // printing data at root
  if (record.height < node->data.height)
    node->left_child = insertNodeAVL__height(node->left_child, record);
  else if (record.height > node->data.height)
    node->right_child = insertNodeAVL__height(node->right_child, record);
  else
    return node;
  // printf("Height %d \n", node -> data.height); // printing data at root

  // Update the balance factor of each node and
  // Balance the tree
  node->height = 1 + max__height(height__height(node->left_child),
               height__height(node->right_child));

  int balance = getBalance__height(node);
  
  int compare_left = -1;
  int compare_right = -1;

  if (node->left_child != NULL){
    if(record.height < node->left_child->data.height)
      compare_left = 1;
    else if (record.height > node->left_child->data.height)
      compare_left = 0;
  }
    
  if (node->right_child != NULL){
    if(record.height < node->right_child->data.height)
      compare_right = 1;
    else if (record.height > node->right_child->data.height)
      compare_right = 0;
  }

  if (balance > 1 && compare_left==1)
    return rightRotate__height(node);

  return node;
}

// Inorder Traversal
void inorder__height(struct node *root) {
  if (root != NULL) // checking if the root is not null
  {
    inorder__height(root -> left_child); // traversing left child
    printf(" %d \n", root -> data.height); // printing data at root
    inorder__height(root -> right_child); // traversing right child
  }
}

///////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////



///////////////////////////////////////////////////////////
// Write in output file
///////////////////////////////////////////////////////////
void write_output_tab__height(char * output_file, char * header, height_record records[], int row_count, int reversed_order){
  sortHeightArray(records, row_count);
  FILE * ofp;
	ofp = fopen(output_file, "w");
	printf("Output File opened...\n");
	fprintf(ofp, "%s", header);
  if (reversed_order == 0){
    for (int i = 1; i < row_count; i++) {
        fprintf(ofp, "%s;%d;%s;%s", records[i].ID_station, records[i].height, records[i].longitude, records[i].latitude);
    }
  }
  else{
    for (int i = row_count-1; i > 0; i--) {
        fprintf(ofp, "%s;%d;%s;%s", records[i].ID_station, records[i].height, records[i].longitude, records[i].latitude);
    }
  }
	fclose(ofp);
	printf("Output File closed...\n");
  
}

void in_order_write__height(FILE * ofp, struct node *root) {
  if (root != NULL) // checking if the root is not null
  {
    in_order_write__height(ofp, root -> left_child); // traversing left child
    if (strcmp(root -> data.ID_station, "init") != 0){
    fprintf(ofp, "%s;%d;%s;%s", root -> data.ID_station, root -> data.height, root -> data.longitude, root -> data.latitude);
    }
    in_order_write__height(ofp, root -> right_child); // traversing right child
  }
}

void reversed_order_write__height(FILE * ofp, struct node *root) {
  if (root != NULL) // checking if the root is not null
  {
    reversed_order_write__height(ofp, root -> right_child); // traversing right child
    if (strcmp(root -> data.ID_station, "init") != 0){
      fprintf(ofp, "%s;%d;%s;%s", root -> data.ID_station, root -> data.height, root -> data.longitude, root -> data.latitude);
    }
    reversed_order_write__height(ofp, root -> left_child); // traversing left child
  }
}

void write_output_tree__height(char * output_file, char * header, struct node * node_height, int reversed_order){
  FILE * ofp;
	ofp = fopen(output_file, "w");
	printf("Output File opened...\n");
	fprintf(ofp, "%s", header);
  if (reversed_order==0){
      in_order_write__height(ofp, node_height);
  }
  else{
      reversed_order_write__height(ofp, node_height);
  }

  fclose(ofp);
	printf("Output File closed...\n");

}

///////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////


int main_height(char* input_file, char* output_file, char* sort_algorithm, int reversed_order){
    printf("Height main entred \n");

	int nb_lines = count_nb_lines_in_file(input_file);
	printf("nb lines in input files: %d\n", nb_lines);
  height_record init_record;
  strcpy(init_record.ID_station, "init");
  init_record.height = 0;
  strcpy(init_record.longitude, "init");
  strcpy(init_record.latitude, "init");

  height_record* records;
  struct node* node_height = new_node__height(init_record);
  struct node* avl_node_height = newNodeAVL__height(init_record);

  if (strcmp(sort_algorithm, "tab") == 0){
      printf("Sort Method structure tab \n");
      records = malloc(nb_lines*sizeof(height_record));
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
	char header[] = "ID_station;Altitude;Longitude;Latitude";
	char coords_buffer[DEFAULT_INPUT_SIZE];
	char height_buffer[DEFAULT_INPUT_SIZE];

    while (row_count<nb_lines) {
		if (row_count==0){
			fscanf(ifp, "%s", _header_buffer);
		}
		else{
            char ID_station_buffer[DEFAULT_INPUT_SIZE];
	          char height_buffer[DEFAULT_INPUT_SIZE];
            char longitude_buffer[DEFAULT_INPUT_SIZE];
            char latitude_buffer[DEFAULT_INPUT_SIZE];

            int nb_cols = fscanf(ifp, "%[^;];%[^;];%[^;];%s", ID_station_buffer, height_buffer, longitude_buffer, latitude_buffer);

			if (nb_cols == 4){
          if (strcmp(sort_algorithm, "tab") == 0){
              strcpy(records[row_count].ID_station, ID_station_buffer);
              records[row_count].height = atoi(height_buffer);
              strcpy(records[row_count].longitude, longitude_buffer);
              strcpy(records[row_count].latitude, latitude_buffer);
          }
          else if (strcmp(sort_algorithm, "abr") == 0){
                  printf("Sort Method structure abr \n");
                  height_record record;
                  strcpy(record.ID_station, ID_station_buffer);
                  record.height = atoi(height_buffer);
                  strcpy(record.longitude, longitude_buffer);
                  strcpy(record.latitude, latitude_buffer);
                  insert__height(node_height, record);
              }
          else if (strcmp(sort_algorithm, "avl") == 0){
                  height_record record;
                  strcpy(record.ID_station, ID_station_buffer);
                  record.height = atoi(height_buffer);
                  strcpy(record.longitude, longitude_buffer);
                  strcpy(record.latitude, latitude_buffer);
                  avl_node_height = insertNodeAVL__height(avl_node_height, record);
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
    write_output_tab__height(output_file, header, records, row_count, reversed_order);
    free(records);
  }

  if (strcmp(sort_algorithm, "abr") == 0){
    write_output_tree__height(output_file, header, node_height, reversed_order);
    free(node_height);
  }

  if (strcmp(sort_algorithm, "avl") == 0){
    write_output_tree__height(output_file, header, avl_node_height, reversed_order);
    free(avl_node_height);
  }


  return 0;
}
