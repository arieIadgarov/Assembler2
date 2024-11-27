
#include "errors.h"

Error errors[] = {
    {ERROR_CODE_0, "No Error"},
    {ERROR_CODE_1, "symbole allready in table => symbol is defined twice"},
    {ERROR_CODE_2, "The provided file name is not exist"},
    {ERROR_CODE_3, ".am file is missing"},

    {ERROR_CODE_4, "dosent start with a legal word"},
    {ERROR_CODE_5, "ilegal number"},

    {ERROR_CODE_6, "Line is too long"},

    {ERROR_CODE_7, "Illegal opcode"},
    {ERROR_CODE_8, "Line contains extra text"},
    {ERROR_CODE_9, "Illegal argument"},

    {ERROR_CODE_10, "Comma in the wrong place"},

    {ERROR_CODE_11, "need a space"},
    {ERROR_CODE_12, "Illegal data line directive"},
    {ERROR_CODE_13, "Instruction '.data' line contains illegal chars or syntax error"}

};

/* Print the error code number and the error message */
void printError(int errorCode)
{
    printf("~~ERROR: ID:%d~~ : %s\n", errorCode, errors[errorCode].error_msg);
}

void errorThrowOut(int errorCode)
{
    if (errorCode)
    {
        printError(errorCode);
        exit(EXIT_FAILURE);
    }
}
/*eror handeling*/
/*in the pre assmbler we dont need to check for erorr as requsted*/

/*erors in the first pass*/
int errorHandlingLine(char *line, int *error_code)
{

    if (strlen(line) > MAX_LINE_LENGTH)
    {
        *error_code = ERROR_CODE_6;
        return ERROR_CODE_6;
    }
    return 0;
}

int errorHandlingDefineLine(const char *line, int *error_code)
{
    const char *ptr;
    int constant_length;
    int number_length;
    /* Check if line starts with ".define"*/
    if (strncmp(line, ".define", 7) != 0)
    {
        *error_code = ERROR_CODE_4;
        return ERROR_CODE_4;
    }

    /* Move pointer to the character after ".define"*/
    ptr = line + 7;

    /* Skip whitespaces*/
    while (*ptr != '\0' && isspace(*ptr))
        ptr++;

    /* Check if the next character is a letter*/
    if (!isalpha(*ptr))
    {
        *error_code = ERROR_CODE_9;
        return ERROR_CODE_9;
    }

    /* Skip the constant (up to 31 characters)*/
    constant_length = 0;
    while (isalnum(*ptr) && constant_length < 31)
    {
        ptr++;
        constant_length++;
    }

    /* Skip whitespaces*/
    while (*ptr != '\0' && isspace(*ptr))
        ptr++;

    /* Check if there is an '=' sign*/
    if (*ptr != '=')
    {
        *error_code = ERROR_CODE_9;
        return ERROR_CODE_9;
    }

    ptr++;

    /* Skip whitespaces*/
    while (*ptr != '\0' && isspace(*ptr))
        ptr++;

    /* Check if the next character is a digit or a sign (+/-)*/
    if (!isdigit(*ptr) && *ptr != '-' && *ptr != '+')
    {
        *error_code = ERROR_CODE_5;
        return ERROR_CODE_5;
    }

    /* Skip the number (up to 6 digits)*/
    number_length = 0;
    if (*ptr == '-' || *ptr == '+')
        ptr++;

    while (isdigit(*ptr) && number_length < 6)
    {
        ptr++;
        number_length++;
    }

    /* Check if the rest of the line is whitespace or empty*/
    while (*ptr != '\0')
    {
        if (!isspace(*ptr))
        {
            *error_code = ERROR_CODE_8;
            return ERROR_CODE_8;
        }
        ptr++;
    }
    return 0;
}

int IsADeclarcdeOpCode(char *word)
{
    int i;

    /* Return -1 if the string is NULL */
    if (word == NULL)
    {
        return -1;
    }
    /* Iterate through the list of known opcodes and compare the string with each opcode in the list */
    for (i = 0; i < 16; i++)
    {
        if (memcmp(word, OPNumbersOfCode[i], 3) == 0)
        {
            return i; /* Return the index of the matching opcode */
        }
    }
    return -1; /* Return -1 if the string does not match any known opcodes */
}

