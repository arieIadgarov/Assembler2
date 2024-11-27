
#include "symbolTable.h"
#include "errors.h"
#include "methodBank.h"


int exe_first_pass(char *file_name, symbolList *mList, char *firstPassOutputFileName, int *DC, int *IC){
    int error_code = ERROR_CODE_0;
    char line[MAX_LINE_LENGTH];

    int LC = 100;
    int numerOfOpCode;
    int numberOfOperands;

    char* base4;
    char* strStringLine;
    char* binaryCodeData;
    char* binaryCodeDataAfterPad;
    char* numiricNumber;
    int firstLabel = 1;

    int size;
    char **dataNumbers;/*for a data line*/

    int operhandTypeD;/*which operhand type first operhand*/
    int operhandTypeS;/*which operhand type second operhand*/

    char *ptrToSecoundWord;
    char *ptrForDestinationOperhand; /*ptr For First Operhand*/
    char *ptrForSourceOperhand;      /*ptr For second Operhand*/

    char binaryCode[15] = "00000000000000\0";
    
    int i;

/*opening files*/
    FILE *inputFile;
    FILE *ptrToutputFile;

    /* Open the file in write mode*/
    ptrToutputFile = fopen(firstPassOutputFileName, "w");
    /* Check if the file was opened successfully*/
    if (ptrToutputFile == NULL)
    {
        printf("Error opening file.\n");
        return 1; /* Return an error code*/
    }

    inputFile = fopen(file_name, "r");
    if (inputFile == NULL)
    {
        printf("Error: Unable to open file '%s'\n", file_name);
        return 1;
    }
    /*check if correct*/

   
/*end of file opening*/
    
    /*while starts*/
    while (fgets(line, sizeof(line), inputFile) != NULL)
    {
        /*checking for general error int the line*/
        errorThrowOut(errorHandlingLine(line, &error_code));

    /*if line in empty continue*/
        if (line[0] == '\n')
        { 
            continue;
        }

    /*if line is a note line skip*/
        if (line[0] == ';')
        {
            continue;
        }

/*start checking for types of lines*/
    /*if the line is define line*/
        if (strstr(line, ".define") != NULL)
        {
            (*DC)++;
            /*checking for all the posible errors in a .define line*/
            errorThrowOut(errorHandlingDefineLine(line, &error_code));

            /*checks if the label is the first in the table*/
            if (firstLabel)
            { 
                numiricNumber = findNumericConstant(line);
                /*adding a label to the table */
                addsymbol(findConstant(line), "mdefine", atoi(numiricNumber), firstLabel, mList);
                firstLabel = 0;

                free(numiricNumber);
                continue;
            } 
            numiricNumber = findNumericConstant(line);
            /*adding a label to the table */
            addsymbol(findConstant(line), "mdefine", atoi(numiricNumber), firstLabel, mList);

            free(numiricNumber);
            continue;
        }

    /*if the line is data line*/
        if (strstr(line, ".data") != NULL)
        {
            if (isLabel(line))
            {
                /*checks if the label is the first in the table*/
                if (firstLabel)
                {
                    /*adding a label to the table */
                    addsymbol(getLabel(line), "data", LC, firstLabel, mList); 
                    firstLabel = 0;
                }
                else{
                    /*adding a label to the table */
                    addsymbol(getLabel(line), "data", LC, firstLabel, mList); 
                }       
            }
            /*checking for all the posible errors in a .data line*/
            errorThrowOut(dataLineErorEndeling(line, &error_code, mList));

            /*returns an array of all the data numbers***/
            dataNumbers = get_data_elements(line);

            for(i = 0; dataNumbers[i] != NULL; i++) {
                
                /*ptr to the i element in the 2D array of chars*/
                (*DC)++;
                /*checks if not a number*/
                if((!isNumber(dataNumbers[i]))){
                    /*only define vals*/
                    /*indetifiy the label and put the val as binary*/
                    binaryCodeData = decimalToBinary2Complement(getValFromSymbol(mList,dataNumbers[i]));

                    /*make the binary 14 bits long*/
                    binaryCodeDataAfterPad = padBinary(binaryCodeData, 14);

                    /*add to binary*/
                    addBittsToBinary(binaryCodeDataAfterPad, 0, 13, binaryCode);
                    
                    /*put to file*/
                    /*putting binaryCode to file*/
                    base4 = binaryToBase4(binaryCode);
                    fprintf(ptrToutputFile, "%s\n", base4);
                    strcpy(binaryCode, "00000000000000\0");
                    LC++;

                    free(binaryCodeDataAfterPad);
                    free(binaryCodeData);
                }
                else{

                    /*indetifiy the label and put the val as binary*/
                    binaryCodeData = decimalToBinary2Complement(atoi(dataNumbers[i]));

                    /*make the binary 14 bits long*/
                    binaryCodeDataAfterPad = padBinary(binaryCodeData, 14);

                    /*add the whole number to binary code*/
                    addBittsToBinary(binaryCodeDataAfterPad, 0, 13, binaryCode);

                    /*put to file*/
                    base4 = binaryToBase4(binaryCode);
                    fprintf(ptrToutputFile, "%s\n", base4);
                    strcpy(binaryCode, "00000000000000\0");
                    LC++;

                    free(binaryCodeDataAfterPad);
                    free(binaryCodeData);
                }
            }
            /*free memory*/
            free(dataNumbers);
            continue;
        }

    /*if the line is string line*/
        if (strstr(line, ".string") != NULL)
        {
            if (isLabel(line))
            {
                /*checks if the label is the first in the table*/
                if (firstLabel)
                {
                    /*adding a label to the table */
                    addsymbol(getLabel(line), "data", LC, firstLabel, mList); 
                    firstLabel = 0;
                }
                else{
                    /*adding a label to the table */
                    addsymbol(getLabel(line), "data", LC, firstLabel, mList); 

                }       
            }
            /*checking for all the posible errors in a .string line*/
            errorThrowOut(stringLineErorEndeling(line, &error_code));

            /*extract the string*/
            strStringLine = extract_string(line);

            /*the size of the string*/
            size = strlen(strStringLine);

            for (i = 0; i < size; i++)
            {
                /*make the char asci then binery***/
                binaryCodeData = decimalToBinary2Complement(strStringLine[i]);
                
                /*make the binary 14 bits long*/
                binaryCodeDataAfterPad = padBinary(binaryCodeData, 14);
                
                /*adds to binary*/
                addBittsToBinary(binaryCodeDataAfterPad, 0, 13, binaryCode);

                /*copy the ascii to binary to the binaryCode***/
                base4 = binaryToBase4(binaryCode);
                fprintf(ptrToutputFile, "%s\n", base4);
                strcpy(binaryCode, "00000000000000\0");
                LC++;
                (*DC)++;
            }

            /*enter the \0 as 14 0's into the file*/
             base4 = binaryToBase4(binaryCode);
            fprintf(ptrToutputFile, "%s\n", base4);
            strcpy(binaryCode, "00000000000000\0");
            LC++;
            (*DC)++; /*for the '\0' char at the end of a string*/

            free(strStringLine);
            continue;

        }

    /*checking if extern line*/
        if (strstr(line, ".extern"))
        {
            (*IC)++;
            /*checking for all the posible errors in a .extern line*/
            errorThrowOut(externErrorHandling(line, &error_code));

            /*extracting the word after the word extern*/
            ptrToSecoundWord = extractWordAfterDirective(line, ".extern");
           
            /*checks if the label is the first in the table*/
            if (firstLabel)
            { 
                addsymbol(ptrToSecoundWord, "extern", LC, firstLabel, mList);
                firstLabel = 0;
                continue;
            }

           /*adding a label to the table */
            addsymbol(ptrToSecoundWord, "extern", LC, firstLabel, mList);
            continue;
        }

    /*checking if entry line*/
        if (strstr(line, ".entry"))
        {
            (*IC)++;
            /*checking for all the posible errors in a .extern line*/
            errorThrowOut(entryErrorHandling(line, &error_code));

            /*extracting the word after the word extern*/
            ptrToSecoundWord = extractWordAfterDirective(line, ".entry");
           
            /*checks if the label is the first in the table*/
            if (firstLabel)
            { 
                addsymbol(ptrToSecoundWord, "entry", LC, firstLabel, mList);
                firstLabel = 0;
                continue;
            }

           /*adding a label to the table */
            addsymbol(ptrToSecoundWord, "entry", LC, firstLabel, mList);
            continue;
        }

    /*now to the rest of the lines the instraction ones*/

        /*checking for all the posible errors in a instraction line*/
        errorThrowOut(OpCodeErrorHandeling(line, &error_code));

        /*checks the labels*/
        if (isLabel(line))
        {
            /*checks if the label is the first in the table*/
            if (firstLabel)
            {
                 /*adding a label to the table */
                addsymbol(getLabel(line), "code", LC, firstLabel, mList); 
                firstLabel = 0;
            }
            else{
                /*adding a label to the table */
                addsymbol(getLabel(line), "code", LC, firstLabel, mList); 

            }       
        }


/*we need to coulculate the number of line each line will take in order to fill correctly the symbol table*/
        /*the number of the op code*/
        numerOfOpCode = findInstructionInLine(line);

        /*how many operhand the op code use*/
        numberOfOperands = findNumberOfOperands(OPNumbersOfCode[numerOfOpCode]);

    /*now the number of words each line will take in the output file*/
        /*for the basic word no mattar how many operhand*/
        LC++; 
        (*IC)++;

        /*checking if we have more then one operhand*/
        if (numberOfOperands > 0)
        {
            /*the first operhand*/
            ptrForDestinationOperhand = extractOperandDestination(line);


            /*the type of the operhand 0 or 1 or 2 or 3*/
            operhandTypeD = findTypeOperhand(ptrForDestinationOperhand, mList);

            
            /*now we check for each case*/
            switch (operhandTypeD)
            {
                case 0: /*if the 0 type thier will be 1 word extra*/
                {
                    LC = LC + 1; 
                    (*IC)++;
                    break;
                }
                case 1: /*if the 1 type thier will be 1 word extra*/
                {
                    LC = LC + 1; 
                    (*IC)++;
                    break;
                }
                case 2: /*if the 2 type thier will be 2 word extra*/
                {
                    LC = LC + 2; 
                    (*IC)++;
                    (*IC)++;
                    break;
                } 
                case 3: /*if the 3 type thier will be 1 word extra*/
                {
                    LC = LC + 1; 
                    (*IC)++;
                    break;
                }
            }
                     
        }
        /*we finished checkin for the first operhand*/

            /*now we check id there is a secound operhand*/
            if (numberOfOperands == 2)
            {
                /*extract the second operhand*/
                ptrForSourceOperhand = extractSecondOperand(line);

                /*the type of the operhand 0 or 1 or 2 or 3*/
                operhandTypeS = findTypeOperhand(ptrForSourceOperhand, mList);
                /*checks what kind this opehand is*/
                switch (operhandTypeS)
                {
                    case 0: /* if the 0 type there will be an extra word*/
                    {
                        LC++;
                        (*IC)++;
                        break;
                    }

                    case 1: /* if the 0 type there will be an extra word*/
                    {
                        LC++;
                        (*IC)++;
                        break;
                    }
                    case 2:/* if the 2 type there will be 2 extra words*/
                    {
                        LC = LC + 2; 
                        (*IC)++;
                        (*IC)++;
                        break;
                    }
                    case 3: /* if the 3 type there will be an extra word if not entering the if*/
                    {
                        /*if the two operhand are 3 type there will be only 1 extra word so if true we add nothing*/
                        if(operhandTypeD == 3)
                        {
                            break; 
                        }
                        LC++; 
                        (*IC)++;
                        break;
                    }
                }
            }
            
            /*if there is no operhand we there is only one word and we will add it now*/
            if (numberOfOperands == 0)
            {
                /*the op code number*/
                numerOfOpCode = findInstructionInLine(line);

            /*putting the opcode in binary code*/
                /*now we make the number decimal*/
                binaryCodeData = decimalToBinary2Complement(numerOfOpCode);
                /*pad the binary so we enter the right amont of bits*/
                binaryCodeDataAfterPad = padBinary(binaryCodeData, 4);
                /*enter the pad binary to the binary code*/
                addBittsToBinary(binaryCodeDataAfterPad, 6,9,binaryCode);

                /*make this number binary and put it in 6 -9 and in the file***/
                base4 = binaryToBase4(binaryCode);
                fprintf(ptrToutputFile, "%s\n", base4);
                strcpy(binaryCode, "00000000000000\0");
                LC++;
                (*IC)++;
                continue;
            }
            /*put the line for the second pass*/
            fprintf(ptrToutputFile, "%s\n", line);
            LC++;
            (*IC)++;
    }
    /*to indecate of end file*/
    fclose(ptrToutputFile);
    fclose(inputFile);
    return 0;
}

