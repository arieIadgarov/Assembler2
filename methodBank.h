#ifndef METHOD_BANK_H
#define METHOD_BANK_H

#include "symbolTable.h"
#include "errors.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <ctype.h>
#include <math.h>

/* Structure to store opcode information */
typedef struct op_code {
    char *opcode;           /* The opcode corresponding to the operation */
    int numberOfOpcode;     /* How many operands the opcode has */
} op_code;


static const char * OPNumbersOfCode[16] = 
        {"mov","cmp","add","sub","not","clr","lea","inc"
        ,"dec","jmp","bne","red","prn", "jsr","rts", "hlt"};

/* Define the registers */
static char *REGS[] = {"r0", "r1", "r2", "r3", "r4", "r5", "r6", "r7"};

/*how many operands the opcodes have */
static op_code OPCODESNumberOfOperands[] = {
        {"mov",  2},
        {"cmp",  2},
        {"add",  2},
        {"sub",  2},
        {"not",  1},
        {"clr",  1},
        {"lea",  2},
        {"inc",  1},
        {"dec",  1},
        {"jmp",  1},
        {"bne",  1},
        {"red",  1},
        {"prn",  1},
        {"jsr",  1},
        {"rts",  0},
        {"hlt", 0}
};

bool is_opcode(const char* str);

char* extract_string(const char* line);

char **get_data_elements(char *line);

char* binaryToBase4(const char* binary);

int extract_number_after_hashtag(const char *word);

char *padBinary(char *binaryStr, int desiredLength);

char *decimalToBinary(int num);

char* extractSecondWord(char* line);

char* binaryToCustomBase(char* binaryCode);

/* Function declarations */
/**
 * @brief Extracts the word inside square brackets from a given input string.
 *
 * This function searches for a substring enclosed in square brackets within the input string.
 * If a valid substring is found, it allocates memory for the extracted word, copies the substring
 * into it, and returns a pointer to the extracted word. If no valid substring is found, or if the
 * square brackets are in an invalid position, it returns NULL.
 *
 * @param input A pointer to the input string to search for the word inside square brackets.
 * @return A pointer to the extracted word if a valid substring inside square brackets is found,
 *         or NULL if no valid substring is found or if the square brackets are in an invalid position.
 *         Note: The caller is responsible for freeing the memory allocated for the extracted word.
 */
char* extractWordInSquareBrackets(const char* input);


/**
 * @brief Extracts the word before square brackets from a given input string.
 *
 * This function searches for a substring preceding the first occurrence of '[' within the input string.
 * If '[' is found, it allocates memory for the extracted word, copies the substring before '[' into it,
 * and returns a pointer to the extracted word. If '[' is not found, it returns NULL.
 *
 * @param input A pointer to the input string to search for the word before square brackets.
 * @return A pointer to the extracted word if '[' is found, or NULL if '[' is not found in the input string.
 *         Note: The caller is responsible for freeing the memory allocated for the extracted word.
 */
char* extract_word_before_bracket(const char* line);


/**
 * @brief Checks if the operand contains a constant index and both extracted substrings are symbols.
 *
 * This function examines the provided operand string to determine if it contains a constant index enclosed
 * in square brackets. If such a constant index is found, it also checks if both the substring inside
 * the square brackets and the substring before the square brackets are symbols already present in the table.
 * If both substrings are symbols, it returns 1; otherwise, it returns 0. If the operand does not contain
 * a constant index or if symbols are not found, it returns 0.
 *
 * @param ptrToOperhand A pointer to the operand string to be analyzed.
 * @return 1 if the operand contains a constant index and both extracted substrings are symbols, 0 otherwise.
 */
int isIndexConstantOperhand(char *ptrToOperhand, symbolList* mList);