int IsRegistorLabel(char *line)
{
    int i;

    /* Return -1 if the string is NULL */
    if (line == NULL)
    {
        return -1;
    }
    for (i = 0; i < 8; i++)
    {
        if (memcmp(line, REGS[i], 2) == 0)
        {
            return i; /* Return the index of the matching opcode */
        }
    }
    return -1;
}

int legal_label(char *line, int *error_code)
{
    int i = 0;
    int amontOfSpaces;
    if (line == NULL)
    {
        return 0;
    }

    /*skiping the spaces*/
    while (isspace(line[i]))
    {
        i++;
    }
    amontOfSpaces = i;

    /*checking if there is a ':'*/
    if (strchr(line, ':') != NULL)
    {
        if (isalpha(line[i]))
        {
            i++;
            while (line[i] != ':' && (isalpha(line[i]) || isdigit(line[i])))
            {
                i++;
            }
            /*if you exited because there was a : its corect*/

            if (line[i] == ':')
            {
                if ((i - amontOfSpaces < MAX_LABEL_LENGTH) && (LabelSavedWord(line, error_code) == 1))
                {
                    return 1;
                }
                return 0;
            }
            /*exited by anything else*/
            else
            {
                return 0;
            }
        }
        return 0;
    }
    return 0;
}

int LabelSavedWord(char *line, int *error_code)
{
    char *ptrToLabel = getLabel(line);

    if ((IsRegistorLabel(ptrToLabel) < 0) && (IsADeclarcdeOpCode(ptrToLabel) < 0))
    {
        return 1;
    }

    return 0;
}

int dataLineErorEndeling(char *line, int *error_code, symbolList *mList)
{
    /*check if the label is legel*/
    int i, j;
    const char *data_pos;
    size_t label_end_index;
    size_t data_end_index;
    char *val;
    char *ptrForstrstr;
    int number_length;
    int index = 0;
    
    val = (char*)malloc(sizeof(char)*MAX_LABEL_LENGTH);

    if (strchr(line, ':') != NULL)
    {
        if (legal_label(line, error_code) == 0)
        {
            *error_code = ERROR_CODE_7;
            return ERROR_CODE_7;
        }
        ptrForstrstr = strstr(line, ":");
        label_end_index = ptrForstrstr - line;
        label_end_index++;

        if (line[label_end_index] != ' ')
        {
            /*needs to be a spaces after a label*/
            printf("%s ", line);
            *error_code = ERROR_CODE_11;
            return ERROR_CODE_11;
        }
    }
    else
    {
        if (line[0] != '.')
        {
            printf("%s ", line);
            *error_code = ERROR_CODE_13;
            return ERROR_CODE_13;
        }
    }

    data_pos = strstr(line, ".data");

    if (data_pos != NULL)
    {
        data_end_index = data_pos - line + strlen(".data");
    }
    else
    {
        /*no data word in a data sentance*/
        printf("%s ", line);
        *error_code = ERROR_CODE_4;
        return ERROR_CODE_4;
    }

    if (line[data_end_index] != ' ')
    {
        /*needs to be a space after .data*/
        printf("%s ", line);
        *error_code = ERROR_CODE_11;
        return ERROR_CODE_11;
    }
    i = data_end_index+1;
    j = i;
   /*giong over numbers part*/
    do{
        if(isdigit(line[j]) || line[j] == '-' || line[j] == '+'){
                
            if (!isdigit(line[j]) && line[j] != '-' && line[j] != '+')
            {
                *error_code = ERROR_CODE_5;
                return ERROR_CODE_5;
            }
            
            number_length = 0;
            if (line[j] != '-' || line[j] != '+')
                j++;

            while (isdigit(line[j]) && number_length < MAX_NUMBER_LENGTH &&(line[j] != '\0' && line[j] != '\r' && line[j] != '\n'))
            {
                j++;
                number_length++;

            }
            /*skip spaces*/
             while (isspace(line[j]) && (line[j] != '\0' && line[j] != '\r' && line[j] != '\n'))
            {
                j++;
            }
        
            if ((line[j] != ',') && (line[j] != '\0' && line[j] != '\r' && line[j] != '\n'))
            {
                printf("%s ", line);
                *error_code = ERROR_CODE_9;
                return ERROR_CODE_9;
            }
            j++;
            /*skip spaces*/
            while (isspace(line[j]) && (line[j] != '\0' && line[j] != '\r' && line[j] != '\n'))
            {
                j++;
            }
            continue;
        }
        if(isalpha(line[j])){
            val[index] = line[j];
            j++;
            while(isalpha(line[j]) || (isdigit(line[j]))){
                val[index] = line[j];
                index++;
                j++;
            }
            if(LabelSavedWord(val, error_code)&& strlen(val) >= MAX_LABEL_LENGTH){
                printf("%s ", line);
                *error_code = ERROR_CODE_9;
                return ERROR_CODE_9;
            }
             while (isspace(line[j]) && (line[j] != '\0' && line[j] != '\r' && line[j] != '\n'))
            {
                j++;
            }
            if ((line[j] != ',') && (line[j] != '\0' && line[j] != '\r' && line[j] != '\n'))
            {
                printf("%s ", line);
                *error_code = ERROR_CODE_9;
                return ERROR_CODE_9;
            }
            j++;
            while (isspace(line[j]) && (line[j] != '\0' && line[j] != '\r' && line[j] != '\n'))
            {
                j++;
            }
            continue;
        }
        else{
            printf("%s ", line);
            *error_code = ERROR_CODE_9;
            return ERROR_CODE_9;
        }
    }while ((line[j] != '\0' && line[j] != '\r' && line[j] != '\n'));
    
    free(val);
    return 0;
}


