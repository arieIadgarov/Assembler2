#ifndef ERRORS_H
#define ERRORS_H

#include "methodBank.h"
#include "symbolTable.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <ctype.h>
#include <math.h>

typedef struct Error
{
    int error_id;    /* Unique identifier for the error */
    char *error_msg; /* Error message associated with the error */
} Error;

/* Represents error codes for various types of errors */
typedef enum ERROR_CODES
{
    /* The following errors are macro related or general */
    ERROR_CODE_0 = 0,
    ERROR_CODE_1,
    ERROR_CODE_2,
    ERROR_CODE_3,

    ERROR_CODE_4,
    ERROR_CODE_5,

    /* Enumeration skip for future macro related errors */

    /* The following errors are related to reading command assembly lines */
    ERROR_CODE_6,
    ERROR_CODE_7,
    ERROR_CODE_8,
    ERROR_CODE_9,

    ERROR_CODE_10,

    ERROR_CODE_11,

    ERROR_CODE_12,
    ERROR_CODE_13
} ERROR_CODES;

/**
 * @brief Prints the error message corresponding to the given error code.
 *
 * This function takes an error code as input and prints the corresponding error message.
 * If the provided errorCode does not correspond to a valid error message in the error database,
 * this function will not print any message.
 *
 * @param errorCode The error code to be printed.
 */
void printError(int errorCode);

/**
 * @brief Throws out an error and exits the program if errorCode is non-zero.
 *
 * This function checks if the provided errorCode is non-zero. If it is, it prints the corresponding error message
 * and terminates the program with EXIT_FAILURE status using the exit() function. If the errorCode is zero,
 * indicating no error, this function does nothing.
 *
 * @param errorCode The error code to be checked.
 */
void errorThrowOut(int errorCode);

/**
 * @brief Performs error handling for a given line of text.
 *
 * This function checks the length of the provided line. If the length exceeds the maximum line length,
 * it sets the error_code pointer to the error code ERROR_CODE_30, indicating a line length error,
 * and returns ERROR_CODE_30. If the length is within the acceptable range, it sets the error_code pointer to 0
 * and returns 0, indicating no error.
 *
 * @param line A pointer to the line of text to be checked.
 * @param error_code A pointer to an integer variable where the error code will be stored.
 *                   If an error occurs, this variable will be updated with the corresponding error code.
 *                   If no error occurs, this variable will be set to 0.
 * @return If an error occurs (line length exceeds MAX_LINE_LENGTH), it returns the corresponding error code.
 *         If no error occurs, it returns 0.
 */
int errorHandlingLine(char *line, int *error_code);

/**
 * @brief Performs error handling for a line containing a ".define" directive.
 *
 * This function checks the syntax and content of a line containing a ".define" directive.
 * If the syntax or content is incorrect, it sets the error_code pointer to the appropriate error code ,
 * indicating a syntax error, and returns the appropriate error code. If the line is correctly formatted,
 * it sets the error_code pointer to 0 and returns 0, indicating no error.
 *
 * @param line A pointer to the line of text to be checked.
 * @param error_code A pointer to an integer variable where the error code will be stored.
 *                   If an error occurs, this variable will be updated with the corresponding error code.
 *                   If no error occurs, this variable will be set to 0.
 * @return If an error occurs (syntax or content error), it returns the corresponding error code.
 *         If no error occurs, it returns 0.
 */
int errorHandlingDefineLine(const char *line, int *error_code);

/**
 * @brief Checks if a given word is a declaration opcode.
 *
 * This function checks if the given word matches any of the known declaration opcodes.
 * If a match is found, it returns the index of the matching opcode.
 * If the word is NULL or does not match any known opcodes, it returns -1.
 *
 * @param word A pointer to the word to be checked.
 * @return If the word is a declaration opcode, it returns the index of the opcode.
 *         If the word is NULL or does not match any known opcodes, it returns -1.
 */
int IsADeclarcdeOpCode(char *word);

/**
 * @brief Checks if a given string is a register label.
 *
 * This function checks if the given string matches any of the known register labels.
 * If a match is found, it returns the index of the matching register.
 * If the string is NULL or does not match any known register labels, it returns -1.
 *
 * @param line A pointer to the string to be checked.
 * @return If the string is a register label, it returns the index of the register.
 *         If the string is NULL or does not match any known register labels, it returns -1.
 */
int IsRegistorLabel(char *line);

/**
 * @brief Checks if a given string is a legal label.
 *
 * This function checks if the given string is a legal label according to the specified rules.
 * A legal label must start with an alphabetical character, followed by alphanumeric characters or '_',
 * and end with a colon ':' character. The maximum length of a label is defined by MAX_LABEL_LENGTH.
 *
 * @param line A pointer to the string to be checked.
 * @param error_code A pointer to an integer variable where the error code will be stored.
 *                   If an error occurs, this variable will be updated with the corresponding error code.
 *                   If no error occurs, this variable will be set to 0.
 * @return If the string is a legal label, it returns 1. If not, it returns 0.
 */