/**
 * @brief Determines the addressing method of the operand and returns its type.
 *
 * This function analyzes the provided operand string to determine its addressing method
 * and returns an integer representing the type of addressing method.
 * - If the operand is an immediate value, it returns 0.
 * - If the operand is a symbol already present in the symbol table, it returns 1.
 * - If the operand contains a constant index enclosed in square brackets and both extracted
 *   substrings are symbols already present in the symbol table, it returns 2.
 * - If the operand is a register, it returns 3.
 *
 * @param ptrToOperhand A pointer to the operand string to be analyzed.
 * @param mList A pointer to the symbol list used for symbol lookup.
 * @return An integer representing the type of addressing method for the operand:
 *         - 0 for immediate value.
 *         - 1 for symbol.
 *         - 2 for constant index.
 *         - 3 for register.
 */
int findTypeOperhand(char *ptrToOperhand, symbolList* mList);


/**
 * @brief Extracts the second operand from a line of assembly code.
 *
 * This function extracts the second operand from a line of assembly code by tokenizing
 * the instruction part of the line and searching for the specified operation code.
 * It then skips the operation code and the first operand to extract the second operand.
 * The function returns a pointer to the extracted second operand or NULL if the operation code
 * is not found or if there are not enough operands.
 *
 * @param line A pointer to the line of assembly code from which the second operand is to be extracted.
 * @param operation_codes An array of operation codes to search for in the line.
 * @param numOfIns The number of operation codes in the array.
 * @return A pointer to the extracted second operand, or NULL if the operation code is not found
 *         or if there are not enough operands.
 *         Note: The caller is responsible for freeing the memory allocated for the extracted operand.
 */char *extractSecondOperand(const char *line);


/**
 * @brief Counts the characters in a string specified by a .string directive.
 *
 * This function counts the characters in a string specified by a .string directive in the given line
 * of assembly code. It searches for the .string directive in the line, moves past any whitespace characters,
 * and checks if the string is enclosed in double quotes. If the string is found and properly enclosed,
 * it counts the characters between the double quotes (excluding the quotes themselves) and returns the count.
 * If the .string directive is not found or if the string is not properly enclosed, it returns 0.
 *
 * @param line A pointer to the line of assembly code to search for the .string directive.
 * @return The number of characters in the string specified by the .string directive if found and properly
 *         enclosed in double quotes; otherwise, it returns 0.
 */
int countStringChars(const char* line);


/**
 * @brief Counts the number of integers following the .data directive in a line of assembly code.
 *
 * This function counts the number of integers following the .data directive in the given line
 * of assembly code. It searches for the .data directive in the line, moves past any whitespace characters,
 * and then counts the integers separated by commas. Integers can be preceded by an optional sign (+ or -)
 * and may be followed by whitespace characters. The function returns the count of integers found.
 * If the .data directive is not found or if there are no integers following it, it returns 0.
 *
 * @param line A pointer to the line of assembly code to search for the .data directive and count integers.
 * @return The number of integers following the .data directive in the line, or 0 if the .data directive
 *         is not found or if there are no integers following it.
 */
int countDataNumbers(const char* line);


/**
 * @brief Finds the index of a register name in an array of register names.
 *
 * This function searches for a given word (register name) in an array of register names.
 * If the word is found in the array, the function returns the index of the word in the array.
 * If the word is not found, the function returns -1.
 *
 * @param word A pointer to the word (register name) to search for.
 * @return The index of the word in the array of register names if found, or -1 if not found.
 */
int findRegister(const char *word);


/**
 * @brief Extracts the word following a specified directive in a line of text.
 *
 * This function extracts the word following a specified directive in the given line of text.
 * It searches for the directive in the line, moves to the position after the directive,
 * skips leading whitespace characters, and finds the end of the word. It then allocates memory
 * for the word, copies it into the allocated memory, and returns a pointer to the extracted word.
 * If the directive is not found in the line, the function returns NULL.
 *
 * @param line A pointer to the line of text to search for the specified directive and extract the word from.
 * @param directive A pointer to the directive to search for in the line.
 * @return A pointer to the extracted word following the directive, or NULL if the directive is not found
 *         in the line or if there is no word following it.
 *         Note: The caller is responsible for freeing the memory allocated for the extracted word.
 */