int stringLineErorEndeling(char *line, int *error_code)
{
    /*check if the label is legel*/
    int i = 0;
    size_t label_pos;
    char *ptrToSigh;
    size_t label_end_index;
    const char *string_pos;
    size_t string_position;
    size_t string_end_index;

    if (strchr(line, ':') != NULL)
    { /*if the first word*/
        if (legal_label(line, error_code) == 0)
        {
            *error_code = ERROR_CODE_7;
            return ERROR_CODE_7;
        }
        ptrToSigh = strchr(line, ':');
        /*get position of after the label*/
        if (strchr(line, ':') != NULL)
        {
            label_pos = (int)(ptrToSigh - line);
        }
        else
        {
            *error_code = ERROR_CODE_7;
            return ERROR_CODE_7;
        }

        if (line[label_pos + 1] != ' ')
        {

            printf("label wasnt leagel2 - %s\n", line);
            *error_code = ERROR_CODE_7;
            return ERROR_CODE_7;
        }

        label_pos++;
        /* Calculate the index of the character after the label*/
        label_end_index = label_pos; /*add 2 for the : and space*/

        i = label_end_index; /*setting i as the end of label*/
    }

    /*skips all spaces*/
    for (; isspace(line[i]); i++)
    {
        continue;
    }

    /*find position of .string*/
    string_pos = strstr(line, ".string");

    if(string_pos == 0){
        *error_code = ERROR_CODE_7;
        return ERROR_CODE_7;
    }
    string_position = (int)(string_pos - line);
   
    if (string_pos != NULL)
    {
        /* Calculate the index of the character after ".string"*/
        string_end_index = string_pos - line + strlen(".string");

        /* checking if somthig in the way after the label*/
        if (string_end_index != i + strlen(".string"))
        {
            *error_code = ERROR_CODE_9;
            return ERROR_CODE_9;
        }

        /* Check if there's a space after ".string"*/
        if (line[string_end_index] != ' ')
        {
            *error_code = ERROR_CODE_9;
            return ERROR_CODE_9;
        }

        i = i + strlen(".string");

        i++;
        while (isspace(line[i]))
        {
            i++;
        }

        /*needs to be an "*/
        if (line[i] != '"')
        {
            *error_code = ERROR_CODE_9;
            return ERROR_CODE_9;
        }
        i++;
        /*all that is included under leagel string*/
        while (line[i] != '\0' && line[i] != '"')
        {
            i++;
        }
        if (line[i] != '"')
        {
            *error_code = ERROR_CODE_9;
            return ERROR_CODE_9;
        }
        /*skipping spaces*/
        i++;
        while (isspace(line[i]))
        {
            i++;
        }
        if (line[i] != '\n' && line[i] != '\0')
        {
            *error_code = ERROR_CODE_9;
            return ERROR_CODE_9;
        }
        return 0;
    }
    return 0;
}

