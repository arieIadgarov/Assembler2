
#include "methodBank.h"
#include "symbolTable.h"
#include "errors.h"

/* Example implementation of is_opcode for demonstration purposes */
bool is_opcode(const char *str)
{
    const char *OPNumbersOfCode[16] =
        {"mov", "cmp", "add", "sub", "not", "clr", "lea", "inc", "dec", "jmp", "bne", "red", "prn", "jsr", "rts", "hlt"};

    int i;
    for (i = 0; i < sizeof(OPNumbersOfCode) / sizeof(OPNumbersOfCode[0]); i++)
    {
        if (strcmp(str, OPNumbersOfCode[i]) == 0)
        {
            return true;
        }
    }

    return false;
}
char *extract_string(const char *line)
{
    const char *start_quote = NULL;
    const char *end_quote = NULL;
    size_t substring_length = 0;
    char *extracted_string = NULL;

    /* Find the position of the opening double quote */
    start_quote = strchr(line, '"');
    if (start_quote)
    {
        /* Find the position of the closing double quote */
        end_quote = start_quote + 1; /* Start searching from the character after the opening quote*/
        while (*end_quote != '"' && *end_quote != '\0')
        {
            ++end_quote;
        }
        if (*end_quote == '"')
        {
            /* Calculate the length of the substring excluding the double quotes */
            substring_length = end_quote - start_quote - 1;
            /* Allocate memory for the extracted substring */
            extracted_string = (char *)malloc(substring_length + 1); /* +1 for the null terminator */
            if (extracted_string)
            {
                /* Copy the substring excluding the double quotes */
                strncpy(extracted_string, start_quote + 1, substring_length);
                extracted_string[substring_length] = '\0'; /* Null-terminate the string */
            }
        }
    }
    return extracted_string;
}

char **get_data_elements(char *line)
{
    char **elements = NULL;
    char *token;
    int count = 0;
    char *end;

    /* Find the .data keyword*/
    token = strstr(line, ".data");
    if (token == NULL)
        return NULL;

    /* Move pointer to the position after .data*/
    token += strlen(".data");

    /* Dynamically allocate memory for elements array*/
    elements = (char **)malloc(100 * sizeof(char *));
    if (elements == NULL)
    {
        fprintf(stderr, "Memory allocation failed\n");
        exit(1);
    }

    /* Tokenize the string by commas*/
    token = strtok(token, ",");
    while (token != NULL)
    {
        /* Remove leading spaces*/
        while (isspace(*token))
            token++;

        /* Remove trailing spaces*/
        end = token + strlen(token) - 1;
        while (isspace(*end))
            end--;
        *(end + 1) = '\0';

        /* Store the token in the elements array*/
        elements[count] = token;
        count++;

        /* Move to the next token*/
        token = strtok(NULL, ",");
    }
    /* Set the last element as NULL to mark the end of the array*/
    elements[count] = NULL;

    return elements;
}

/* Function to convert binary string to base-4 string*/
char *binaryToBase4(const char *binary)
{
    static char base4[6]; /* maximum length of base-4 string is 6 characters for 15 binary digits*/
    int length = strlen(binary);
    int i;
    /* Mapping of binary digits to base-4 digits*/
    char base4_map[4] = {'*', '#', '%', '!'};

    /* Initialize base-4 string index*/
    int base4_index = 0;

    /* Iterate over binary string*/
    for (i = 0; i < length; i += 2)
    {
        /* Convert binary digits to decimal*/
        int decimal = (binary[i] - '0') * 2 + (binary[i + 1] - '0');

        /* Map decimal to base-4 digit and store in base-4 string*/
        base4[base4_index++] = base4_map[decimal];
    }

    /* Null-terminate the base-4 string*/
    base4[base4_index] = '\0';

    return base4;
}

