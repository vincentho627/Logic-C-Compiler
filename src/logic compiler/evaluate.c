#include "evaluate.h"

static void init_assignments(Assignment *assignments) {
  for (int i = 0; i < MAX_D_COUNT; ++i) {
    assignments[i].var = ' ';
    assignments[i].val = false;
  }
  return;
}

bool are_equal(Assignment *assignments) {
  bool result = assignments[0].val;
  for (int i = 1; i < MAX_D_COUNT; ++i) {
    result = result && assignments[i].val;
  }
  return result;
}

/**
 * Reads through source file and lists out all declared variables and the formula
 */
static void decode_source_format(FILE *file, Assignment *assignments, char *formula) {
  assert(file != NULL);
  char line[LINE_LENGTH];
  char arg[LINE_LENGTH];
  char tokens[2];
  int index = 0;
  bool value;

  while (fgets(line, LINE_LENGTH, file) != NULL) {
    if (line[0] == 'd') {
      // Writes the assignment values
      strcpy(arg, &line[2]);
      tokens[0] = arg[0];
      tokens[1] = ' ';
      for (int i = 0; i < strlen(arg); ++i) {
        if (arg[i] != ' ' && arg[i] != '=' && arg[i] != '\n') {
          tokens[1] = arg[i];
        }
      }

      assignments[index].var = tokens[0];
      if (tokens[1] == '0') {
        value = false;
      } else if (tokens[1] == '1') {
        value = true;
      } else {
        perror("Truth value could not be identified\n");
        exit(EXIT_FAILURE);
      }
      assignments[index].val = value;
      index++;
      if (index >= MAX_D_COUNT) {
        perror("Program does not support over 8 variables\n");
        exit(EXIT_FAILURE);
      }
    } else if (line[0] == 'e') {
      // Puts the formula into the tree
      strcpy(formula, &line[2]);
    }
  }

  return;
}

static void print_file(char *filepath) {
  printf(" \n");
  FILE *file = fopen(filepath, "r");
  if (file == NULL) {
    perror("Help file could not be found, how ironic lmao");
    return;
  }

  char buffer[LINE_LENGTH];
  while (fgets(buffer, LINE_LENGTH, file) != NULL) {
    printf("%s", buffer);
  }
  printf(" \n \n");
  return;
}

int main(int argc, char **argv) {
  assert(argc < 4);
  if (argc == 1) {
    printf(" \n");
    printf("Usage: evaluate <source-file> <assembly-file>\n");
    printf("– Compiles <source-file> containing propositional logic into <assembly-file>.\n");
    printf(" \n");
    printf("Usage: evaluate help\n");
    printf("– Provides more information on how to write a valid source file.\n");
    printf(" \n");
    return EXIT_SUCCESS;
  }

  if (strncmp(argv[1], "help", 4) == 0) {
    print_file("./help.txt");
    return EXIT_SUCCESS;
  }

  Assignment *assignments = malloc(MAX_D_COUNT * sizeof(Assignment));
  CHECK_P(assignments);
  init_assignments(assignments);
  char *formula = malloc(LINE_LENGTH * sizeof(char));
  CHECK_P(formula);
  Parse_Tree *tree = create_tree();

  FILE *file_src = fopen(argv[1], "r");
  FILE *file_out = fopen(argv[2], "w");
  if (file_src == NULL || file_out == NULL) {exit(EXIT_FAILURE);}

  decode_source_format(file_src, assignments, formula);
  tree = to_tree(formula);
  write_code(file_out, tree, assignments);

  printf(" \n");
  printf("Run the following file on assembler and emulator, and find the result at $0:\n");
  printf("%s\n", argv[2]);
  printf(" \n");

  fclose(file_src);
  fclose(file_out);
  return EXIT_SUCCESS;
}
