#ifndef PRE_ASSEMBLER_H
#define PRE_ASSEMBLER_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <stdbool.h>
#include <math.h>
#include "macroTable.h"
#include "symbolTable.h"

#define MAX_LINE_LENGTH 82

/**
 * @brief Pre-processes the input file by expanding macros.
 *
 * This function reads the input file line by line, expands macros if encountered,
 * and writes the processed content to an output file. It handles macro definitions
 * and macro usage within the input file.
 *
 * @param inputFile The name of the input file containing assembly code.
 * @return Returns an integer representing the success or failure of the operation.
 *         0 for success, non-zero for failure.
 */
int pre_Assmbler(char *inputFile, macroList* mList, char *preAsmblrOutputFileName);

#endif /* PRE_ASSEMBLER_H */