int extract_number_after_hashtag(const char *word)
{
    const char *ptr = word;
    int sign = 1;
    int number = 0;

    /* Skip characters until '#' is found */
    while (*ptr && *ptr != '#')
        ptr++;

    /* If '#' is found, parse the number */
    if (*ptr == '#')
    {
        ptr++; /* Move past the '#' */

        /* Check for optional sign */
        if (*ptr == '-')
        {
            sign = -1;
            ptr++;
        }
        else if (*ptr == '+')
        {
            ptr++;
        }

        /* Parse digits */
        while (isdigit(*ptr))
        {
            number = number * 10 + (*ptr - '0');
            ptr++;
        }

        /* Apply sign */
        number *= sign;
    }

    return number;
}

char *padBinary(char *binaryStr, int desiredLength)
{
    int numZeros;
    char *paddedBinary;
    int i;

    int originalLength = strlen(binaryStr);

    if (originalLength >= desiredLength)
    {
        /* No need to pad, return original string*/
        return binaryStr;
    }

    /* Allocate memory for the padded binary string*/
    paddedBinary = (char *)malloc((desiredLength + 1) * sizeof(char));

    /* Calculate number of leading zeros to pad*/
    numZeros = desiredLength - originalLength;

    /* Fill leading zeros*/
    for (i = 0; i < numZeros; i++)
    {
        paddedBinary[i] = '0';
    }

    /* Copy original binary string after leading zeros*/
    strcpy(paddedBinary + numZeros, binaryStr);

    return paddedBinary;
}

char *decimalToBinary(int num)
{
    /* Counter for binary array*/
    int i = 0;
    int j, k;
    char *binaryStr;
    /* Array to store binary digits*/
    char *binary = (char *)malloc(14 * sizeof(char));
    /* Convert decimal to binary*/
    while (num > 0)
    {
        binary[i] = (num % 2) + '0'; /* Convert digit to char*/
        num = num / 2;
        i++;
    }

    /* Allocate memory for the binary string*/
    binaryStr = (char *)malloc(i * sizeof(char));

    /* Construct binary string in reverse order*/
    for (j = i - 1, k = 0; j >= 0; j--, k++)
    {
        binaryStr[k] = binary[j];
    }
    binaryStr[i] = '\0'; /* Null-terminate the string*/
    /* Free the memory used by binary array*/
    free(binary);

    return binaryStr;
}

char **extractWords(char *sentence, int *numWords)
{
    char *token;
    char *delimiter = ", ";
    int count = 0;

    char **words = (char **)malloc(MAX_LABEL_LENGTH * sizeof(char *));
    if (words == NULL)
    {
        perror("Memory allocation failed");
        exit(EXIT_FAILURE);
    }

    token = strtok(sentence, delimiter);

    /* Skip until ".data"*/
    while (token != NULL && strcmp(token, ".data") != 0)
    {
        token = strtok(NULL, delimiter);
    }
    if (token == NULL)
    {
        *numWords = 0;
        return words;
    }

    /* Extract words after ".data"*/
    token = strtok(NULL, delimiter);
    while (token != NULL)
    {
        words[count] = (char *)malloc(MAX_LABEL_LENGTH * sizeof(char));
        if (words[count] == NULL)
        {
            perror("Memory allocation failed");
            exit(EXIT_FAILURE);
        }
        strcpy(words[count], token);
        count++;

        token = strtok(NULL, delimiter);
    }

    *numWords = count;
    return words;
}

char *extractSecondWord(char *line)
{
    const char *delimiter = " ";
    char *secondWord = NULL;
    char *token;
    char *copy = malloc(strlen(line) + 1); /* Allocate memory for a copy of the line */
    if (copy == NULL)
    {
        fprintf(stderr, "Memory allocation failed\n");
        exit(1);
    }
    strcpy(copy, line); /* Copy the line into the allocated memory */

    /* Get the first token (word) */
    token = strtok(copy, delimiter);
    /* Get the second token (word) */
    token = strtok(NULL, delimiter);

    if (token != NULL)
    {
        /* Allocate memory for the second word */
        secondWord = malloc(strlen(token) + 1);
        if (secondWord == NULL)
        {
            fprintf(stderr, "Memory allocation failed\n");
            exit(1);
        }
        strcpy(secondWord, token); /* Copy the second word into the allocated memory */
    }

    free(copy); /* Free the memory allocated for the copy */
    return secondWord;
}

