#include "parse_tree.h"

Node *create_node() {
  Node *new_node = malloc(sizeof(Node));
  CHECK_P(new_node);
  new_node->var = ' ';
  new_node->op = EMPTY;
  new_node->addr = 0;
  new_node->l = new_node->r = NULL;
  return new_node;
}

Parse_Tree *create_tree() {
  Parse_Tree *new_tree = malloc(sizeof(Parse_Tree));
  CHECK_P(new_tree);
  new_tree->root = create_node();
  return new_tree;
}

int is_empty(Node *n) {
  return (n == NULL) || (n->var == ' ' && n->op == EMPTY);
}

void fill_node(Node *n, char var, operator op) {
  n->var = var;
  n->op = op;
  return;
}

static Node *push_at_node(Node *n) {
  Node *new_node = create_node();
  new_node->l = n;
  return new_node;
}

void push_to_tree(Parse_Tree *t) {
  t->root = push_at_node(t->root);
  return;
}

operator to_operator(char item) {
  switch (item) {
    case '!':
      return NOT;
    case '&':
      return AND;
    case '|':
      return OR;
    case '>':
      return IMPLY;
    case '-':
      return XNOR;
    default:
      return EMPTY;
  }
}

static Node *add_at_node(Node *n, char item) {
  operator op = to_operator(item);

  if (is_empty(n)) {
    if (n == NULL) {n = create_node();}
    fill_node(n, item, op);
    return n;
  }

  if (op < n->op) {
    if (is_empty(n->l)) {
      n->l = add_at_node(n->l, item);
    } else {
      n->r = add_at_node(n->r, item);
    }
    return n;
  }

  Node *new_node = push_at_node(n);
  fill_node(new_node, item, op);
  return new_node;
}

void add_to_tree(Parse_Tree *t, char item) {
  t->root = add_at_node(t->root, item);
  return;
}

void merge_trees(Parse_Tree *t_main, Parse_Tree *t_sub) {
  assert(t_main->root);
  if (is_empty((t_main->root)->l)) {
    (t_main->root)->l = t_sub->root;
  } else if (is_empty((t_main->root)->r)) {
    (t_main->root)->r = t_sub->root;
  } else {
    perror("t_main is already filled and could not be merged");
  }
}

static void map_by_node(Node *n, void (*func)(Node *)) {
  if (is_empty(n)) {
    return;
  }
  Node *m = n;
  map_by_node(m->l, func);
  map_by_node(m->r, func);
  func(m);
  return;
}

void map_to_tree(Parse_Tree *t, void (*func)(Node *)) {
  map_by_node(t->root, func);
  return;
}

static void destroy_node(Node *n) {
  free(n);
  return;
}

void destroy_tree(Parse_Tree *t) {
  map_to_tree(t, &destroy_node);
  return;
}

static int find_subformula(char *formula, char *subformula) {
  int index = 0;
  int bracket_count = -1;
  while (bracket_count != 0 || formula[index] != ')') {
    switch (formula[index]) {
      case '(':
        bracket_count++;
        break;
      case ')':
        bracket_count--;
        break;
    }
    index++;
    assert(index < strlen(formula));
  }
  strncpy(subformula, &formula[1], index - 1);
  return index;
}

Parse_Tree *to_tree(char *formula) {
  Parse_Tree *new_tree = create_tree();
  Parse_Tree *sub_tree;
  char *subformula = malloc(strlen(formula) * sizeof(char));
  CHECK_P(subformula);

  int index = 0;
  while (index < strlen(formula)) {
    if (formula[index] == '(') {
      index += find_subformula(&formula[index], subformula);
      sub_tree = to_tree(subformula);
      merge_trees(new_tree, sub_tree);
    } else if (formula[index] != '\n') {
      add_to_tree(new_tree, formula[index]);
    }
    index++;
  }

  free(subformula);
  return new_tree;
}

static void assign_addr_to_node(Node *n) {
  if (is_empty(n)) {
    return;
  }
  uint32_t addr = n->addr;
  if (n->l) {(n->l)->addr = ADDR_CHILD_L(addr);}
  if (n->r) {(n->r)->addr = ADDR_CHILD_R(addr);}
  assign_addr_to_node(n->l);
  assign_addr_to_node(n->r);
  return;
}

void assign_addr(Parse_Tree *t) {
  assign_addr_to_node(t->root);
  return;
}