int entryErrorHandling(char *line, int *error_code)
{
    const char *label;
    int i;
    /* Check if the line starts with ".entry "*/
    if (strncmp(line, ".entry ", 7) != 0)
    {
        *error_code = ERROR_CODE_4;
        return ERROR_CODE_4;
    }

    /* Skip the ".entry " part*/
    label = line + 7;

    /* Check if the label starts with a letter*/
    if (!isalpha(label[0]))
    {
        *error_code = ERROR_CODE_9;
        return ERROR_CODE_9;
    }

    /* Check if the label contains only alphanumeric characters*/
    for (i = 1; label[i] != '\0'&& label[i] != '\r' && label[i] != '\n'; i++)
    {
        if (!isalnum(label[i]) || i >= 32)
        {
            *error_code = ERROR_CODE_9;
            return ERROR_CODE_9;
        }
    }

    return 0;
}

int externErrorHandling(char *line, int *error_code)
{
    const char *label;
    int i;

    /* Check if the line starts with".extern "*/
    if (strncmp(line, ".extern ", 8) != 0)
    {
        *error_code = ERROR_CODE_4;
        return ERROR_CODE_4;
    }

    /* Skip the ".extern " part*/
    label = line + 8;

    /* Check if the label starts with a letter*/
    if (!isalpha(label[0]))
    {
        *error_code = ERROR_CODE_9;
        return ERROR_CODE_9;
    }

    /* Check if the label contains only alphanumeric characters*/
    for (i = 1; label[i] != '\0'  && label[i] != '\r' && label[i] != '\n'; i++)
    {
        if (!isalnum(label[i]) || i >= 32)
        {
            *error_code = ERROR_CODE_9;
            return ERROR_CODE_9;
        }
    }
    return 0;
}