char *binaryToCustomBase(char *binaryCode)
{
    int i;
    int num;
    char *base4Code;
    int length = strlen(binaryCode);
    if (length != 14)
    {
        fprintf(stderr, "Invalid binary code length. Expected 14 bits. - %s\n", binaryCode);
        exit(1);
    }

    base4Code = (char *)malloc(5 * sizeof(char));
    if (base4Code == NULL)
    {
        fprintf(stderr, "Memory allocation failed.\n");
        exit(1);
    }

    for (i = 0; i < 14; i += 2)
    {
        num = (binaryCode[i] - '0') * 2 + (binaryCode[i + 1] - '0');
        switch (num)
        {
        case 0:
            base4Code[i / 2] = '*';
            break;
        case 1:
            base4Code[i / 2] = '#';
            break;
        case 2:
            base4Code[i / 2] = '%';
            break;
        case 3:
            base4Code[i / 2] = '!';
            break;
        default:
            fprintf(stderr, "Invalid binary code: %s\n", binaryCode);
            free(base4Code);
            exit(1);
        }
    }

    return base4Code;
}
/* Function to extract the word inside square brackets */
char *extractWordInSquareBrackets(const char *input)
{
    char *openingBracket;
    char *closingBracket;
    size_t length;
    char *extractedWord;

    /* Find the position of '[' and ']' */
    openingBracket = strchr(input, '[');
    closingBracket = strchr(input, ']');

    /* If both '[' and ']' are present and ']' comes after '[' */
    if (openingBracket && closingBracket && closingBracket > openingBracket)
    {
        /* Calculate the length of the substring inside square brackets */
        length = closingBracket - (openingBracket + 1);

        /* Allocate memory for the extracted word */
        extractedWord = (char *)malloc(length + 1);
        if (extractedWord == NULL)
        {
            perror("Memory allocation failed");
            exit(EXIT_FAILURE);
        }

        /* Copy the substring inside square brackets into the extracted word */
        strncpy(extractedWord, openingBracket + 1, length);
        extractedWord[length] = '\0'; /* Null-terminate the string */

        return extractedWord;
    }

    return NULL; /* Return NULL if square brackets are not found or are in an invalid position */
}

/* Function to extract the word before square brackets */
/* Function to extract the word before the '[' character in a line */
char *extract_word_before_bracket(const char *line)
{
    int i = 0;
    int j = 0;
    int found_bracket = 0;
    char *result;
    int len;

    /* Iterate through the line to find the '[' character */
    while (line[i] != '\0')
    {
        if (line[i] == '[')
        {
            found_bracket = 1;
            break;
        }
        i++;
    }

    /* If '[' character not found, return NULL */
    if (!found_bracket)
    {
        return NULL;
    }

    i--; /* Move back from '[' to the last character of the word*/

    /* Skip any trailing spaces */
    while (i >= 0 && isspace(line[i]))
    {
        i--;
    }

    /* Find the beginning of the word */
    while (i >= 0 && !isspace(line[i]))
    {
        i--;
    }

    /* Allocate memory for the extracted word */
    result = (char *)malloc((j + 1) * sizeof(char));
    if (result == NULL)
    {
        return NULL; /* Memory allocation failed*/
    }

    /* Copy the word into the result buffer */
    while (line[i + 1] != '[' && !isspace(line[i + 1]) && line[i + 1] != '\0')
    {
        result[j++] = line[i + 1];
        i++;
    }
    result[j] = '\0';

    return result;
}

