#include "assmbler.h"
#include "symbolTable.h"
#include "pre.h"
#include "secondPass.h"
#include "firstPass.h"

int main(int argc, char *argv[])
{
    char line[81];
    char lastLine[81];
    int i;
    int *IC = 0;
    int *DC = 0;
    int succes;
    char *file;
    symbolList *mList;
    macroList *mcList;

    FILE *filePointerext;
    FILE *filePointerent;

    char *preAsmblrOutputFileName;
    char *firstPassOutputFileName;
    char *secondPassOutputFileName;
    char *finnlly;

    symbol *current;
    FILE *ptrToOutputFile;
    FILE *ptrToInputFile;

    file = argv[1];
    preAsmblrOutputFileName = "pre_out.txt";
    firstPassOutputFileName = "first_out.txt";
    secondPassOutputFileName = "second_out.txt";

    mList = (symbolList *)calloc(sizeof(symbolList), 1); /*Initialized list*/
    if (mList == NULL)
    {
        printf("memory allocetion faild");
        return 1;
    }
    mList->start = NULL;
    mList->end = NULL;

    mcList = (macroList *)malloc(sizeof(macroList)); /*create macro list*/
    if (mcList == NULL)
    {
        printf("Error aloocetion file.\n");
        exit(1);
    }
    mcList->start = NULL;
    mcList->end = NULL;

    for (i = 1; i < argc; i++)
    {
        succes = pre_Assmbler(file, mcList, preAsmblrOutputFileName);
        if (succes == 0)
        {
            printf("preAssembler done successfully with file: %s\n", file);
        }
        else
        {
            printf("bad news :( %s\n", file);

        }

        succes = exe_first_pass(preAsmblrOutputFileName, mList, firstPassOutputFileName, &DC, &IC);
        if (succes == 0)
        {
            printf("firstPass done successfully with file: %s\n", file);
        }
        else
        {
            printf("bad news :( %s\n", file);
        }
        /*will overwirte the out file if firstpass*/
        succes = exe_second_pass(firstPassOutputFileName, mList, secondPassOutputFileName, &DC, &IC);
        if (succes == 0)
        {
            printf("second_pass executed successfully with file: %s\n", file);
        }
        else
        {
            printf("bad news :( %s\n", file);
        }


        /*REMOVING THE file first_out*/
        if (remove(firstPassOutputFileName) == 0)
        {
            printf("File %s deleted successfully.\n", firstPassOutputFileName);
        }
        else
        {
            printf("Error deleting file %s.\n", firstPassOutputFileName);
        }

        /*REMOVING THE file pre_out*/
        if (remove(preAsmblrOutputFileName) == 0)
        {
            printf("File %s deleted successfully.\n", preAsmblrOutputFileName);
        }
        else
        {
            printf("Error deleting file %s.\n", preAsmblrOutputFileName);
        }

        /*creating the output file*/
        ptrToOutputFile = fopen("assmbler.ob", "w");
        if (ptrToOutputFile == NULL)
        {
            printf("Unable to open the file.\n");
            return 1;
        }
        printf("created ob file\n");

        /*opening the last output*/
        ptrToInputFile = fopen(secondPassOutputFileName, "r");
        if (ptrToInputFile == NULL)
        {
            printf("Unable to open the file.\n");
            return 1;
        }

        /*putting every thing in the output file*/
        fprintf(ptrToOutputFile, "   %d   %d\n", DC, IC);
        while (fgets(line, sizeof(line), ptrToInputFile) != NULL)
        {
            fprintf(ptrToOutputFile, "%s", line);
        }
        
        fclose(ptrToOutputFile);
        fclose(ptrToInputFile);



        /*creating ext files*/
        if (isSymbolAlreadyInTableByprop(mList, "extern"))
        {
            /*opening extern file*/
            filePointerext = fopen("assmbler.ext", "w");

            if (filePointerext == NULL)
            {
                printf("Unable to open the file.\n");
                return 1;
            }

            current = mList->start;
            /* Iterate through the list */
            while (current != NULL)
            {
                /* Compare the name */
                if (strcmp(current->properties, "extern") == 0)
                {
                    fprintf(filePointerext, "0%d   %s\n", current->val, current->name);
                }
                current = current->next;
            }
            fclose(filePointerext);
        }
        /*creating ent files*/
        if (isSymbolAlreadyInTableByprop(mList, "entry"))
        {

            filePointerent = fopen("assmbler.ent", "w");

            if (filePointerent == NULL)
            {
                printf("Unable to open the file.\n");
                return 1;
            }
            current = mList->start;
            /* Iterate through the list */
            while (current != NULL)
            {
                /* Compare the name */
                if (strcmp(current->properties, "entry") == 0)
                {
                    fprintf(filePointerent, "0%d   %s\n", current->val, current->name);
                }
                current = current->next;
            }
            fclose(filePointerent);
        }
        return 0;
    }
    return 0;
}