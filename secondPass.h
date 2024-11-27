#ifndef EXE_SECOND_PASS_H
#define EXE_SECOND_PASS_H

#include "symbolTable.h"
#include "methodBank.h"

/**
 * @brief Executes the second pass of the assembler.
 *
 * This function performs the second pass of the assembly process, where it processes
 * the assembly code generated in the first pass. It generates the final object code
 * by resolving symbols, filling in opcodes and operands, and writing the output to a file.
 *
 * @param inputFile The name of the input file containing assembly code.
 * @param mList Pointer to the symbol list.
 * @return Returns an integer representing the success or failure of the operation.
 *         0 for success, non-zero for failure.
 */
int exe_second_pass(char *inputFile, symbolList *mList, char *secondPassOutputFileName,int *DC,int *IC);

#endif /* EXE_SECOND_PASS_H */