int legal_label(char *line, int *error_code);

/**
 * @brief Checks if a label does not conflict with saved words (registers or opcodes).
 *
 * This function checks if the label extracted from the given line does not conflict with any saved words,
 * such as register labels or declaration opcodes.
 * If the label does not conflict with any saved words, it returns 1. Otherwise, it returns 0.
 *
 * @param line A pointer to the line containing the label to be checked.
 * @param error_code A pointer to an integer variable where the error code will be stored.
 *                   If an error occurs, this variable will be updated with the corresponding error code.
 *                   If no error occurs, this variable will be set to 0.
 * @return If the label does not conflict with any saved words, it returns 1.
 *         If the label conflicts with any saved words (registers or opcodes), it returns 0.
 */
int LabelSavedWord(char *line, int *error_code);

/**
 * @brief Performs error handling for a line containing a ".data" directive.
 *
 * This function checks the syntax and content of a line containing a ".data" directive.
 * It verifies that the label, if present, is legal, and then checks the syntax of the ".data" directive
 * and the numbers following it. It returns an error code if any syntax errors are detected.
 *
 * @param line A pointer to the line of text to be checked.
 * @param error_code A pointer to an integer variable where the error code will be stored.
 *                   If an error occurs, this variable will be updated with the corresponding error code.
 *                   If no error occurs, this variable will be set to 0.
 * @return If the line contains syntax errors, it returns the corresponding error code.
 *         If the line is correct, it returns 0.
 */
int dataLineErorEndeling(char *line, int *error_code, symbolList *mList);

/**
 * @brief Performs error handling for a line containing a ".string" directive.
 *
 * This function checks the syntax and content of a line containing a ".string" directive.
 * It verifies that the label, if present, is legal, and then checks the syntax of the ".string" directive
 * and the string literal following it. It returns an error code if any syntax errors are detected.
 *
 * @param line A pointer to the line of text to be checked.
 * @param error_code A pointer to an integer variable where the error code will be stored.
 *                   If an error occurs, this variable will be updated with the corresponding error code.
 *                   If no error occurs, this variable will be set to 0.
 * @return If the line contains syntax errors, it returns the corresponding error code.
 *         If the line is correct, it returns 0.
 */
int stringLineErorEndeling(char *line, int *error_code);

/**
 * @brief Performs error handling for a line containing an ".entry" directive.
 *
 * This function checks the syntax and content of a line containing an ".entry" directive.
 * It verifies that the syntax of the ".entry" directive is correct and that the following symbol (label)
 * is valid. It returns an error code if any syntax errors are detected.
 *
 * @param line A pointer to the line of text to be checked.
 * @param error_code A pointer to an integer variable where the error code will be stored.
 *                   If an error occurs, this variable will be updated with the corresponding error code.
 *                   If no error occurs, this variable will be set to 0.
 * @return If the line contains syntax errors, it returns the corresponding error code.
 *         If the line is correct, it returns 1 if there's an entry symbol and 0 if not.
 */
int entryErrorHandling(char *line, int *error_code);

/**
 * @brief Performs error handling for a line containing an ".extern" directive.
 *
 * This function checks the syntax and content of a line containing an ".extern" directive.
 * It verifies that the syntax of the ".extern" directive is correct and that the following symbol (label)
 * is valid. It returns an error code if any syntax errors are detected.
 *
 * @param line A pointer to the line of text to be checked.
 * @param error_code A pointer to an integer variable where the error code will be stored.
 *                   If an error occurs, this variable will be updated with the corresponding error code.
 *                   If no error occurs, this variable will be set to 0.
 * @return If the line contains syntax errors, it returns the corresponding error code.
 *         If the line is correct, it returns 1 if there's an external symbol and 0 if not.
 */
int externErrorHandling(char *line, int *error_code);

/**
 * @brief Performs error handling for a line containing an opcode instruction.
 *
 * This function checks the syntax and content of a line containing an opcode instruction.
 * It verifies that the opcode is valid and that the operands, if present, are syntactically correct.
 * It returns an error code if any syntax errors are detected.
 *
 * @param line A pointer to the line of text to be checked.
 * @param error_code A pointer to an integer variable where the error code will be stored.
 *                   If an error occurs, this variable will be updated with the corresponding error code.
 *                   If no error occurs, this variable will be set to 0.
 * @return If the line contains syntax errors, it returns the corresponding error code.
 *         If the line is correct, it returns 0.
 */
int OpCodeErrorHandeling(char *line, int *error_code);

#endif