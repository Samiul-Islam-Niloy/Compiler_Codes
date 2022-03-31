#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<ctype.h>
#define NUMBER_OF_KEYWORDS 32


typedef struct ErrorInfo ErrorInfo;

struct ErrorInfo
{
    int linenumber;
    char errortype[5];
    char errorvalue[100];
    char errorMess[100];
};

char keywords[NUMBER_OF_KEYWORDS][100] =
{
    "auto","break","case","char","const","continue","default",
    "do","double","else","enum","extern","float","for","goto",
    "if","int","long","register","return","short","signed",
    "sizeof","static","struct","switch","typedef","union",
    "unsigned","void","volatile","while"
};

int isKeyword(char str[])
{
    int flag = 0;

    for(int i = 0; i < NUMBER_OF_KEYWORDS; i++)
    {
        if(strcmp(keywords[i], str) == 0)
        {
            flag = 1;
            break;
        }
    }

    return flag;
}

int isOperator(char ch)
{
    int flag = 0;
    if (ch == '+' || ch == '-' || ch == '*' ||ch == '/' || ch == '>' || ch == '<' ||ch == '=' ||ch == '!' || ch == '&' || ch == '|')
    {
        flag = 1;
    }
    return flag;
}

int isSeperator(char ch)
{
    int flag = 0;
    if (ch == ';' || ch == ',' || ch == '\'' )
    {
        flag = 1;
    }
    return flag;
}

int isAvoid(char ch)
{
    int flag = 1;
    if (ch >= 0 && ch <= 32 )
    {
        flag = 0;
    }
    return flag;
}

int isNumber(char str[])
{
    int flag = 1;
    int numberOfpoint = 0;
    char str1[strlen(str) + 1];
    strcpy(str1,str);

    for (int i = 0; i < strlen(str1); i++)
    {
        if(str1[i] >= 'A' && str1[i] <= 'Z')
        {
            str1[i] = str1[i] + 32;
        }
    }

    if(str1[0] == '0' && str1[1] == 'x')
    {
        for(int i = 2; i < strlen(str1); i++)
        {
            if((str1[i] >= '0' && str1[i] <= '9') || (str1[i] >= 'a' && str1[i] <= 'f') )
            {
                continue;
            }
            else
            {
                return 0;
            }
        }
        return 1;
    }
    for(int i = 0; i < strlen(str1); i++)
    {
        if(isdigit(str1[i]))
        {
            flag = 1;
            continue;
        }
        else if(str1[i] == '.')
        {
            numberOfpoint++;
        }
        else
        {
            flag = 0;
            break;

        }
    }
    if(numberOfpoint <=1 && flag == 1)
        return 1;
    else
        return 0;
}

int isValidIdentifier(char str[])
{
    if (!((str[0] >= 'a' && str[0] <= 'z') || (str[0] >= 'A' && str[1] <= 'Z')|| str[0] == '_'))
        return 0;

    for (int i = 1; i < strlen(str); i++)
    {
        if (!((str[i] >= 'a' && str[i] <= 'z') || (str[i] >= 'A' && str[i] <= 'Z') || (str[i] >= '0' && str[i] <= '9') || str[i] == '_'))
            return 0;
    }

    return 1;
}


int isParen(char ch)
{
    int flag = 0;
    if (ch == '(' || ch == ')' || ch == '{'  || ch == '}' || ch == '[' || ch == ']' )
    {
        flag = 1;
    }
    return flag;
}

void removeSpace(char str[])
{
    char output[strlen(str)+1];
    int j = 0;
    for(int i = 0; i < strlen(str) ; i++)
    {
        if(str[i] != ' ')
        {
            output[j] = str[i];
            j++;
        }
    }
    output[j] = '\0';
    strcpy(str, output);
}

ErrorInfo allErrors[1000];
char input[1000000] ;
char all_code_in_line[1000][1000] ;
char allVal_token[1000][1000] ;
char allVal_token_type[1000][1000] ;

int token_map_line[1000];
int allVal_token_size = 0;
int lineCount = 1;
int numberOfError = 0;

char stackParen[10000][5] ;
int stackParenHead = 0;

int ifCount= 0;

void fileRead(char *fileName)
{
    FILE *fptr;
    char c;
    int i = 0;
    if ((fptr = fopen(fileName, "r")) == NULL)
    {
        printf("Error! opening file");
    }
    else
    {
        while((c = fgetc(fptr)) != EOF)
        {
            input[i] = c;
            i++;
        }
    }
    fclose(fptr);
}


void printOutput(char str[])
{
    printf("%s\n",str);
}


