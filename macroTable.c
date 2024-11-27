
#include "macroTable.h"


char* find_word_after_mcr(const char* line){
    const char* macroString = "mcr";
    const char* delimiters = " \t\n";
    const char* wordEnd;
    char* word;
    size_t wordLength;
    /* Find the position of "macro" in the line*/
    const char* macroPos = strstr(line, macroString);
    if (macroPos == NULL) {
        return NULL; /* "macro" not found in the line*/
    }

    /* Move to the position after "macro"*/
    macroPos += strlen(macroString);

    /* Skip leading whitespaces*/
    while (*macroPos != '\0' && strchr(delimiters, *macroPos) != NULL) {
        macroPos++;
    }

    /* Find the end of the word after "macro"*/
    wordEnd = macroPos;
    while (*wordEnd != '\0' && !isspace(*wordEnd)) {
        wordEnd++;
    }

    /* Allocate memory for the word after "macro"*/
    wordLength = wordEnd - macroPos;
    word = (char*)malloc((wordLength + 1) * sizeof(char));
    if (word == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        exit(1);
    }

    /* Copy the word after "macro" to the new memory*/
    strncpy(word, macroPos, wordLength);
    word[wordLength] = '\0'; /* Null-terminate the string*/

    return word;
}

void addMacro(char* line, int firstMacro, macroList *mList){
    char *nn;
	macro* newM= (macro*)malloc(sizeof(macro)); 
    if(firstMacro){
        firstMacro = 0;
        mList -> start = newM;  
        mList -> end = newM;    
    }
    else{
        mList -> end -> next = newM;
        mList -> end = newM;
    }

    /*we didnt yet stasrted to input data*/
    newM -> dataFirst = NULL;
    newM ->dataLast = NULL;


    nn = find_word_after_mcr(line);
    newM -> name = nn;
    newM -> next = NULL;
}

void addMacroContent(char* line, macroList* mList) {
    int wantedSize = 1; 
    char* newContent;
    macro* last = mList->end;

    if (last->content != NULL) {
        wantedSize += strlen(last->content);
    }
    wantedSize += strlen(line);

    newContent = (char*)malloc(wantedSize);
    if (newContent == NULL) {
        printf("Memory allocation failed.\n");
        return;
    }

    if (last->content != NULL) {
        strcpy(newContent, last->content);
        strcat(newContent, line);
    } else {
        strcpy(newContent, line);
    }

    free(last->content);  
    last->content = newContent; 
    
}

void addMacroData(char* line, int firstData, macroList *mList){
    macroData* newData = (macroData*)malloc(sizeof(macroData));
    newData -> data = (char*)malloc(82);
    strcpy(newData -> data, line);/*enters the line as data*/

    if(firstData){
        mList -> end -> dataFirst = newData;
        mList -> end -> dataLast = newData;
    }
    else{
        mList -> end -> dataLast -> next = newData;
        mList -> end -> dataLast = newData;

    }
    newData-> next = NULL;
}
macro* getMacroByName(const char* name, const macroList* mList) {
    macro* current = mList->start;

    /* Iterate through the macro list */
    while (current != NULL) {
        /* Compare the names */
        if (strcmp(name, current->name) == 0) {
            /* Macro with the same name found, return its pointer */
            return current;
        }
        current = current->next;
    }
    /* Macro with the given name not found, return NULL */
    return NULL;
}
int isMacroInTable(char* name, macroList* mList){

    macro* current;
	current = mList->start;

    name[strcspn(name, "\r\n")] = 0;
    name[strcspn(name, "\n")] = 0;


    /* Iterate through the macro list */
    while (current != NULL) {
        /* Compare the names */

        if (strcmp(name, current->name) == 0) {
            /* Macro with the same name found */
            return 1;
        }

        current = current->next;
    }
    /* Macro with the given name not found */
    return 0;
}