char* extractWordAfterDirective(const char* line, const char* directive);


/**
 * @brief Extracts the operand following an operation code in a line of assembly code.
 *
 * This function extracts the operand following an operation code in the given line of assembly code.
 * It searches for the operation code in the line, tokenizes the instruction part to find the operation code,
 * and then extracts the operand that follows it. The function returns a pointer to the extracted operand.
 * If the operation code is not found in the line or if there is no operand following it, the function returns NULL.
 *
 * @param line A pointer to the line of assembly code to search for the operation code and extract the operand from.
 * @param operation_codes An array of operation codes to search for in the line.
 * @param numOfIns The number of operation codes in the array.
 * @return A pointer to the extracted operand following the operation code, or NULL if the operation code is not found
 *         in the line or if there is no operand following it.
 *         Note: The caller is responsible for freeing the memory allocated for the extracted operand.
 */
char *extractOperandDestination(const char *line);


/**
 * @brief Checks if a string represents a valid number.
 *
 * This function checks if the given string represents a valid number. It iterates through
 * each character of the string and verifies whether it is a digit, a positive or negative sign,
 * or a null terminator. If any character other than these is encountered, the function returns 0,
 * indicating that the string is not a valid number. Otherwise, if all characters are valid,
 * the function returns 1, indicating that the string represents a valid number.
 *
 * @param str A pointer to the string to be checked.
 * @return 1 if the string represents a valid number, 0 otherwise.
 */
int isNumber(const char* str);


/**
 * @brief Extracts the operand type 0 from a sentence.
 *
 * This function extracts the operand type 0 from the given sentence. Operand type 0 is defined
 * as the substring following the first occurrence of '#' in the sentence. The function skips leading
 * whitespace characters after '#' and extracts the word or number following it until a space or end of string
 * is encountered. It allocates memory for the extracted operand and returns a pointer to it. If the operand
 * is a symbol already present in the symbol table, the function returns it without further processing.
 * If the operand is neither a number nor a symbol in the symbol table, it returns NULL.
 *
 * @param sentence A pointer to the sentence from which to extract the operand number 0.
 * @return A pointer to the extracted operand number 0, or NULL if the operand is not found or is not
 *         a valid number nor a symbol in the symbol table.
 *         Note: The caller is responsible for freeing the memory allocated for the extracted operand.
 */
char* extractOperhand0(const char* sentence, symbolList* mList);


/**
 * @brief Extracts a string enclosed in double quotes from a line of text.
 *
 * This function extracts a string enclosed in double quotes from the given line of text.
 * It searches for the first occurrence of double quotes (") in the line, moves past it,
 * and then finds the next occurrence of double quotes to determine the end of the string.
 * The function then calculates the length of the substring between the double quotes,
 * allocates memory for the extracted string, copies the substring into the allocated memory,
 * and returns a pointer to the extracted string. If the opening or closing double quotes
 * are not found, the function returns NULL.
 *
 * @param line A pointer to the line of text from which to extract the string enclosed in double quotes.
 * @return A pointer to the extracted string enclosed in double quotes, or NULL if the opening or closing
 *         double quotes are not found in the line.
 *         Note: The caller is responsible for freeing the memory allocated for the extracted string.
 */
char* extractString(const char* line);


/**
 * @brief Converts a binary number by inverting all bits after the first '1'.
 *
 * This function converts a binary number by inverting all bits after the first '1'.
 * It searches for the index of the first '1' in the binaryNumber, then iterates through
 * the binaryNumber starting from the bit after the first '1' and inverts each bit.
 * The function returns a pointer to the converted binary number.
 * If no '1' is found in the binaryNumber, the function returns the original binaryNumber.
 *
 * @param binaryNumber A pointer to the binary number to be converted.
 * @return A pointer to the converted binary number.
 *         Note: The caller is responsible for freeing the memory allocated for the converted binary number.
 */
char* convertBinaryNumber(char* binaryNumber);


