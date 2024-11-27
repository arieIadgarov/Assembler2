
#include "symbolTable.h"

/* the symbolTable */

void addsymbol(char *name, char *property, int val, int first, symbolList *mList)
{
    /*error endeling asssymbol*/
    symbol *newSymbol = (symbol *)malloc(sizeof(symbol));
    if (newSymbol == NULL)
    {
        printf("error memory");
        exit(EXIT_FAILURE);
    }

    /*name*/
    newSymbol->name = (char *)malloc(sizeof(char) * 81);
    if (newSymbol->name == NULL)
    {
        printf("error memory\n");
        exit(1);
    }

    if (name == NULL)
    {
        printf("error memory\n");
        exit(1);
    }

    strcpy(newSymbol->name, name);

    newSymbol->properties = (char *)malloc(sizeof(char) * 81);
    if (newSymbol->properties == NULL)
    {
        printf("error memory");
        exit(1);
    }
    strcpy(newSymbol->properties, property);
    newSymbol->val = val;
    newSymbol->next = NULL;

    if (first)
    {
        mList->start = newSymbol;
        mList->end = newSymbol;
    }
    else
    {
        mList->end->next = newSymbol;
        mList->end = newSymbol;
    }
}

struct symbol getFirstSymble(symbolList *list)
{
    return *list->start;
}

struct symbol getLastSymble(symbolList *list)
{
    return *list->end;
}

int isSymbolAlreadyInTable(symbolList *list, char *symbolName)
{

    symbol *current;
    if (list == NULL)
    {
        printf("enterd - list is null\n");
    }
    current = list->start;

    symbolName[strcspn(symbolName, "\r\n")] = 0;
    symbolName[strcspn(symbolName, "\n")] = 0;

    /* Iterate through the list */
    while (current != NULL)
    {
        /* Compare the name */
        if (strcmp(current->name, symbolName) == 0)
        {
            return 1; /* Symbol with the given name found */
        }
        current = current->next;
    }
    return 0; /* Symbol not found */
}

int getValFromSymbol(symbolList *list, char *symbolName)
{
    symbol *current = list->start;

    /* Iterate through the list */
    while (current != NULL)
    {
        /* Compare the name */
        if (strcmp(current->name, symbolName) == 0)
        {
            return current->val; /* Symbol with the given name found */
        }
        current = current->next;
    }
    return 0; /* Symbol not found */
}

int isSymbolExtern(symbolList *list, char *symbolName)
{
    symbol *current = list->start;

    /* Iterate through the list */
    while (current != NULL)
    {
        /* Compare the name */
        if (strcmp(current->properties, "extern") == 0)
        {
            return current->val; /* Symbol with the given name found */
        }
        current = current->next;
    }
    return 0; /* Symbol not found */
}

int isSymbolAlreadyInTableByprop(symbolList *list, char *property)
{
    symbol *current;
    if (list == NULL)
    {
        printf("enterd - list is null\n");
    }
    current = list->start;

    /* Iterate through the list */
    while (current != NULL)
    {

        /* Compare the name */
        if (strcmp(current->properties, property) == 0)
        {
            printf("%s found!\n", property);
            return 1; /* Symbol with the given name found */
        }
        current = current->next;
    }
    printf("%s not found!\n", property);
    return 0; /* Symbol not found */
}
