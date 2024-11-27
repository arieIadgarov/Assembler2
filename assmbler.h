#ifndef MAIN_H
#define MAIN_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <ctype.h>
#include <math.h>



/**
 * @brief The main function of the assembler program.
 *
 * This function serves as the entry point for the assembler program. It processes
 * command-line arguments to determine the input file, performs pre-assembly processing,
 * first pass assembly, and second pass assembly.
 *
 * @param argc The number of command-line arguments.
 * @param argv An array of strings containing the command-line arguments.
 * @return Returns an integer representing the success or failure of the operation.
 *         0 for success, non-zero for failure.
 */
int main(int argc, char *argv[]);

#endif /* MAIN_H */