/**
 * @brief Calculates the two's complement of a binary number.
 *
 * This function calculates the two's complement of a binary number represented as an array of characters.
 * It first creates a copy of the input binaryArray and then performs the two's complement operation.
 * The function returns a pointer to the result, which is the two's complement of the input binary number.
 *
 * @param binaryArray An array of characters representing the binary number for which to calculate the two's complement.
 * @param numOfdigits The number of digits in the binary number.
 * @return A pointer to the two's complement of the input binary number.
 *         Note: The caller is responsible for freeing the memory allocated for the result.
 */
char* decimalToBinary2Complement(int num);
/*******************************************************************************************chage the api we change the method*/

/**
 * @brief Performs a bitwise NOT operation on a binary number.
 *
 * This function performs a bitwise NOT operation on the given binary number represented as an array of characters.
 * It inverts each bit of the binary number.
 *
 * @param binary An array of characters representing the binary number to perform the bitwise NOT operation on.
 */
void bitwiseNOT(char *binaryStr);


/**
 * @brief Adds one to a binary number represented as an array of characters.
 *
 * This function adds one to the given binary number represented as an array of characters.
 * It starts from the least significant bit (LSB) and iterates towards the most significant bit (MSB),
 * adding one while propagating the carry. If the LSB is '0', it changes it to '1' and resets the carry.
 * If the LSB is '1', it changes it to '0' and continues to propagate the carry to the next bit.
 * If there is a carry after processing all bits, it indicates overflow and a message is printed.
 *
 * @param binary An array of characters representing the binary number to which one will be added.
 *               After the operation, this array will contain the result.
 *               Note: The array must have enough space to accommodate the result.
 *               The function does not return the result separately, as it modifies the original array.
 *               It's the caller's responsibility to ensure that the array has enough capacity.
 *               If an overflow occurs, the function prints a message indicating overflow.
 */
void addOne(char *binaryStr); 


/**
 * @brief Parses the numbers following the .data directive in a line of text.
 *
 * This function parses the numbers following the .data directive in the given line of text.
 * It searches for the .data directive in the line, reads each number separated by commas,
 * and stores them in a dynamically allocated integer array. The function returns a pointer to
 * the array containing the parsed numbers. If the .data directive is not found in the line,
 * or if there are any errors in the number format, the function returns NULL.
 *
 * @param line A pointer to the line of text containing the .data directive and the numbers to parse.
 * @return A pointer to the dynamically allocated integer array containing the parsed numbers, or NULL
 *         if the .data directive is not found in the line or if there are any errors in the number format.
 *         Note: The caller is responsible for freeing the memory allocated for the returned array.
 */
int* parseDataNumbers(char *line);


/**
 * @brief Finds the numeric constant defined in a line of text.
 *
 * This function finds the numeric constant defined in the given line of text, typically following a ".define" directive.
 * It skips past the ".define" part, constant name, and any whitespace before and after the '=' character.
 * Then, it extracts the numeric constant and returns it as a dynamically allocated string.
 * If the numeric constant is not found or memory allocation fails, the function returns NULL.
 *
 * @param line A pointer to the line of text containing the definition of the numeric constant.
 * @return A pointer to the dynamically allocated string containing the numeric constant, or NULL if
 *         the numeric constant is not found or memory allocation fails.
 *         Note: The caller is responsible for freeing the memory allocated for the returned string.
 */
char* findNumericConstant(char *line);


/**
 * @brief Finds the constant name defined in a line of text.
 *
 * This function finds the constant name defined in the given line of text, typically following a ".define" directive.
 * It skips past the ".define" part and any leading whitespace.
 * Then, it extracts the constant name and returns it as a dynamically allocated string.
 * If the constant name is not found or memory allocation fails, the function returns NULL.
 *
 * @param line A pointer to the line of text containing the definition of the constant.
 * @return A pointer to the dynamically allocated string containing the constant name, or NULL if
 *         the constant name is not found or memory allocation fails.
 *         Note: The caller is responsible for freeing the memory allocated for the returned string.
 */