int isIndexConstantOperhand(char *ptrToOperhand, symbolList *mList)
{

    int isSymbol1;
    int isSymbol2;

    size_t length;
    size_t lengthBefore;

    char *extractedWord;
    char *extractedWordBefore;

    /* Find the position of '[' and ']' */
    char *openingBracket = strchr(ptrToOperhand, '[');
    char *closingBracket = strchr(ptrToOperhand, ']');

    if (openingBracket && closingBracket && closingBracket > openingBracket)
    {
        /* Calculate the length of the substring inside square brackets */
        length = closingBracket - (openingBracket + 1);

        /* Allocate memory for the extracted word */
        extractedWord = (char *)malloc(length + 1);
        if (extractedWord == NULL)
        {
            perror("Memory allocation failed");
            exit(EXIT_FAILURE);
        }

        /* Copy the substring inside square brackets into the extracted word */
        strncpy(extractedWord, openingBracket + 1, length);
        extractedWord[length] = '\0'; /* Null-terminate the string */

        /* Calculate the length of the substring before '[' if openingBracket exists */
        lengthBefore = openingBracket - ptrToOperhand;
        if (openingBracket)
        {
            /* Allocate memory for the extracted word before '[' */
            extractedWordBefore = (char *)malloc(lengthBefore + 1);
            if (extractedWordBefore == NULL)
            {
                perror("Memory allocation failed");
                exit(EXIT_FAILURE);
            }

            /* Copy the substring before '[' into the extracted word before */
            strncpy(extractedWordBefore, ptrToOperhand, lengthBefore);
            extractedWordBefore[lengthBefore] = '\0'; /* Null-terminate the string */

            /* Check if both extracted words are symbols already in the table */
            isSymbol1 = isSymbolAlreadyInTable(mList, extractedWord);
            isSymbol2 = isSymbolAlreadyInTable(mList, extractedWordBefore);

            /* Free dynamically allocated memory */
            free(extractedWordBefore);

            /* Free dynamically allocated memory for extracted word */
            free(extractedWord);

            /* Return 1 if both are symbols, otherwise 0 */
            return isSymbol1 && isSymbol2;
        }

        /* Free dynamically allocated memory for extracted word */
        free(extractedWord);
    }

    /* Return 0 if the operand doesn't contain a constant index or if symbols are not found */
    return 0;
}

int findTypeOperhand(char *ptrToOperhand, symbolList *mList)
{
    int operhandType = -1;
    /*adress method for the 00*/
    if (extractOperhand0(ptrToOperhand, mList) != NULL)
    {
        operhandType = 0;
    }
    /*adress method for the 01*/
    if (isSymbolAlreadyInTable(mList, ptrToOperhand))
    {
        operhandType = 1;
    }

    /*adress method for the 10*/
    if (isIndexConstantOperhand(ptrToOperhand, mList))
    {
        operhandType = 2;
    }

    /*adress method for the 11*/
    if (findRegister(ptrToOperhand) >= 0)
    {
        operhandType = 3;
    }

    return operhandType;
}

/* Function to extract the second operand from an instruction */
char *extractSecondOperand(const char *line)
{
    const char *ptr = line;
    const char *lastWordStart = NULL;
    const char *lastWordEnd = NULL;
    size_t wordLen;
    char *lastWord;
    /* Find the last comma*/
    while (*ptr)
    {
        if (*ptr == ',')
        {
            lastWordStart = ptr + 1;
            break;
        }
        ptr++;
    }

    /* If no comma found, return NULL*/
    if (!lastWordStart)
        return NULL;

    /* Find the start of the last word (skipping spaces)*/
    while (*lastWordStart && isspace(*lastWordStart))
        lastWordStart++;

    /* Find the end of the last word*/
    lastWordEnd = lastWordStart;
    while (*lastWordEnd && !isspace(*lastWordEnd))
        lastWordEnd++;

    /* Allocate memory for the last word*/
    wordLen = lastWordEnd - lastWordStart;
    lastWord = (char *)malloc(wordLen + 1);
    if (!lastWord)
        return NULL;

    /* Copy the last word*/
    strncpy(lastWord, lastWordStart, wordLen);
    lastWord[wordLen] = '\0';

    return lastWord;
}

