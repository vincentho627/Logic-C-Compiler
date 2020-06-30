#include "assembly_writer.h"

void write_code(FILE *file, Parse_Tree *tree, Assignment *assignments) {
  assign_addr(tree);
  assign_value_to_tree(tree, assignments);
  // $1 stores the value for true
  fprintf(file, "mov r1,#1\n");
  // $2 stores the value for false
  fprintf(file, "mov r2,#0\n");
  evaluate(file, tree->root);
  fprintf(file, "and r0,r0,#1\n");
  return;
}

static void load_children_vals(FILE *file, uint32_t addr) {
  fprintf(file, "mov r3,#0x%x\n", MEMORY_OFFSET + ADDR_CHILD_L(addr));
  fprintf(file, "ldr r4,[r3]\n");
  fprintf(file, "mov r3,#0x%x\n", MEMORY_OFFSET + ADDR_CHILD_R(addr));
  fprintf(file, "ldr r5,[r3]\n");
  return;
}

static void store_intermediate_result(FILE *file, uint32_t addr) {
  fprintf(file, "mov r3,#0x%x\n", MEMORY_OFFSET + addr);
  fprintf(file, "str r0,[r3]\n");
  return;
}

void evaluate(FILE *file, Node *node) {
  if (is_empty(node)) {return;}

  // Case when node is a leaf, i.e. a variable
  if (!node->l && !node->r) {
    // $3 stores a temporary value for memory accessing
    fprintf(file, "mov r3,#0x%x\n", MEMORY_OFFSET + node->addr);
    if (node->val) {
      fprintf(file, "str r1,[r3]\n");
    } else {
      fprintf(file, "str r2,[r3]\n");
    }
    return;
  }

  // Case when node is an operator: Evaluate the children sub-trees
  evaluate(file, node->l);
  evaluate(file, node->r);

  // Store result in the node's memory address
  // $4, $5 store a temporary truth values that will later be used
  // $6 is another intermediate register for IMPLY and XNOR
  switch (node->op) {
    case NOT:
      fprintf(file, "mov r3,#0x%x\n", MEMORY_OFFSET + ADDR_CHILD_L(node->addr));
      fprintf(file, "ldr r4,[r3]\n");
      fprintf(file, "eor r0,r4,#1\n");
      store_intermediate_result(file, node->addr);
      break;
    case AND:
      load_children_vals(file, node->addr);
      fprintf(file, "and r0,r4,r5\n");
      store_intermediate_result(file, node->addr);
      break;
    case OR:
      load_children_vals(file, node->addr);
      fprintf(file, "orr r0,r4,r5\n");
      store_intermediate_result(file, node->addr);
      break;
    case IMPLY:
      load_children_vals(file, node->addr);
      fprintf(file, "eor r6,r4,#1\n");
      fprintf(file, "orr r0,r6,r5\n");
      store_intermediate_result(file, node->addr);
      break;
    case XNOR:
      load_children_vals(file, node->addr);
      fprintf(file, "eor r6,r4,r5\n");
      fprintf(file, "eor r0,r6,#1\n");
      store_intermediate_result(file, node->addr);
      break;
    default:
      perror("Parse tree is not syntactically correct");
      exit(EXIT_FAILURE);
  }
}

static void assign_value_to_node(Node *node, Assignment assign) {
  if (is_empty(node)) {
    return;
  }
  // Traverse through tree in post-order
  assign_value_to_node(node->l, assign);
  assign_value_to_node(node->r, assign);

  if (node->var == assign.var) {
    node->val = assign.val;
  }
  return;
}

void assign_value_to_tree(Parse_Tree *tree, Assignment *assignments) {
  for (int i = 0; i < MAX_D_COUNT; ++i) {
    assign_value_to_node(tree->root, assignments[i]);
  }
  return;
}
