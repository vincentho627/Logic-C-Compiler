#ifndef EVALUATE_H
#define EVALUATE_H

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <assert.h>
#include "parse_tree.h"
#include "assembly_writer.h"

#define LINE_LENGTH 512

/**
 * Checks if all the variables have the same truth value
 */
bool are_equal(Assignment *assignments);

int main(int argc, char **argv);

#endif