/* Function to count the length of the string following the .string directive*/
int countStringChars(const char *line)
{
    const char *str_directive = ".string";         /* Directive to look for */
    const char *ptr = strstr(line, str_directive); /* Find the position of .string */

    int count;

    if (ptr == NULL)
    {
        return 0; /* If .string not found, return 0 */
    }
    ptr += strlen(str_directive); /* Move pointer to the end of .string */

    /* Move past any whitespace characters */
    while (*ptr != '\0' && isspace(*ptr))
    {
        ptr++;
    }

    /* Check if the string is enclosed in double quotes */
    if (*ptr == '"')
    {
        count = 0;
        ptr++; /* Move past the opening double quote */
        /* Count characters until the closing double quote is encountered */
        while (*ptr != '\0' && *ptr != '"')
        {
            count++;
            ptr++;
        }
        return count;
    }

    return 0; /* If the string is not enclosed in double quotes, return 0 */
}

/* Function to count the number of integers following the .data directive */
int countDataNumbers(const char *line)
{
    int count = 0;
    const char *data = ".data";           /* Directive to look for */
    const char *ptr = strstr(line, data); /* Find the position of .data */

    if (ptr == NULL)
    {
        return 0; /* If .data not found, return 0 */
    }
    ptr += strlen(data); /* Move pointer to the end of .data */

    /* Move past any whitespace characters */
    while (*ptr != '\0' && isspace(*ptr))
    {
        ptr++;
    }

    /* Count numbers separated by commas */
    while (*ptr != '\0')
    {
        /* Skip leading whitespace */
        while (*ptr != '\0' && isspace(*ptr))
        {
            ptr++;
        }
        /* If it's a digit or a sign followed by a digit, count it as a number */
        if (isdigit(*ptr) || ((*ptr == '-' || *ptr == '+') && isdigit(*(ptr + 1))))
        {
            count++;
            /* Move to the next character after the number */
            while (*ptr != '\0' && *ptr != ',' && !isspace(*ptr))
            {
                ptr++;
            }
        }
        else if (*ptr == ',')
        {
            /* Move past the comma */
            ptr++;
        }
        else
        {
            /* Move to the next character */
            ptr++;
        }
    }

    return count;
}

/*register finder*/
int findRegister(const char *word)
{
    int i;
    for (i = 0; i < 8; i++)
    {
        if (strcmp(word, REGS[i]) == 0)
        {
            return i; /* Return index if word is found */
        }
    }
    return -1; /* Return -1 if word is not found */
}

char *extractWordAfterDirective(const char *line, const char *directive)
{
    const char *position = strstr(line, directive);
    const char *end;
    char *word;
    if (position == NULL)
        return NULL; /* Directive not found in the line*/

    position += strlen(directive); /* Move to the position after the directive*/

    /* Skip leading whitespace*/
    while (isspace(*position))
        position++;

    /* Find the end of the word*/
    end = position;
    while (*end != '\0' && !isspace(*end))
        end++;

    /* Allocate memory for the word*/
    word = (char *)malloc(end - position + 1);
    if (word == NULL)
    {
        fprintf(stderr, "Memory allocation failed.\n");
        exit(1);
    }

    /* Copy the word into the allocated memory*/
    strncpy(word, position, end - position);
    word[end - position] = '\0'; /* Null-terminate the string*/

    return word;
}

/* Function to extract the word after an operation code */
char *extractOperandDestination(const char *line)
{
    char *token;
    const char *delimiters = " \t"; /* Delimiters include space and tab */
    char *lineCopy = (char *)malloc(strlen(line) + 1);
    if (lineCopy == NULL)
    {
        /* Memory allocation failed*/
        printf("Memory allocation failed\n");
        exit(1);
    }

    strcpy(lineCopy, line);

    /* Check for a label at the beginning */
    token = strtok(lineCopy, delimiters);
    if (token && strchr(token, ':'))
    {
        /* Skip label */
        token = strtok(NULL, delimiters);
    }

    /* The next token should be the opcode */
    if (token && is_opcode(token))
    {
        /* Get the next part which should start with the operands */
        token = strtok(NULL, "");
        if (token)
        {
            /* Find the start of the first operand */
            token = strtok(token, ", \t"); /* Get the first operand */
            return token;
        }
    }
    free(lineCopy);
    return NULL; /* No operand found */
}