void commentRemove()
{
    char output[1000000];
    int j = 0;
    for(int i = 0; i < strlen(input) ; i++ )
    {
        if(input[i] == '/' && input[i+1] == '*' )
        {
            int multiStart = 1;
            while(multiStart)
            {
                if(input[i]== '\n')
                    output[j++] = input[i];
                i++;
                if(input[i] == '*' &&  input[i+1] == '/')
                {
                    multiStart = 0;
                    i++;
                }

            }
        }
        else if(input[i] == '/' && input[i+1] == '/')
        {
            int singleStart = 1;
            while(singleStart)
            {
                if(input[i+1] == '\n')
                    output[j++] = input[i+1];
                i++;
                if(input[i] == '\n')
                {
                    singleStart = 0;
                }
            }
        }
        else
        {
            output[j] = input[i];
            j++;
        }

    }
    output[j] = '\0';
    strcpy(input,output);
}


void trim(char str[])
{
    char output[10000];
    int length = strlen(str);
    if(length <= 0)
        return ;

    int j = 0;
    int start  = 0;
    for(int i = 0; str[i] == ' ' ; i++)
        start++;

    for(int i = start ; i < strlen(str) ; i++ )
    {

        if(str[i] == ' '  && str[i+1] == ' ')
        {
            int space = 1;
            while(space)
            {
                if(input[i]== '\n')
                    output[j++] = input[i];
                i++;
                if(str[i+1] != ' ')
                {
                    space = 0;
                    output[j] = ' ';
                    j++;
                }
            }
        }
        else
        {
            output[j] = str[i];
            j++;
        }
    }
    output[j] = '\0';
    if(output[strlen(output)-1] == ' ')
        output[strlen(output)-1] = '\0';


    int i = 0;
    for(; i<strlen(output) ; i++)
    {
        str[i] = output[i];
    }

    str[i] = '\0';
}


void convertInLine()
{
    char buffer[500];
    for(int i = 0,j=0 ; i < strlen(input) ; i++)
    {
        buffer[j++] = input[i];
        if(input[i] == '\n')
        {
            buffer[j] = '\0';
            trim(buffer);
            strcpy(all_code_in_line[lineCount], buffer );
            lineCount++;
            j = 0;
        }
    }
}


void convertString()
{
    char output[1000000];
    int j = 0;
    for(int i = 0; i < strlen(input) ; i++)
    {
        if(isSeperator(input[i]) || isOperator(input[i]) || isParen(input[i]))
        {
            output[j++] = ' ';
            output[j++] = input[i];
            output[j++] = ' ';
        }
        else
        {
            output[j++] = input[i];
        }
    }
    output[j] = '\0';
    strcpy(input, output);
}


char* check_type(char str[])
{
    if(isSeperator(str[0]))
    {
        return "sp";
    }
    else if(isParen(str[0]))
    {
        return "par";
    }
    else if(isOperator(str[0]))
    {
        return "op";
    }
    else
    {
        trim(str);
        if(isKeyword(str))
        {
            return "kw";
        }
        else if(isNumber(str))
        {
            return "num";
        }
        else if(isValidIdentifier(str))
        {
            return "id";
        }
    }
    return "unknown";

}

void parse_token()
{
    for(int i = 1 ; i <= lineCount; i++)
    {
        char buffer[500];
        for(int j = 0,k = 0; j < strlen(all_code_in_line[i]) ; j++)
        {
            buffer[k++] = all_code_in_line[i][j];
            if(all_code_in_line[i][j] == ' ')
            {
                buffer[k] = '\0';

                char *ty = check_type(buffer);
                trim(buffer);
                strcpy(allVal_token[allVal_token_size],buffer);
                strcpy(allVal_token_type[allVal_token_size],ty);
                token_map_line[allVal_token_size] = i;
                allVal_token_size++;
                k = 0;
            }
        }

    }
}

void addedError(int currentLine, char type[],  char errorVal[],  char messages[])
{

    if(numberOfError == 0)
    {
        allErrors[numberOfError].linenumber = currentLine;
        strcpy(allErrors[numberOfError].errortype,type);
        strcpy(allErrors[numberOfError].errorvalue,errorVal);
        strcpy(allErrors[numberOfError].errorMess,messages);
        numberOfError++;
    }
    else
    {
        if(allErrors[numberOfError-1].linenumber != currentLine|| strcmp(allErrors[numberOfError-1].errortype, type) != 0 || strcmp(allErrors[numberOfError-1].errorvalue, errorVal) != 0 )
        {
            allErrors[numberOfError].linenumber = currentLine;
            strcpy(allErrors[numberOfError].errortype,type);
            strcpy(allErrors[numberOfError].errorvalue,errorVal);
            strcpy(allErrors[numberOfError].errorMess,messages);
            numberOfError++;
        }
    }
}

