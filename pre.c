#include "pre.h"

#define MAX_LINE_LENGTH 82


int pre_Assmbler(char* inputFile, macroList* mList, char *preAsmblrOutputFileName){

    int insideMacro = 0;/*macro flag*/
    int isFirstData;/*macro contant during definition*/
    int isfirstMacro = 1;/*if this is the first macro*/

    macro* macroToPrint;

    char line[MAX_LINE_LENGTH];

    FILE *ptrToOutputFile;
    FILE *ptrToInputFile = fopen(inputFile, "r");
    if (ptrToInputFile == NULL) {    /*check if correct*/
        printf("Error: Unable to open file '%s'\n", inputFile);
        return 1;
    }

    /*out put file creation*/
    
    /* Open the file in write mode */
    ptrToOutputFile = fopen(preAsmblrOutputFileName, "w");
    /* Check if the file was opened successfully */
    if (ptrToOutputFile == NULL) {
        printf("Error opening file.\n");
        return 1; /* Return an error code */
    }

	

    while (fgets(line, sizeof(line), ptrToInputFile) != NULL) {
  

        if (strcmp(line, "\n") == 0) {/*if line in empty continue */
            continue;
        }
        if (strcmp(line, "\r\n") == 0) {/*if line in empty continue */
            continue;
        }
        if (line[0] == ';'){/*if line is a note*/
            continue;
        }



/******definition staff start*******/
        if ((strstr(line, "mcr") == line) ) {/*start definition of a macro*/
            addMacro(line, isfirstMacro, mList);
            isfirstMacro=0;
            isFirstData = 1;
			insideMacro = 1;
            continue; 
        }

        /*inside macro define */
        if(insideMacro){
            /* Check if line starts with the word "endmcr" */


            if ((strcmp(line, "endmcr\n") == 0) || (strcmp(line, "endmcr\r\n") == 0)) {
                insideMacro = 0;
                continue; 
            }
            addMacroContent(line, mList);
            isFirstData=0;
            continue;
        }
/******definition staff end*******/

/******indentify macro start*******/

        if(isMacroInTable(line, mList)){

            macroToPrint = getMacroByName(line,mList);

            fprintf(ptrToOutputFile,"%s\n",macroToPrint->content);
            continue;
           
		}
/******indentify macro end*******/

        /*if this is a reguler line*/
        fprintf(ptrToOutputFile,"%s\n",line);
    }
    
    fclose(ptrToOutputFile);
    fclose(ptrToInputFile);
    return 0;
}