int isNumber(const char *str)
{
    int i;
    for (i = 0; str[i] != '\0'; i++)
    {
        if (!isdigit(str[i]) && str[i] != '-' && str[i] != '+')
        {
            return 0;
        }
    }
    return 1;
}

/*operhand number 0*/ 
char *extractOperhand0(const char *sentence, symbolList *mList)
{
    int i;
    char *result;
    const char *start = strchr(sentence, '#'); /* Find the first occurrence of # */
    if (start == NULL)
        return NULL; /* If # is not found, return NULL */

    /* Move the pointer to the character after # */
    start++;

    /* Skip leading whitespace */
    while (isspace(*start))
    {
        start++;
    }

    /* Allocate memory for the word or number */
    result = (char *)malloc(strlen(start) + 1);
    if (result == NULL)
    {
        perror("Memory allocation failed");
        exit(EXIT_FAILURE);
    }

    /* Copy the characters into the result until a space or end of string is encountered */
    i = 0;
    while (start[i] != '\0' && !isspace(start[i]))
    {
        result[i] = start[i];
        i++;
    }
    result[i] = '\0'; /* Null-terminate the result string */

    for (i = 0; result[i] != '\0'; i++)
    {
        if (!isdigit(result[i]) && result[i] != '-' && result[i] != '+')
        {
            if (isSymbolAlreadyInTable(mList, result))
            {
                return result;
            }
            return NULL;
        }
    }
    return result;
}

char *convertBinaryNumber(char *binaryNumber)
{ /*check this*/
    /* Step 1: Find the index of the first '1' in the binaryNumber */
    int indexOfFirstOne = -1;
    int i;

    for (i = 0; i < strlen(binaryNumber); i++)
    {
        if (binaryNumber[i] == '1')
        {
            indexOfFirstOne = i;
            break;
        }
    }

    /* If no '1' is found, return the original binaryNumber */
    if (indexOfFirstOne == -1)
        return binaryNumber;

    /* Step 2: Invert all the bits after the first '1' */
    for (i = indexOfFirstOne + 1; i < strlen(binaryNumber); i++)
    {
        if (binaryNumber[i] == '0')
            binaryNumber[i] = '1';
        else
            binaryNumber[i] = '0';
    }

    return binaryNumber;
}

/* Function to calculate the two's complement */

char *decimalToBinary2Complement(int num)
{
    char *binary;

    if (num >= 0)
    {
        binary = decimalToBinary(num);
        return binary;
    }
    else
    {
        num = -num;
        binary = decimalToBinary(num);
        bitwiseNOT(binary);
        addOne(binary);
        return binary;
    }
}

void bitwiseNOT(char *binaryStr)
{
    int length = strlen(binaryStr);
    int i;
    /* Iterate through each character in the string*/
    for (i = 0; i < length; i++)
    {
        /* Perform bitwise NOT operation*/
        if (binaryStr[i] == '0')
        {
            binaryStr[i] = '1';
        }
        else if (binaryStr[i] == '1')
        {
            binaryStr[i] = '0';
        }
        /* Ignore characters other than '0' and '1'*/
    }
}

void addOne(char *binaryStr)
{
    int length = strlen(binaryStr);

    /* Start from the least significant bit*/
    int i = length - 1;
    int carry = 1; /* Carry bit initially set to 1*/

    /* Iterate through each bit in the binary string*/
    while (i >= 0 && carry)
    {
        /* If the current bit is '0', set it to '1' and exit the loop*/
        if (binaryStr[i] == '0')
        {
            binaryStr[i] = '1';
            carry = 0; /* No need to carry forward*/
        }
        /* If the current bit is '1', set it to '0' and continue to next bit*/
        else
        {
            binaryStr[i] = '0';
            i--;
        }
    }

    /* If there's still a carry after the loop ends, prepend '1' to the binary string*/
    if (carry)
    {
        printf("Overflow: Unable to add one. The result will overflow.\n");
    }
}

