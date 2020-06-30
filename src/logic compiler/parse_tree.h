#ifndef PARSE_TREE_H
#define PARSE_TREE_H

#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <math.h>
#include <assert.h>

#define CHECK_P(ptr) \
  do {if (ptr == NULL) {perror("Could not allocate memory"); exit(EXIT_FAILURE);}} while (0)
#define ADDR_CHILD_L(addr) (uint32_t) pow(2, addr) + 1
#define ADDR_CHILD_R(addr) (uint32_t) pow(2, addr) + 2
#define ADDR_PARENT(addr)  (uint32_t) (addr - 1) / 2

typedef enum {
  EMPTY = 0,
  NOT   = 1,
  AND   = 2,
  OR    = 3,
  IMPLY = 4,
  XNOR  = 5,
} operator;

typedef struct Node Node;

struct Node {
  char var;
  bool val;
  operator op;
  uint32_t addr;
  Node *l;
  Node *r;
};

typedef struct Parse_Tree {
  Node *root;
} Parse_Tree;

Node *create_node();

Parse_Tree *create_tree();

int is_empty(Node *n);

/**
 * Assigns var and op to the node's corresponding attributes
 */
void fill_node(Node *n, char var, operator op);

/**
 * 'Pushes' the root of t down to the left child of a newly-created empty node
 */
void push_to_tree(Parse_Tree *t);

/**
 * Converts a character to the correct operator type, or to EMPTY if it is a variable
 */
operator to_operator(char item);

/**
 * Adds a variable / operator to the tree as a node, depending on its binding strength
 */
void add_to_tree(Parse_Tree *t, char item);

/**
 * Merges t_sub into the shallowest empty node of t_main
 */
void merge_trees(Parse_Tree *t_main, Parse_Tree *t_sub);

/**
 * Applies func to every node in the tree in post-order traversal
 */
void map_to_tree(Parse_Tree *t, void (*func)(Node *));

void destroy_tree(Parse_Tree *t);

/**
 * Converts a formula string into its corresponding Parse_Tree
 */
Parse_Tree *to_tree(char *formula);

/**
 * Assigns every node in Parse_Tree an array address according to its position
 */
void assign_addr(Parse_Tree *t);

#endif