int OpCodeErrorHandeling(char *line, int *error_code)
{
    /*check if the label is legel*/
    int i = 0, j = 0;
    int numberOfOperands;
    char *label_pos;
    int count = 0; /*to check what opehand is that*/
    int opCodeLen = 3;
    char opCodeWord[3];

    size_t label_end_index;
    int wascoma = 0;

    char registerCheck[2] = "  ";

    if (strchr(line, ':') != NULL)
    { /*if the first word*/
        if (legal_label(line, error_code) == 0)
        {
            printf("label wasnt leagel33 - %s\n", line);
            *error_code = ERROR_CODE_7;
            return ERROR_CODE_7;
        }
        /*get position of after the label*/
        if (strchr(line, ':') != NULL)
        {
            label_pos = strchr(line, ':');
        }
        else
        {
            printf("label wasnt leagel2 - %s\n", line);
            *error_code = ERROR_CODE_7;
            return ERROR_CODE_7;
        }

        label_pos++;
        /* Calculate the index of the character after the label*/
        label_end_index = label_pos - line;

        i = label_end_index; /*setting i as the end of label*/
    }

    while (isspace(line[i]))
    {
        i++;
    }
    /*now we should arrive at the opcode*/
    while (isalpha(line[i]))
    {
        if (count < opCodeLen)
        {
            opCodeWord[count] = line[i];
            count++;
            i++;
        }
        else
        {
            printf("opcode too long");
            *error_code = ERROR_CODE_7;
            return ERROR_CODE_7;
        }
    }

    if (count != opCodeLen)
    {
        printf("opcode too short");
        *error_code = ERROR_CODE_7;
        return ERROR_CODE_7;
    }

    /*does not a leagel operhand*/
    if (IsADeclarcdeOpCode(opCodeWord) < 0)
    {
        *error_code = ERROR_CODE_7;
        return ERROR_CODE_7;
    }

    /*a must space*/
    if (line[i] != ' ' && line[i] != '\0' && line[i] != '\n')
    {
        printf("1");
        *error_code = ERROR_CODE_9;
        return ERROR_CODE_9;
    }
    i++;

    /*skips all of them*/

    while (isspace(line[i]))
    {
        i++;
    }

    numberOfOperands = findNumberOfOperands(opCodeWord);
    switch (numberOfOperands)
    {
    case 0:
    {
        while (isspace(line[i]))
        {
            i++;
        }
        if (line[i] != '\0')
        {
            *error_code = ERROR_CODE_9;
            return ERROR_CODE_9;
        }
        else
        {
            return 0;
        }
        break;
    }
    case 1:
    {

        /*for operhand*/
        while (isspace(line[i]))
        {
            i++;
        }

        /*syntax operhand*/
        if ((!isalpha(line[j])) && !isdigit(line[j]) && (line[j] != '#'))
        {
            printf("ilegel operhand7\n");
            *error_code = ERROR_CODE_9;
            return ERROR_CODE_9;
        }
        /*now we can extract the word and check*/
        j = i;
        /*for the first type*/
        if ((line[j] == '#'))
        {
            j++;

            if ((line[j] != '+' && line[j] != '-' && !isdigit(line[j])) && (!isalpha(line[j])))
            {
                printf("ilegel operhand1");
                *error_code = ERROR_CODE_9;
                return ERROR_CODE_9;
            }
            j++;

            if ((!isdigit(line[j])) && (!isalpha(line[j])))
            {
                if (line[j] != '\n' && line[j] != '\0' && line[j] != '\r')
                {
                    printf("ilegel operhand1");
                    *error_code = ERROR_CODE_9;
                    return ERROR_CODE_9;
                }
            }
            j++;
            /*wating for the operhand to end*/
            while (line[j] != ' ' && line[j] != '\n' && line[j] != '\0' && line[j] != '\r')
            {
                if ((!isdigit(line[j])) && (!isalpha(line[j])))
                {
                    printf("ilegel operhand2");
                    *error_code = ERROR_CODE_9;
                    return ERROR_CODE_9;
                }
                j++;
            }
        }
        /*for the second type*/
        /*and */
        /*for the third type*/
        if (isalpha(line[j]))
        {
            j = i;
            j++;
            /*have to be a char or digit*/
            while (isalpha(line[j]) || isdigit(line[j]))
            {
                j++;
            }
            /*why it stoped being a char or a digit*/
            if (line[j] != '[' && line[j] != ' ' && line[j] != '\n' && line[j] != '\0' && line[j] != '\r')
            {
                printf("ilegel operhand4 - %s\n", line);
                *error_code = ERROR_CODE_9;
                return ERROR_CODE_9;
            }

            /*the next thing is [number or constant]*/
            if (line[j] == '[')
            {
                j++;
                while (isalpha(line[j]) || isdigit(line[j]))
                {
                    j++;
                }
                /*why it wtoped being digit or letter*/
                if (line[j] != ']')
                {
                    printf("ilegel operhand5");
                    *error_code = ERROR_CODE_9;
                    return ERROR_CODE_9;
                }

                j++;
                /*check after [] to see if legal*/
                if (line[j] != ' ' && line[j] != '\n' && line[j] != '\0' && line[j] != '\r')
                {
                    printf("ilegel operhand5");
                    *error_code = ERROR_CODE_9;
                    return ERROR_CODE_9;
                }
            }
        }

        /*fourth option register*/
        if (line[j] == 'r' && isdigit(line[j + 1]) && ((line[j + 2] == ' ') || (line[j] != '\n' && line[j] != '\0' && line[j] != '\r')))
        {
            j = i;
            registerCheck[0] = line[j];
            registerCheck[1] = line[j + 1];

            if (IsRegistorLabel(registerCheck) <= 0)
            {
                printf("ilegel operhand6\n");
                *error_code = ERROR_CODE_9;
                return ERROR_CODE_9;
            }
            j = j + 2;
        }

        /*when the spaces end and the , appers*/
        j++;
        while (isspace(line[j]))
        {
            j++;
        }
        /*if its not , is not correct*/
        if (line[j] != '\n' && line[j] != '\0' && line[j] != '\r')
        {
            printf("ilegel operhand5 - %s", line);
            *error_code = ERROR_CODE_9;
            return ERROR_CODE_9;
        }

        return 0;
        break;
    }
    case 2:
    {
        /*for first operhand*/
        while (isspace(line[i]))
        {
            i++;
        }
        /*syntax operhand*/
        if ((!isalpha(line[i])) && !isdigit(line[i]) && (line[i] != '#'))
        {
            printf("not alpha not digit not # - %s\n", line);
            *error_code = ERROR_CODE_9;
            return ERROR_CODE_9;
        }
        /*now we can extract the word and check*/
        j = i;
        /*for the first type*/
        if ((line[j] == '#'))
        {
            j++;

            if ((line[j] != '+' && line[j] != '-' && !isdigit(line[j])) && (!isalpha(line[j])))
            {
                printf("ilegel operhand1");
                *error_code = ERROR_CODE_9;
                return ERROR_CODE_9;
            }
            j++;
            if ((!isdigit(line[j])) && (!isalpha(line[j])))
            {
                if (line[j] != ' ' && line[j] != ',')
                {
                    printf("ilegel operhand1");
                    *error_code = ERROR_CODE_9;
                    return ERROR_CODE_9;
                }
            }
            if (line[j] == ',')
            {
                wascoma = 1;
            }
            j++;
            /*wating for the operhand to end*/
            while (line[j] != ' ' && line[j] != ',')
            {
                if ((!isdigit(line[j])) && (!isalpha(line[j])))
                {
                    printf("ilegel operhand2");
                    *error_code = ERROR_CODE_9;
                    return ERROR_CODE_9;
                }
                j++;
            }
        }
        /*for the second type*/
        /*and */
        /*for the third type*/
        if (isalpha(line[j]))
        {
            j = i;
            j++;
            /*have to be a char or digit*/
            while (isalpha(line[j]) || isdigit(line[j]))
            {
                j++;
            }
            /*why it stoped being a char or a digit*/
            if (line[j] != '[' && line[j] != ' ' && line[j] != ',')
            {
                printf("ilegel operhand42 - %s\n", line);
                *error_code = ERROR_CODE_9;
                return ERROR_CODE_9;
            }

            /*the next thing is [number or constant]*/
            if (line[j] == '[')
            {
                j++;
                while (isalpha(line[j]) || isdigit(line[j]))
                {
                    j++;
                }
                /*why it wtoped being digit or letter*/
                if (line[j] != ']')
                {
                    printf("ilegel operhand5");
                    *error_code = ERROR_CODE_9;
                    return ERROR_CODE_9;
                }

                j++;
                /*check after [] to see if legal*/
                if (line[j] != ' ' && line[j] != ',')
                {
                    printf("ilegel operhand5");
                    *error_code = ERROR_CODE_9;
                    return ERROR_CODE_9;
                }
            }
        }

        /*fourth option register*/

        if (line[j] == 'r' && isdigit(line[j + 1]) && ((line[j + 2] == ' ') || (line[j + 2] == ',')))
        {
            j = i;
            registerCheck[0] = line[j];
            registerCheck[1] = line[j + 1];

            if (IsRegistorLabel(registerCheck) <= 0)
            {
                printf("ilegel operhand6\n");
                *error_code = ERROR_CODE_9;
                return ERROR_CODE_9;
            }
            j = j + 2;
            if (line[j] == ',')
            {
                wascoma = 1;
            }
        }

        /*when the spaces end and the , appers*/
        while (isspace(line[j]))
        {
            j++;
        }

        if (line[j] != ',' && wascoma != 1)
        {
            printf("no comma6 - %s\n", line);
            *error_code = ERROR_CODE_9;
            return ERROR_CODE_9;
        }
        j++;

        /*for secound operhand*/
        while (isspace(line[j]))
        {
            j++;
        }

        /*syntax operhand*/
        if ((!isalpha(line[j])) && !isdigit(line[j]) && (line[j] != '#'))
        {
            printf("ilegel operhand7\n");
            *error_code = ERROR_CODE_9;
            return ERROR_CODE_9;
        }
        /*now we can extract the word and check*/

        /*for the first type*/
        if ((line[j] == '#'))
        {
            j++;
            if ((line[j] != '+' && line[j] != '-' && !isdigit(line[j])) && (!isalpha(line[j])))
            {
                printf("ilegel operhand1");
                *error_code = ERROR_CODE_9;
                return ERROR_CODE_9;
            }
            j++;
            if ((!isdigit(line[j])) && (!isalpha(line[j])))
            {
                if (line[j] != '\n' && line[j] != '\0' && line[j] != '\r')
                {
                    printf("ilegel operhand1");
                    *error_code = ERROR_CODE_9;
                    return ERROR_CODE_9;
                }
            }
            j++;
            /*wating for the operhand to end*/
            while (line[j] != ' ' && line[j] != '\n' && line[j] != '\0' && line[j] != '\r')
            {
                if ((!isdigit(line[j])) && (!isalpha(line[j])))
                {
                    printf("ilegel operhand2");
                    *error_code = ERROR_CODE_9;
                    return ERROR_CODE_9;
                }
                j++;
            }
        }
        /*for the second type*/
        /*and */
        /*for the third type*/
        if (isalpha(line[j]))
        {
            j++;
            /*have to be a char or digit*/
            while (isalpha(line[j]) || isdigit(line[j]))
            {
                j++;
            }
            /*why it stoped being a char or a digit*/
            if (line[j] != '[' && line[j] != ' ' && line[j] != '\n' && line[j] != '\0' && line[j] != '\r')
            {
                printf("ilegel operhand43 - %s - %d<\n", line, line[j]);
                *error_code = ERROR_CODE_9;
                return ERROR_CODE_9;
            }

            /*the next thing is [number or constant]*/
            if (line[j] == '[')
            {
                j++;
                while (isalpha(line[j]) || isdigit(line[j]))
                {
                    j++;
                }
                /*why it wtoped being digit or letter*/
                if (line[j] != ']')
                {
                    printf("ilegel operhand5");
                    *error_code = ERROR_CODE_9;
                    return ERROR_CODE_9;
                }

                j++;
                /*check after [] to see if legal*/
                if (line[j] != ' ' && line[j] != '\n' && line[j] != '\0' && line[j] != '\r')
                {
                    printf("ilegel operhand5");
                    *error_code = ERROR_CODE_9;
                    return ERROR_CODE_9;
                }
            }
        }

        /*fourth option register*/
        if (line[j] == 'r' && isdigit(line[j + 1]) && ((line[j + 2] == ' ') || (line[j] != '\n' && line[j] != '\0' && line[j] != '\r')))
        {
            registerCheck[0] = line[j];
            registerCheck[1] = line[j + 1];

            if (IsRegistorLabel(registerCheck) <= 0)
            {
                printf("ilegel operhand6\n");
                *error_code = ERROR_CODE_9;
                return ERROR_CODE_9;
            }
            j++;
        }

        j++;
        /*when the spaces end and the , appers*/
        while (isspace(line[j]))
        {
            j++;
        }
        /*if its not , is not correct*/
        if (line[j] != '\n' && line[j] != '\0' && line[j] != '\r')
        {
            printf("ilegel operhand5");
            *error_code = ERROR_CODE_9;
            return ERROR_CODE_9;
        }
        return 0;

        break;
    }
    }
    return 0;
}