char* findConstant(char *line);


/**
 * @brief Checks if a string represents a valid label.
 *
 * This function checks if the given string represents a valid label according to the assembly language syntax.
 * A valid label starts with an alphabetic character, followed by alphanumeric characters or underscores.
 * It can be up to MAX_LABEL_LENGTH characters long.
 * The label must end with a colon (':') and should not contain leading or trailing whitespace.
 *
 * @param line A pointer to the string to be checked.
 * @return true if the string represents a valid label, false otherwise.
 */
bool isLabel(char *line);


/**
 * @brief Extracts the label from a line of text.
 *
 * This function extracts the label from the given line of text, assuming the line contains a valid label.
 * It allocates memory for the label, extracts the characters representing the label,
 * and returns the label as a dynamically allocated string.
 * If the line does not contain a valid label or memory allocation fails, the function returns NULL.
 *
 * @param line A pointer to the line of text containing the label.
 * @return A pointer to the dynamically allocated string containing the extracted label, or NULL if
 *         the line does not contain a valid label or memory allocation fails.
 *         Note: The caller is responsible for freeing the memory allocated for the returned string.
 */
char* getLabel(char *line);


/**
 * @brief Finds the number of operands for a given opcode name.
 *
 * This function searches for the given opcode name in a predefined array of opcode structures.
 * If a match is found, it returns the number of operands associated with that opcode.
 * If the opcode name is not found, the function returns -1.
 *
 * @param name A pointer to the opcode name to search for.
 * @return The number of operands for the given opcode name if found; otherwise, -1.
 */
int findNumberOfOperands(const char *name);


/**
 * @brief Copies a range of bits from one binary string to another.
 *
 * This function copies a range of bits from the input binary string `str` to the output binary string `output`.
 * It starts copying bits from the `start_bit` position (inclusive) and stops at the `end_bit` position (exclusive).
 * The `output` string should be pre-allocated and have enough space to accommodate the copied bits.
 *
 * @param str A pointer to the input binary string from which bits will be copied.
 * @param start_bit The starting bit position (inclusive) to begin copying from.
 * @param end_bit The ending bit position (exclusive) to stop copying.
 * @param output A pointer to the output binary string where the copied bits will be stored.
 *               This string should be pre-allocated and have enough space to accommodate the copied bits.
 */
void addBittsToBinary(char *str, int start_bit, int end_bit, char *output);


/**
 * @brief Converts a decimal number to a binary string representation.
 *
 * This function converts the given decimal number `n` to its binary representation.
 * The resulting binary string will have a specified number of digits `num_digits`.
 * The function dynamically allocates memory for the binary string and returns a pointer to it.
 * The caller is responsible for freeing the allocated memory when it's no longer needed.
 *
 * @param n The decimal number to convert to binary.
 * @param num_digits The desired number of digits in the binary representation.
 * @return A pointer to the dynamically allocated binary string representing the decimal number.
 *         The caller is responsible for freeing this memory.
 */
char* decimalToBinaryNOTUSE(int n, int num_digits);


/**
 * @brief Finds the index of the instruction in a given sentence.
 *
 * This function searches for an instruction in the provided sentence. It tokenizes the sentence
 * based on spaces and compares each token with the array of instructions. If a match is found,
 * it returns the index of the instruction in the array. If no instruction is found, it returns -1.
 *
 * @param sentence The sentence to search for the instruction.
 * @return The index of the instruction in the array if found, otherwise -1.
 */
int findInstructionInLine(const char *sentence);


/**
 * @brief Converts an integer to a binary string representation.
 *
 * This function converts the given integer to its binary representation as a string.
 * It ensures that the binary representation is padded to the specified number of bits.
 * The resulting binary string is stored in a static buffer and returned.
 *
 * @param num The integer to convert to binary.
 * @param num_bits The number of bits to represent the binary number.
 * @return A pointer to the binary string representation of the integer.
 */
char* bin(int num, int num_bits);

#endif /* SYMBOL_TABLE_H */