int *parseDataNumbers(char *line)
{
    int *numbers;
    const char *dataPos = strstr(line, ".data");
    int count = 0;
    char *endPtr;
    long number;

    if (dataPos == NULL)
        return NULL; /* .data directive not found*/

    numbers = (int *)malloc(MAX_NUMBER_LENGTH * sizeof(int));
    if (numbers == NULL)
    {
        fprintf(stderr, "Memory allocation failed.\n");
        exit(1);
    }

    /* Move to the position after ".data"*/
    dataPos += 5;

    /* Read numbers separated by commas*/
    while (*dataPos != '\0')
    {
        /* Read the number*/
        number = strtol(dataPos, &endPtr, 10);

        /* Check if a valid number was found*/
        if (endPtr != dataPos)
        {
            numbers[count++] = (int)number;

            /* Skip whitespace after the number*/
            dataPos = endPtr;

            /* Skip leading whitespace before the next comma or end of line*/
            while (isspace(*dataPos))
                dataPos++;

            /* Check for comma*/
            if (*dataPos == ',')
            {
                /* Move to the next character after the comma*/
                dataPos++;
            }
            else if (!isspace(*dataPos) && *dataPos != '\0')
            {
                /* Invalid character after the number*/
                fprintf(stderr, "Error: Invalid character found after number in .data line.\n");
                free(numbers);
                return NULL;
            }
        }
        else
        {
            /* Invalid number format*/
            fprintf(stderr, "Error: Invalid number format in .data line.\n");
            free(numbers);
            return NULL;
        }
    }

    return numbers;
}

char *findNumericConstant(char *line)
{
    int i = 0;
    char *numericConstant;

    /* Skip until '=' character */
    while (*line != '=' && *line != '\0')
    {
        line++;
    }

    /* Check if '=' is present */
    if (*line != '=')
        return NULL;

    /* Skip '=' character */
    line++;

    /* Skip whitespace after '=' */
    while (isspace(*line))
        line++;

    /* Allocate memory for the numeric constant */
    numericConstant = (char *)malloc(MAX_LABEL_LENGTH + 1);
    if (numericConstant == NULL)
    {
        printf("Memory allocation failed.\n");
        exit(1); /* Exit program with error */
    }

    /* Extract the numeric constant */
    while (*line != '\0' && (isdigit(*line) || *line == '.' || *line == '-' || *line == '+'))
    {
        numericConstant[i++] = *line;
        line++;
    }
    numericConstant[i] = '\0'; /* Null-terminate the numeric constant */

    return numericConstant;
}

/*proof*/
/*method to find the val in a .define sentance*/
char *findConstant(char *line)
{ 
    char *constant;
    int i;
    /* Skip ".define " part */
    line += 8;

    /* Skip leading whitespace */
    while (isspace(*line))
        line++;

    /* Allocate memory for the constant name */
    constant = (char *)malloc(MAX_LABEL_LENGTH + 1);
    if (constant == NULL)
    {
        printf("Memory allocation failed.\n");
        exit(1); /* Exit program with error */
    }

    /* Extract the constant name */
    i = 0;
    while (*line != '\0' && *line != ' ' && *line != '=')
    {
        constant[i++] = *line;
        line++;
    }
    constant[i] = '\0'; /* Null-terminate the constant name */

    return constant;
}

bool isLabel(char *line)
{
    int labelLength;
    /* Check if the line is empty or NULL */
    if (line == NULL || *line == '\0')
        return false;

    /* Skip leading whitespace */
    while (isspace(*line))
        line++;

    /* Check if the first character is an alphabet letter */
    if (!isalpha(*line))
        return false;

    /* Check if the label contains only valid characters and is not longer than the maximum length */
    labelLength = 0;
    while (*line != '\0' && labelLength < MAX_LABEL_LENGTH && *line != ':')
    {
        if (!isalnum(*line) && *line != '_')
            return false;
        line++;
        labelLength++;
    }

    /* Check if the label ends with ':' and there is no whitespace before it */
    if (*line == ':' && !isspace(*(line - 1)))
        return true;

    return false;
}

