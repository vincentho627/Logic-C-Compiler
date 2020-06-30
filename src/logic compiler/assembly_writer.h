#ifndef ASSEMBLY_WRITER_H
#define ASSEMBLY_WRITER_H

#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include "parse_tree.h"

#define MAX_D_COUNT   8
#define MEMORY_OFFSET 192

typedef struct Assignment {
  char var;
  bool val;
} Assignment;

void write_code(FILE *file, Parse_Tree *tree, Assignment *assignments);

void evaluate(FILE *file, Node *node);

void assign_value_to_tree(Parse_Tree *tree, Assignment *assignments);

#endif