int main()
{
    fileRead("input.c");
    printf("\n");
    printOutput(input);

    commentRemove();
    convertString();
    convertInLine();

    printf("\n\n\n\n-------------Adding Line Number-------------\n\n");
    for(int i = 1 ; i <= lineCount; i++)
    {
        printf("%d    %s\n",i,all_code_in_line[i]);
    }

    parse_token();

    printf("\n\n\n\n-------------With Token Value-------------\n\n");
    int currntLine = 1;
    for(int i = 0 ; i <= allVal_token_size; i++)
    {
        if(currntLine != token_map_line[i] && token_map_line[i]!=0)
        {
            printf("\n\n");
            printf("%d    ",token_map_line[i]);
        }
        currntLine = token_map_line[i];
        printf("%s %s ",allVal_token_type[i], allVal_token[i]);

    }

    for(int i = 0 ; i <= allVal_token_size; i++)
    {

        if( strcmp(allVal_token[i], "for") == 0 && strcmp(allVal_token[i+1], "(") == 0 && strcmp(allVal_token[i+2], ";") == 0 && strcmp(allVal_token[i+3], ";") == 0 && strcmp(allVal_token[i+4], ")") == 0)
        {
            i += 4;
            continue;
        }

        if(allVal_token[i][0]=='+' && allVal_token[i+1][0]=='+')
        {
            i+=2;
            continue;

        }
        if(allVal_token[i][0]=='=' && allVal_token[i+1][0]=='=')
        {
            i+=2;
            continue;

        }
        if(allVal_token[i][0]=='-' && allVal_token[i+1][0]=='-')
        {
            i+=2;
            continue;

        }
        if(allVal_token[i][0]=='<' && allVal_token[i+1][0]=='=')
        {
            i+=2;
            continue;

        }
        if(allVal_token[i][0]=='>' && allVal_token[i+1][0]=='=')
        {
            i+=2;
            continue;

        }
        if( strcmp(allVal_token[i], "if") == 0)
        {
            ifCount++;
        }

        if(strcmp(allVal_token[i], "else") == 0 )
        {
            if(ifCount<=0)
            {
                addedError(token_map_line[i], allVal_token_type[i],allVal_token[i], "Unmatched else");
            }
            else
            {
                ifCount--;
            }
        }


        if(isParen(allVal_token[i][0]))
        {

            if(allVal_token[i][0] == '(' || allVal_token[i][0] == '{' || allVal_token[i][0] == '[')
            {
                strcpy(stackParen[stackParenHead], allVal_token[i]);
                stackParenHead +=1;
            }
            else
            {
                int crna_stacl = stackParenHead-1;
                if(allVal_token[i][0] == '}' && stackParen[crna_stacl][0] != '{')
                {
                    addedError(token_map_line[i], allVal_token_type[i],allVal_token[i], "Unbalanced Bracket");
                }
                else if(allVal_token[i][0] == ')' && stackParen[crna_stacl][0] != '(')
                {
                    addedError(token_map_line[i], allVal_token_type[i],allVal_token[i], "Unbalanced Bracket");
                }
                else if(allVal_token[i][0] == ']' && stackParen[crna_stacl][0] != '[')
                {
                    addedError(token_map_line[i], allVal_token_type[i],allVal_token[i], "Unbalanced Bracket");
                }
                else
                {
                    stackParenHead -= 1;
                }
            }
        }
        else if(strcmp(allVal_token_type[i], allVal_token_type[i-1] ) == 0)
        {

            addedError(token_map_line[i], allVal_token_type[i],allVal_token[i], "Duplicate token");
        }
        else if(strcmp(allVal_token[i], allVal_token[i-1] ) == 0)
        {

            addedError(token_map_line[i], allVal_token_type[i],allVal_token[i], "Duplicate token");
        }


    }
    printf("\n\n\n\n-------------Error-------------\n\n");
    while(stackParenHead >= 1)
    {

        addedError(lineCount-1,"par",stackParen[stackParenHead--], "Unbalanced Bracket");
    }
    printf("\n\n");
    for(int i = 0 ; i < numberOfError; i++)
    {
        printf(" line no %d - error type %s - error: %s - %s \n",allErrors[i].linenumber, allErrors[i].errortype, allErrors[i].errorvalue, allErrors[i].errorMess );
    }

}