char *getLabel(char *line)
{ 
    int i;
    char *label;
    /* Check if the line contains a label */
    if (!isLabel(line))
        return NULL;

    /* Allocate memory for the label */
    label = (char *)malloc(MAX_LABEL_LENGTH + 1);
    if (label == NULL)
    {
        printf("Memory allocation failed.\n");
        exit(1); /* Exit program with error */
    }

    /* Extract the label */
    i = 0;
    while (*line != '\0' && *line != ':' && i < MAX_LABEL_LENGTH)
    {
        if (isalnum(*line) || *line == '_')
        {
            label[i++] = *line;
        }
        line++;
    }
    label[i] = '\0'; /* Null-terminate the label */

    return label;
}

/* Function to find the number of operands for a given opcode name */
int findNumberOfOperands(const char *name)
{
    /* Iterate through the array to find the matching opcode name */
    int i;
    for (i = 0; i < sizeof(OPCODESNumberOfOperands) / sizeof(OPCODESNumberOfOperands[0]); i++)
    {
        if (memcmp(OPCODESNumberOfOperands[i].opcode, name, 3) == 0)
        {
            return OPCODESNumberOfOperands[i].numberOfOpcode;
        }
    }
    /* Return -1 if the name is not found */
    return -1;
}

void addBittsToBinary(char *str, int start_bit, int end_bit, char *output)
{
    int i = strlen(str) - 1;
    int j = 14 - start_bit - 1;
    for (i; i >= 0; i--)
    {
        output[j] = str[i];
        j--;
    }
}

char *decimalToBinaryNOTUSE(int n, int num_digits)
{
    int i;
    int start, end;

    char temp;
    char *binary = (char *)malloc((num_digits) * sizeof(char)); /* Allocate memory for the binary string */
    if (binary == NULL)
    {
        printf("Memory allocation failed.\n");
        exit(1); /* Exit program with error */
    }

    /* Convert decimal to binary */
    i = 0;
    while (n > 0 || i < num_digits)
    {
        binary[i++] = (n % 2) + '0'; /* Convert digit to character */
        n /= 2;
    }

    /* Reverse the binary string */
    start = 0;
    end = i - 1;
    while (start < end)
    {
        temp = binary[start];
        binary[start] = binary[end];
        binary[end] = temp;
        start++;
        end--;
    }
    return binary;
}

int findInstructionInLine(const char *sentence)
{
    int i = 0;
    char *token;
    char sentence_copy[MAX_LINE_LENGTH];

    /* Make a copy of the sentence since strtok modifies the original string */
    strncpy(sentence_copy, sentence, MAX_LINE_LENGTH);
    sentence_copy[MAX_LINE_LENGTH - 1] = '\0'; /* Ensure null-termination */

    /* Tokenize the sentence based on spaces */
    token = strtok(sentence_copy, " ");
    while (token != NULL)
    {
        /* Check if the token is in the array of instructions */
        for (i = 0; i < 16; i++)
        {
            if (memcmp(token, OPNumbersOfCode[i], 3) == 0)
            {
                return i; /* Instruction found in the sentence, return its index */
            }
        }
        token = strtok(NULL, " ");
    }
    return -1; /* No instruction from the array is present in the sentence */
}

/*number to binery*/
char *bin(int num, int num_bits)
{
    static char bin_num[33]; /* 32 bits for the binary number + 1 for null terminator */
    int i = num_bits;
    bin_num[num_bits + 1] = '\0';

    for (; i >= 0; i--)
    {
        bin_num[i] = (num & (1 << (num_bits - 1))) == 0 ? '0' : '1';
        num <<= 1;
    }

    return bin_num;
}
