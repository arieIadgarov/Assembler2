#ifndef SYMBOL_TABLE_H
#define SYMBOL_TABLE_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <ctype.h>
#include <math.h>


#define MAX_LINE_LENGTH 82
#define MAX_LABEL_LENGTH 32
#define MAX_NUMBER_LENGTH 6

/* Symbol structure */
typedef struct symbol {
    char *name;         /* Name associated with the symbol */
    char *properties;   /* 1 of 4 */
    int val;            /* val number  */
    struct symbol *next;  /* Pointer to the next symbol in the linked list */
} symbol;

/* Symbol list structure */
typedef struct symbolList{
    symbol* start;
    symbol* end;
} symbolList;


/* Function prototypes */

/**
 * @brief Checks if a symbol with the given name is defined as 'extern' in the symbol list.
 *
 * This function searches for a symbol with the specified name in the symbol list. If a symbol with
 * the given name and 'extern' property is found, it returns the value associated with that symbol.
 * If the symbol is not found or is not defined as 'extern', it returns 0.
 *
 * @param list A pointer to the symbol list.
 * @param symbolName A pointer to the name of the symbol to search for.
 * @return If a symbol with the given name and 'extern' property is found, it returns the value associated
 *         with that symbol. If the symbol is not found or is not defined as 'extern', it returns 0.
 */
int isSymbolExtern(symbolList *list, char* symbolName);

/**
 * @brief Adds a new symbol to the symbol list.
 *
 * This function creates a new symbol with the provided name, property, and value, and adds it to the
 * symbol list. If the list is empty, the new symbol becomes both the start and the end of the list.
 * If the list already contains symbols, the new symbol is added to the end of the list.
 *
 * @param name A pointer to the name of the symbol to be added.
 * @param property A pointer to the property of the symbol to be added.
 * @param val The value associated with the symbol to be added.
 * @param first Flag indicating if the new symbol should be the first in the list (1 for true, 0 for false).
 * @param mList A pointer to the symbol list to which the new symbol will be added.
 */
void addsymbol(char *name, char *property, int val, int first, symbolList *sList);

/**
 * @brief Retrieves the first symbol from the symbol list.
 *
 * This function returns a copy of the first symbol in the provided symbol list.
 *
 * @param list A pointer to the symbol list.
 * @return A copy of the first symbol in the list.
 */
symbol* getFirstSymbol(symbolList list);

/**
 * @brief Retrieves the last symbol from the symbol list.
 *
 * This function returns a copy of the last symbol in the provided symbol list.
 *
 * @param list A pointer to the symbol list.
 * @return A copy of the last symbol in the list.
 */
symbol* getLastSymbol(symbolList list);

/**
 * @brief Checks if a symbol with the given name already exists in the symbol table.
 *
 * This function searches for a symbol with the specified name in the provided symbol list.
 *
 * @param list A pointer to the symbol list.
 * @param symbolName The name of the symbol to search for.
 * @return 1 if the symbol is found in the table, 0 otherwise.
 */
int isSymbolAlreadyInTable(symbolList *list, char* symbolName);

/**
 * @brief Retrieves the value associated with the specified symbol from the symbol table.
 *
 * This function searches for a symbol with the specified name in the provided symbol list
 * and returns its associated value.
 *
 * @param list A pointer to the symbol list.
 * @param symbolName The name of the symbol to search for.
 * @return The value associated with the symbol if found, 0 otherwise.
 */
int getValFromSymbol(symbolList *list, char* symbolName);

/**
 * @brief Checks if a symbol with the given property already exists in the symbol table.
 *
 * This function searches for a symbol with the specified property in the provided symbol list.
 *
 * @param list A pointer to the symbol list.
 * @param property The property of the symbol to search for.
 * @return 1 if the symbol is found in the table, 0 otherwise.
 */
int isSymbolAlreadyInTableByprop(symbolList *list, char *property);


#endif /* SYMBOL_TABLE_H */
