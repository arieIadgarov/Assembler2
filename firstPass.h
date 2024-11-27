#ifndef EXE_FIRST_PASS_H
#define EXE_FIRST_PASS_H

#include "symbolTable.h"
#include "errors.h"
#include "methodBank.h"

/**
 * @brief Executes the first pass of the assembler.
 *
 * This function reads an assembly file in the first pass and generates a symbol table 
 * along with processing certain directives and instructions. It also generates output 
 * in the form of intermediate code or object code.
 *
 * @param file_name The name of the assembly file to process.
 * @param mList Pointer to the symbol list.
 * @return Returns an integer representing the success or failure of the operation.
 *         0 for success, non-zero for failure.
 */
int exe_first_pass(char *file_name, symbolList *mList, char * firstPassOutputFileName, int *DC, int *IC);

#endif /* EXE_FIRST_PASS_H */
