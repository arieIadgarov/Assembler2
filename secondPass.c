

#include "symbolTable.h"

#include "secondPass.h"
int exe_second_pass(char *inputFile, symbolList *mList, char *secondPassOutputFileName,int *DC,int *IC)
{

    int operhandTypeD; /*which operhand type first operhand*/
    int operhandTypeS; /*which operhand type second operhand*/

    char *ptrForDestinationOperhand; /*ptr For First Operhand*/
    char *ptrForSourceOperhand;      /*ptr For second Operhand*/

    int numberOfOperands;
    int numerOfOpCode;

    char* base4;

    char* padBin;
    char* padBin2;
    char *decimalConvernt;  /*for the decimal char to free it*/
    char *decimalConvernt2; /*if needed to convert 2 numbers*/
    char *WordAfterDirective;

    int numberOfRegisterOP; /*the number of register that the operhand represent*/

    char *symbolInType2; /*a ptr for the symbol brfore the []*/

    int LC = 100;

    char line[MAX_LINE_LENGTH];
    char binaryCode[15];

    FILE *ptrToOutputFile;
    FILE *ptrToInputFile;

    /*opening files*/
    ptrToOutputFile = fopen(secondPassOutputFileName, "w");
    /* Check if the file was opened successfully */
    if (ptrToOutputFile == NULL)
    {
        printf("Error opening file.\n");
        return 1; /* Return an error code */
    }

    /* Open the file in read mode */
    ptrToInputFile = fopen(inputFile, "r");
    /* Check if the file was opened successfully */
    if (ptrToInputFile == NULL)
    {
        printf("Error opening file.\n");
        return 1; /* Return an error code */
    }
    /*done openning files*/

    /*setting the binarycode to 0*/
    strcpy(binaryCode, "00000000000000\0");

    /*starting loop*/
    while (fgets(line, sizeof(line), ptrToInputFile) != NULL)
    {
        if (isalpha(line[0]))
        {
            /*now we enter the first word*/
            /*the op code number*/
            numerOfOpCode = findInstructionInLine(line);

        /*putting the opcode in binary code*/
            /*now we make the number decimal*/
            decimalConvernt = decimalToBinary2Complement(numerOfOpCode);
            /*pad the binary so we enter the right amont of bits*/
            padBin = padBinary(decimalConvernt, 4);
            /*enter the pad binary to the binary code*/
            addBittsToBinary(padBin, 6,9,binaryCode);

            /*how many operhand the op code use*/
            numberOfOperands = findNumberOfOperands(OPNumbersOfCode[numerOfOpCode]);

        /*can be atlist 1 operhand*/
            /*the operhand - the word*/
            ptrForDestinationOperhand = extractOperandDestination(line);
            /*the type*/
            operhandTypeD = findTypeOperhand(ptrForDestinationOperhand, mList);

            /*for each case the first operhand*/
            switch (operhandTypeD)
            {
            case 0:
            {
                /*add 00 to 2,3*/
                addBittsToBinary("00", 2, 3, binaryCode);
                break;
            }
            case 1:
            {
                /*add 01 to 2,3*/
                addBittsToBinary("01", 2, 3, binaryCode);
                break;
            }
            case 2:
            {
                /*add 10 to 2,3*/
                addBittsToBinary("10", 2, 3, binaryCode);
                break;
            }
            case 3:
            {
                /*add 11 to 2,3*/
                addBittsToBinary("11", 2, 3, binaryCode);
                break;
            }
            }

            /*of there are 2 operhands*/
            if (numberOfOperands == 2)
            {
                /*the operhand the word*/
                ptrForSourceOperhand = extractSecondOperand(line);
                /*the type*/
                operhandTypeS = findTypeOperhand(ptrForSourceOperhand, mList);

                /*for each case the first operhand*/
                switch (operhandTypeS)
                {
                case 0:
                {
                    /*add 00 to 4,5*/
                    addBittsToBinary("00", 4, 5, binaryCode);
                    break;
                }
                case 1:
                {
                    /*add 01 to 4,5*/
                    addBittsToBinary("01", 4, 5, binaryCode);
                    break;
                }
                case 2:
                {
                    /*add 10 to 4,5*/
                    addBittsToBinary("10", 4, 5, binaryCode);
                    break;
                }
                case 3:
                {
                    /*add 11 to 4,5*/
                    addBittsToBinary("11", 4, 5, binaryCode);
                    break;
                }
                }
            }
            /*put the basic word in file*/ /*turn binarycode to 4 spacil***/
            /*putting binaryCode to file*/
            base4 = binaryToBase4(binaryCode);
            fprintf(ptrToOutputFile, "0%d  %s\n", LC, base4);
            strcpy(binaryCode, "00000000000000\0");
            LC++;

            /*freeing the basic word use of memory*/
           
                    /*free(padBin);
                    free(decimalConvernt);*/

            /*now we done with the basic word*/
            /*the extra words*/
            switch (operhandTypeD)
            {
            case 0:
            {
                /*for the first type all bits 0,1 are 00*/
                addBittsToBinary("00", 0, 1, binaryCode);

                /*extract the sybol after the sigh #*/
                WordAfterDirective = extractWordAfterDirective(ptrForDestinationOperhand, "#");

                /*checking if the */
                if (isNumber(WordAfterDirective))
                {

                    /*turning the ptrForDestinationOperhand to a number and makes it binary*/
                    decimalConvernt = decimalToBinary2Complement(extract_number_after_hashtag(ptrForDestinationOperhand));

                    /*pad the binary so we enter the right amont of bits*/
                    padBin = padBinary(decimalConvernt, 11);

                    /*adding to the extra word the bits*/
                    addBittsToBinary(padBin, 2, 13, binaryCode);

                    /*freeing memory*/
                    
                    /*free(padBin);
                    free(decimalConvernt);*/
                }
                /*meaning is a symbol*/
                else
                {

                    /*make this sybol a binary code by finding the val of the symbol and converting it to binary */
                    decimalConvernt = decimalToBinary2Complement(getValFromSymbol(mList, WordAfterDirective));

                    /*pad the binary so we enter the right amont of bits*/
                    padBin = padBinary(decimalConvernt, 11);

                    /*adding to the extra word the bits*/
                    addBittsToBinary(padBin, 2, 13, binaryCode);

                    /*add word to file***/

                    /*free memory*/
                    /*free(WordAfterDirective);
                    
                    free(padBin);
                    free(decimalConvernt);*/
                }

                /*putting binaryCode to file*/
                base4 = binaryToBase4(binaryCode);
                fprintf(ptrToOutputFile, "0%d  %s\n", LC, base4);
                strcpy(binaryCode, "00000000000000\0");
                LC++;

                break;
            }
            case 1:
            {
                /*the first extra word*/
                /*checking if the symbol is in the table*/
                if (isSymbolAlreadyInTable(mList, ptrForDestinationOperhand))
                {
                    /*checks if the first 2 bits are 01 or 10*/
                    /*if the symbol is extern then 10 if otherwise then 01*/
                    if (isSymbolExtern(mList, ptrForDestinationOperhand))
                    {
                        /*extern symbol*/
                        addBittsToBinary("10", 0, 1, binaryCode);
                    }
                    else
                    {
                        /*otherwise*/
                        addBittsToBinary("01", 0, 1, binaryCode);
                    }

                    /*getting the val from symbol and making it binary*/
                    decimalConvernt = decimalToBinary2Complement(getValFromSymbol(mList, ptrForDestinationOperhand));

                    /*pad the binary so we enter the right amont of bits*/
                    padBin = padBinary(decimalConvernt, 11);

                    /*adding to the extra word the bits*/
                    addBittsToBinary(padBin, 2, 13, binaryCode);

                    /*add word to file***/

                    /*freeing memory*/
                    
                    /*free(padBin);
                    free(decimalConvernt);*/
                }
                else
                {
                    /*symbol is not defined*/
                    printf("symbol not defined\n");
                    exit(1);
                }

                /*putting binaryCode to file*/
                base4 = binaryToBase4(binaryCode);
                fprintf(ptrToOutputFile, "0%d  %s\n", LC, base4);
                strcpy(binaryCode, "00000000000000\0");
                LC++;

                break;
            }

            case 2:
            {
                /*getting the first word => str[5] getting str*/
                symbolInType2 = extract_word_before_bracket(ptrForDestinationOperhand);

                /*checking if word is symbol*/
                if (isSymbolAlreadyInTable(mList, symbolInType2))
                {

                    if (isSymbolExtern(mList, ptrForDestinationOperhand))
                    {
                        /*extern symbol*/
                        addBittsToBinary("10", 0, 1, binaryCode);
                    }
                    else
                    {
                        /*otherwise*/
                        addBittsToBinary("01", 0, 1, binaryCode);
                    }

                    /*getting the addres of str(first word) and making it binary*/
                    decimalConvernt = decimalToBinary2Complement(getValFromSymbol(mList, symbolInType2));

                    /*pad the binary so we enter the right amont of bits*/
                    padBin = padBinary(decimalConvernt, 11);

                    addBittsToBinary(padBin, 2, 13, binaryCode);

                    /*freeing decimalConvernt and padBin*/
                    
                    /*free(padBin);
                    free(decimalConvernt);*/
                }
                else
                {

                    /*symbol is not defined*/
                    printf("symbol not defined\n");
                    exit(1);
                }
                /*putting binaryCode to file*/
                base4 = binaryToBase4(binaryCode);
                fprintf(ptrToOutputFile, "0%d  %s\n", LC, base4);
                strcpy(binaryCode, "00000000000000\0");
                LC++;

                /*free(symbolInType2);*/

                /*the second extra word*/
                /*getting the first word => str[5] getting 5*/
                symbolInType2 = extractWordInSquareBrackets(ptrForDestinationOperhand);

                /* 0,1 bits are always 00 in this word*/
                addBittsToBinary("00", 0, 1, binaryCode);

                if (isNumber(symbolInType2))
                {

                    /*turning the symbolInType2 to a number with atoi and making it binary*/
                    decimalConvernt = decimalToBinary2Complement(atoi(symbolInType2));
                    
                    /*pad the binary so we enter the right amont of bits*/
                    padBin = padBinary(decimalConvernt, 11);

                    /*adding to the extra word the bits*/
                    addBittsToBinary(padBin, 2, 13, binaryCode);

                    /*freeing memory*/
                    
                    /*free(padBin);
                    free(decimalConvernt);*/
                }
                /*this is a symbol*/
                else
                {
                    /*make this sybol a binary code by finding the val of the symbol and converting it to binary */
                    decimalConvernt = decimalToBinary2Complement(getValFromSymbol(mList, symbolInType2));

                    /*pad the binary so we enter the right amont of bits*/
                    padBin = padBinary(decimalConvernt, 11);

                    /*adding to the extra word the bits*/
                    addBittsToBinary(padBin, 2, 13, binaryCode);

                    /*free memory*/
                    
                    /*free(padBin);
                    free(decimalConvernt);*/
                }

                /*putting binaryCode to file*/
                base4 = binaryToBase4(binaryCode);
                fprintf(ptrToOutputFile, "0%d  %s\n", LC, base4);
                strcpy(binaryCode, "00000000000000\0");
                LC++;

                /*free(symbolInType2);*/
                break;
            }
            case 3:
            {
                /*allways 00 in bits 0-1*/
                addBittsToBinary("00", 0, 1, binaryCode);
                /*checking if there are 2 operhands*/
                if (numberOfOperands == 2)
                {
                    /*if both operhands are type 3*/
                    if (operhandTypeS == 3)
                    {
                    /*the des operhand*/
                        /*findidng the number of register*/
                        numberOfRegisterOP = findRegister(ptrForDestinationOperhand);

                        /*making the number of register as binary*/
                        decimalConvernt = decimalToBinary2Complement(numberOfRegisterOP);

                        /*pad the binary so we enter the right amont of bits*/
                        padBin = padBinary(decimalConvernt, 3);

                        /*adds the first operhand to the word*/
                        addBittsToBinary(padBin, 2, 4, binaryCode);

                    /*now the source operhand */
                        /*findidng the number of register for source*/
                        numberOfRegisterOP = findRegister(ptrForSourceOperhand);

                        /*making the number of register as binary for source*/
                        decimalConvernt2 = decimalToBinary2Complement(numberOfRegisterOP);

                        /*pad the binary so we enter the right amont of bits*/
                        padBin2 = padBinary(decimalConvernt2, 3);

                        /*adds the second operhand to the word*/
                        addBittsToBinary(padBin2, 5, 7, binaryCode);

                        /*putting binaryCode to file*/
                        base4 = binaryToBase4(binaryCode);
                        fprintf(ptrToOutputFile, "0%d  %s\n", LC, base4);
                        strcpy(binaryCode, "00000000000000\0");
                        LC++;

                        /*freeing memory*/
                        
                    /*free(padBin2);
                    free(decimalConvernt2);*/
                        
                    /*free(padBin);
                    free(decimalConvernt);*/
                        break;
                    }
                }
            /*meaning ther is only the one operhand that type 3*/
                /*findidng the number of register*/
                numberOfRegisterOP = findRegister(ptrForDestinationOperhand);

                /*making the number of register as binary*/
                decimalConvernt = decimalToBinary2Complement(numberOfRegisterOP);

                /*pad the binary so we enter the right amont of bits*/
                padBin = padBinary(decimalConvernt, 3);

                /*adds the first operhand to the word*/
                addBittsToBinary(padBin, 2, 4, binaryCode);

                /*putting binaryCode to file*/
                base4 = binaryToBase4(binaryCode);
                fprintf(ptrToOutputFile, "0%d  %s\n", LC, base4);
                strcpy(binaryCode, "00000000000000\0");
                LC++;   

                /*free(decimalConvernt)*/
                
                    /*free(padBin);
                    free(decimalConvernt);*/
                
                break;
            }
            }
    /*if there is a secound operhand this will handle it*/
            if (numberOfOperands > 1)
            {
                switch (operhandTypeS)
                {
                case 0:
                {
                    /*for the first type all bits 0,1 are 00*/
                    addBittsToBinary("00", 0, 1, binaryCode);
                    /*extract the sybol after the sigh #*/
                    WordAfterDirective = extractWordAfterDirective(ptrForSourceOperhand, "#");

                    /*checking if the */
                    if (isNumber(WordAfterDirective))
                    {

                        /*turning the ptrForDestinationOperhand to a number and makes it binary*/
                        decimalConvernt = decimalToBinary2Complement(extract_number_after_hashtag(ptrForSourceOperhand));
                        
                        /*pad the binary so we enter the right amont of bits*/
                        padBin = padBinary(decimalConvernt, 11);

                        /*adding to the extra word the bits*/
                        addBittsToBinary(padBin, 2, 13, binaryCode);

                        /*freeing memory*/
                        
                    /*free(padBin);
                    free(decimalConvernt);*/
                    }
                    /*meaning is a symbol*/
                    else
                    {
                        /*make this sybol a binary code by finding the val of the symbol and converting it to binary */
                        decimalConvernt = decimalToBinary2Complement(getValFromSymbol(mList, WordAfterDirective));

                        /*pad the binary so we enter the right amont of bits*/
                        padBin = padBinary(decimalConvernt, 11);

                        /*adding to the extra word the bits*/
                        addBittsToBinary(padBin, 2, 13, binaryCode);

                        /*free memory*/

                    /*free(WordAfterDirective);
                        
                    free(padBin);
                    free(decimalConvernt);*/
                    }

                    /*putting binaryCode to file*/
                    base4 = binaryToBase4(binaryCode);
                    fprintf(ptrToOutputFile, "0%d  %s\n", LC, base4);
                    strcpy(binaryCode, "00000000000000\0");
                    LC++;

                    break;
                }
                case 1:
                {
                    /*the first extra word*/
                    /*checking if the symbol is in the table*/
                    if (isSymbolAlreadyInTable(mList, ptrForSourceOperhand))
                    {
                        /*checks if the first 2 bits are 01 or 10*/
                        /*if the symbol is extern then 10 if otherwise then 01*/
                        if (isSymbolExtern(mList, ptrForSourceOperhand))
                        {
                            /*extern symbol*/
                            addBittsToBinary("10", 0, 1, binaryCode);
                        }
                        else
                        {
                            /*otherwise*/
                            addBittsToBinary("01", 0, 1, binaryCode);
                        }

                        /*getting the val from symbol and making it binary*/
                        decimalConvernt = decimalToBinary2Complement(getValFromSymbol(mList, ptrForSourceOperhand));

                        /*pad the binary so we enter the right amont of bits*/
                        padBin = padBinary(decimalConvernt, 11);

                        /*adding to the extra word the bits*/
                        addBittsToBinary(padBin, 2, 13, binaryCode);

                        /*putting binaryCode to file*/
                        base4 = binaryToBase4(binaryCode);
                        fprintf(ptrToOutputFile, "0%d  %s\n", LC, base4);
                        strcpy(binaryCode, "00000000000000\0");
                        LC++;

                        /*freeing memory*/
                        
                    /*free(padBin);
                    free(decimalConvernt);*/
                    }
                    else
                    {
                        /*symbol is not defined*/
                        printf("symbol not defined\n");
                        exit(1);
                    }
                    break;
                }
                case 2:
                {
                    /*getting the first word => str[5] getting str*/
                    symbolInType2 = extract_word_before_bracket(ptrForSourceOperhand);
                    /*checking if word is symbol*/
                    if (isSymbolAlreadyInTable(mList, symbolInType2))
                    {

                        if (isSymbolExtern(mList, ptrForSourceOperhand))
                        {
                            /*extern symbol*/
                            addBittsToBinary("10", 0, 1, binaryCode);
                        }
                        else
                        {
                            /*otherwise*/
                            addBittsToBinary("01", 0, 1, binaryCode);
                        }

                        /*getting the addres of str(first word) and making it binary*/
                        decimalConvernt = decimalToBinary2Complement(getValFromSymbol(mList, symbolInType2));
                        
                        /*pad the binary so we enter the right amont of bits*/
                        padBin = padBinary(decimalConvernt, 11);

                        addBittsToBinary(padBin, 2, 13, binaryCode);

                        /*freeing decimalConvernt*/
                        
                    /*free(padBin);
                    free(decimalConvernt);*/
                    }
                    else
                    {
                        /*symbol is not defined*/
                        printf("symbol not defined\n");
                        exit(1);
                    }
                    /*putting binaryCode to file*/
                    base4 = binaryToBase4(binaryCode);
                    fprintf(ptrToOutputFile, "0%d  %s\n", LC, base4);
                    strcpy(binaryCode, "00000000000000\0");
                    LC++;

                    /*free(symbolInType2);*/

                    /*the second extra word*/
                    /*getting the first word => str[5] getting 5*/
                    symbolInType2 = extractWordInSquareBrackets(ptrForSourceOperhand);

                    /* 0,1 bits are always 00 in this word*/
                    addBittsToBinary("00", 0, 1, binaryCode);

                    if (isNumber(symbolInType2))
                    {
                        /*turning the symbolInType2 to a number with atoi and making it binary*/
                        decimalConvernt = decimalToBinary2Complement(atoi(symbolInType2));

                        /*pad the binary so we enter the right amont of bits*/
                        padBin = padBinary(decimalConvernt, 11);

                        /*adding to the extra word the bits*/
                        addBittsToBinary(padBin, 2, 13, binaryCode);

                        /*freeing memory*/
                        
                    /*free(padBin);
                    free(decimalConvernt);*/
                    }
                    /*this is a symbol*/
                    else
                    {
                        /*make this sybol a binary code by finding the val of the symbol and converting it to binary */
                        decimalConvernt = decimalToBinary2Complement(getValFromSymbol(mList, symbolInType2));

                        /*pad the binary so we enter the right amont of bits*/
                        padBin = padBinary(decimalConvernt, 11);

                        /*adding to the extra word the bits*/
                        addBittsToBinary(padBin, 2, 13, binaryCode);

                        /*add word to file***/

                        /*free memory*/
                        
                        /*free(padBin);
                        free(decimalConvernt);*/
                    }

                    /*putting binaryCode to file*/
                    base4 = binaryToBase4(binaryCode);
                    fprintf(ptrToOutputFile, "0%d  %s\n", LC, base4);
                    strcpy(binaryCode, "00000000000000\0");
                    LC++;

                    /*free(symbolInType2);*/
                    break;
                }
                case 3:
                {   
                    /*allways 00 in bits 0-1*/
                    addBittsToBinary("00", 0, 1, binaryCode);

                    /*checking if the 2 operhands are type 3 id yes its already taken take of in when we sort the first operhand*/
                    if(operhandTypeD == 3){
                        break;
                    }
                    /*findidng the number of register*/
                    numberOfRegisterOP = findRegister(ptrForSourceOperhand);

                    /*making the number of register as binary*/
                    decimalConvernt = decimalToBinary2Complement(numberOfRegisterOP);

                    /*pad the binary so we enter the right amont of bits*/
                    padBin = padBinary(decimalConvernt, 11);

                    /*adds the first operhand to the word*/
                    addBittsToBinary(padBin, 5, 7, binaryCode);

                    /*putting binaryCode to file*/
                    base4 = binaryToBase4(binaryCode);
                    fprintf(ptrToOutputFile, "0%d  %s\n", LC, base4);
                    strcpy(binaryCode, "00000000000000\0");
                    LC++;

                    /*free(padBin);
                    free(decimalConvernt);*/
                    break;
                }
                }
                
            }
            /*we handlde all the instrection line*/
            
            continue;
        }
        /*line that are were filled by the firstPass*/
        else if (line[0] == ' ' || line[0] == '\n' || line[0] == '\r' || line[0] == '\0')
        {
            continue;
        }
       
        /*a line that the first pass put there*/
        else
        {
            /*adding to that file the LC*/
            fprintf(ptrToOutputFile, "0%d  %s", LC, line);
            strcpy(binaryCode, "00000000000000\0");
            LC++;
        }

    }
    
    /*(free(padBin2);
    free(padBin);
    free(decimalConvernt2);
    free(decimalConvernt);*/

    fclose(ptrToInputFile);
    fclose(ptrToOutputFile);
    return 0;
}
