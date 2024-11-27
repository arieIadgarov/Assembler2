#ifndef MACROTABLE_H
#define MACROTABLE_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <ctype.h>
#include <math.h>

typedef struct macroData {
    char* data;
    int size;
    struct macroData* next;
} macroData;

typedef struct macro {
    char* name;
    macroData* dataFirst;
    macroData* dataLast;
    struct macro* next;

    char *content;
} macro;

typedef struct macroList {
    macro* start;
    macro* end;
} macroList;

macroList* mList;/*Initialized list */
macro* newM;
macroData* newData;

char* find_word_after_mcr(const char* line);
void addMacro(char* line, int firstMacro, macroList *mList);
void addMacroData(char* line, int firstData, macroList *mList);

void addMacroContent(char* line, macroList *mList);
macro* getMacroByName(const char* name, const macroList* mList);
int isMacroInTable(char* name, macroList* mList);
#endif /* MACROTABLE_H */
