/*
--------------------------------------------------
CSE4130: Formal Languages & Compilers Lab Project
Name: Samiul Islam Niloy
ID: 170204016
Section: A
Lab Group: A1
--------------------------------------------------
*/
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include <stddef.h>
#include <stdbool.h>
#include <bits/stdc++.h>
#define NUMBER_OF_KEYWORDS 32
using namespace std;

// Value definitions
#define NULL_CHAR '\0'

#define QUOTE_NONE '\0'

#define COMMENT_NONE 0
#define COMMENT_SINGLE 1
#define COMMENT_MULTI 2

// Global variables
char *source_code = NULL;
int source_length;

char quote_type = QUOTE_NONE;
bool escape_char = false;
int comment_type = COMMENT_NONE;

char *filtered_code = NULL;
int filtered_length = 0;

int i;

// Function prototypes
void filter_code();

void filter_comments();

void filter_whitespaces();

//Step 0 Begins here
void fileWrite(char *fileName, char sentence[])
{
    FILE *fptr;
    fptr = fopen(fileName, "w");

    if (fptr == NULL)
    {
        printf("Error! opening file");
        exit(1);
    }
    fprintf(fptr, "%s", sentence);
    fclose(fptr);
}

void step0()
{
    printf("------------------------------------------------------------------------------\n");
    printf("Please Enter The Input. If you want to stop input please EOF or CTRL+Z: \n");
    printf("------------------------------------------------------------------------------\n");
    char str[1000000];
    int i = 0;

    for( i = 0 ; scanf("%c",&str[i]) != EOF ; i++);

    fileWrite("input.txt",str);
}
//Step 0 Ends here

//Step 1 Begins here
void step1()
{
    printf("\n------------------------------------------------------------------------------\n");
    printf("Step 01: Scanning and Filtering a Source Program \n");
    printf("------------------------------------------------------------------------------\n");
    // Opening source file
    FILE *source_file = fopen("input.txt", "rb");
    FILE *filtered_file = fopen("filtered.txt", "w");

    if (source_file)
    {
        // Finding the length of the source file
        fseek(source_file, 0, SEEK_END);
        source_length = ftell(source_file);
        fseek(source_file, 0, SEEK_SET);

        // Allocating memory to store the source code as a string
        source_code = (char*)malloc(source_length + 1);
        source_code[source_length] = NULL_CHAR;

        // Reading the source file as string
        if (fread(source_code, 1, source_length, source_file) == source_length)
        {
            // Displaying source code
            printf("Source code:\n");
            fputs(source_code, stdout);

            // Calling function to filter the source code
            filter_code();

            // Saving the filtered code in a file
            fputs(filtered_code, filtered_file);

            // Displaying the filtered code
            printf("\nFiltered code:\n");
            fputs(filtered_code, stdout);
        }
        else
        {
            printf("File could not be read!");
        }

        fclose(source_file);
        fclose(filtered_file);
    }
    else
    {
        printf("File could not be opened!");
    }
}

// This function filters the source code
void filter_code()
{
    // Allocating memory to store the filtered code as a string
    filtered_code = (char*)malloc(source_length + 1);
    filtered_code[source_length] = NULL_CHAR;

    for (i = 0; i < source_length; i++)
    {
        if (quote_type == QUOTE_NONE)
        {
            if (source_code[i] == '"' || source_code[i] == '\'')
            {
                if (comment_type == COMMENT_NONE)
                {
                    // Start of a quote, that means a string or character
                    quote_type = source_code[i];

                    // Adding to the filtered code --------------------------------------------------------------------
                    filtered_code[filtered_length++] = source_code[i];
                }
            }
            else
            {
                // This is certain that this character is not inside any string or character
                // Calling function to filter comment
                filter_comments();
            }
        }
        else if (!escape_char)
        {
            if (source_code[i] == '\\')
            {
                // Setting flag to escape the next character
                escape_char = true;
            }
            else if (source_code[i] == quote_type)
            {
                // End of the quote, that means a string or a character
                quote_type = QUOTE_NONE;
            }

            // Adding to the filtered code ----------------------------------------------------------------------------
            filtered_code[filtered_length++] = source_code[i];
        }
        else
        {
            // Resetting flag to not escape any more character
            escape_char = false;

            // Adding to the filtered code ----------------------------------------------------------------------------
            filtered_code[filtered_length++] = source_code[i];
        }
    }

    // Reallocating memory to free extra allocated space
    realloc(filtered_code, filtered_length + 1);
    filtered_code[filtered_length] = NULL_CHAR;
}

// This function filters out the comments
void filter_comments()
{
    if (comment_type == COMMENT_NONE)
    {
        if (source_code[i] == '/')
        {
            if (source_code[i + 1] == '/')
            {
                // Start of a single line comment
                comment_type = COMMENT_SINGLE;
                i++;
            }
            else if (source_code[i + 1] == '*')
            {
                // Start of a multiline comment
                comment_type = COMMENT_MULTI;
                i++;
            }
            else
            {
                // This is certain that this character is not inside any comment
                // Calling function to filter whitespaces
                filter_whitespaces();
            }
        }
        else
        {
            // This is certain that this character is not inside any comment
            // Calling function to filter whitespaces
            filter_whitespaces();
        }
    }
    else if (comment_type == COMMENT_SINGLE)
    {
        if (source_code[i] == '\n')
        {
            // End of the single line comment
            comment_type = COMMENT_NONE;

            // This is certain that this character is not inside any comment
            // Calling function to filter whitespaces
            filter_whitespaces();
        }
    }
    else if (comment_type == COMMENT_MULTI)
    {
        if (source_code[i] == '*')
        {
            if (source_code[i + 1] == '/')
            {
                // End of the multi line comment
                comment_type = COMMENT_NONE;
                i++;
            }
        }
    }
}

// This function filters out the extra white spaces
void filter_whitespaces()
{
    if (isspace(source_code[i]))
    {
        if (filtered_length > 0)
        {
            // Checking if the previously taken character is a space or punctuation mark
            if (!isspace(filtered_code[filtered_length - 1])
                    && (!ispunct(filtered_code[filtered_length - 1]) || filtered_code[filtered_length - 1] == '_'))
            {
                // Finding the next non space character
                while (isspace(source_code[i]))
                {
                    i++;
                }

                // Checking if this character is a punctuation mark
                if (!ispunct(source_code[i]) || source_code[i] == '_')
                {
                    // Adding to the filtered code --------------------------------------------------------------------
                    filtered_code[filtered_length++] = ' ';
                }

                // Adding to the filtered code ------------------------------------------------------------------------
                filtered_code[filtered_length++] = source_code[i];
            }
        }
    }
    else
    {
        // Adding to the filtered code --------------------------------------------------------------------------------
        filtered_code[filtered_length++] = source_code[i];
    }
}
//Step 1 Ends Here


//Step 2 Begins Here

void printInput()
{
    FILE *fp1;
    char ch;
    fp1 = fopen("filtered.txt", "r");
    if (fp1 == NULL)
    {
        printf("error while opening the file\n");
        exit(0);
    }
    printf("\n\nSample Input:\n");
    while ((ch = fgetc(fp1)) != EOF)
    {
        printf("%c",ch);
    }
    fclose(fp1);
}

void separateLexemes()
{
    FILE *fp1, *fp2;
    fp1 = fopen("filtered.txt", "r");
    fp2 = fopen("step01.txt", "w");
    char ch1, ch2;
    char separator[] = ";,:'";
    char operators[] = "+-*/=&|!<>%";
    char parenthesis[] = "(){}[]";
    if (!fp1)
    {
        printf("File can not be opened");
    }
    while ((ch1 = fgetc(fp1)) != EOF)
    {
        int i = 0, j = 0, k = 0;

        if (ch1 == '<' || ch1 == '>' || ch1 == '=')
        {
            ch2 = fgetc(fp1);
            if (ch2 == '=')
            {
                fputc(' ', fp2);
                fputc(ch1, fp2);
                fputc(ch2, fp2);
                fputc(' ', fp2);
            }
            else
            {
                fputc(' ', fp2);
                fputc(ch1, fp2);
                fputc(' ', fp2);
                if (ch2 != ' ')
                    fputc(ch2, fp2);
            }
            continue;
        }

        if (ch1 == '+' || ch1 == '-')
        {
            ch2 = fgetc(fp1);
            if (ch2 == '=' || ch2 == '+' || ch2 == '-')
            {
                fputc(' ', fp2);
                fputc(ch1, fp2);
                fputc(ch2, fp2);
                fputc(' ', fp2);
            }
            else
            {
                fputc(' ', fp2);
                fputc(ch1, fp2);
                fputc(' ', fp2);
                if (ch2 != ' ')
                    fputc(ch2, fp2);
            }
            continue;
        }

        if (ch1 == '&' || ch1 == '|')
        {
            ch2 = fgetc(fp1);
            if (ch2 == '&' || ch2 == '|')
            {
                fputc(' ', fp2);
                fputc(ch1, fp2);
                fputc(ch2, fp2);
                fputc(' ', fp2);
            }
            else
            {
                fputc(' ', fp2);
                fputc(ch1, fp2);
                fputc(' ', fp2);
                if (ch2 != ' ')
                    fputc(ch2, fp2);
            }
            continue;
        }

        if (ch1 == '!')
        {
            ch2 = fgetc(fp1);
            if (ch2 == '=')
            {
                fputc(' ', fp2);
                fputc(ch1, fp2);
                fputc(ch2, fp2);
                fputc(' ', fp2);
            }
            else
            {
                fputc(' ', fp2);
                fputc(ch1, fp2);
                fputc(' ', fp2);
                if (ch2 != ' ')
                    fputc(ch2, fp2);
            }
            continue;
        }

        for (j = 0; j < 4; j++)
        {
            if (ch1 == separator[j])
            {
                fputc(' ', fp2);
                fputc(ch1, fp2);
                fputc(' ', fp2);
                k = 1;
                break;
            }
        }
        for (j = 0; j < 11; j++)
        {
            if (ch1 == operators[j])
            {
                fputc(' ', fp2);
                fputc(ch1, fp2);
                fputc(' ', fp2);
                k = 1;
                break;
            }
        }
        for (j = 0; j < 6; j++)
        {
            if (ch1 == parenthesis[j])
            {
                fputc(' ', fp2);
                fputc(ch1, fp2);
                fputc(' ', fp2);
                k = 1;
                break;
            }
        }
        if (k)
            continue;
        fputc(ch1, fp2);
    }
    fclose(fp1);
    fclose(fp2);
}

int keywordDetect(char buffer[])
{
    char keywords[32][10] = {"auto", "break", "case", "char", "const", "continue", "default",
                             "do", "double", "else", "enum", "extern", "float", "for", "goto",
                             "if", "int", "long", "register", "return", "short", "signed",
                             "sizeof", "static", "struct", "switch", "typedef", "union",
                             "unsigned", "void", "volatile", "while"
                            };
    int i, flag = 0;

    for (i = 0; i < 32; ++i)
    {
        if (strcmp(keywords[i], buffer) == 0)
        {
            flag = 1;
            break;
        }
    }
    return flag;
}

int num_rec(char *lex)
{
    int i, l, s;
    i = 0;
    if (isdigit(lex[i]))
    {
        s = 1;
        i++;
    }
    else if (lex[i] == '.')
    {
        s = 2;
        i++;
    }
    else
        s = 0;
    l = strlen(lex);
    if (s == 1)
        for (; i < l; i++)
        {
            if (isdigit(lex[i]))
                s = 1;
            else if (lex[i] == '.')
            {
                s = 2;
                i++;
                break;
            }
            else
            {
                s = 0;
                break;
            }
        }
    if (s == 2)
        if (isdigit(lex[i]))
        {
            s = 3;
            i++;
        }
        else
            s = 0;
    if (s == 3)
        for (; i < l; i++)
        {
            if (isdigit(lex[i]))
                s = 3;
            else
            {
                s = 0;
                break;
            }
        }
    if (s == 3)
        s = 1;
    return s;
}

void printFirstStep()
{
    FILE *fp1;
    char ch;
    fp1 = fopen("step01.txt", "r");
    if (fp1 == NULL)
    {
        printf("error while opening the file\n");
        exit(0);
    }
    printf("\n\nStep 1:\n");
    while ((ch = fgetc(fp1)) != EOF)
    {
        printf("%c",ch);
    }
    fclose(fp1);
}

int processSecondStep()
{
    char ch, buffer[20], operators[] = "+-*/%&|!", parenthesis[] = "(){}[]";

    FILE *fp1, *fp2;

    int i, j = 0;
    int error = 0;

    fp1 = fopen("step01.txt", "r");
    fp2 = fopen("output.txt", "w");

    if (fp1 == NULL)
    {
        printf("error while opening the file\n");
        exit(0);
    }

    while ((ch = fgetc(fp1)) != EOF)
    {
        if (ch == '>' || ch == '<' || ch == '=')
        {
            char cd = fgetc(fp1);

            if (cd == '=')
            {
                //printf("[op %c%c] ", ch, cd);
                char key[] = "[op ";
                int m = strlen(key);
                int l = 0;
                for (l = 0; l < m; l++)
                    fputc(key[l], fp2);
                fputc(ch, fp2);
                fputc(cd, fp2);
                fputc(']', fp2);
                fputc(' ', fp2);
            }
            else
            {
                //printf("[op %c] ", ch);
                char key[] = "[op ";
                int m = strlen(key);
                int l = 0;
                for (l = 0; l < m; l++)
                    fputc(key[l], fp2);
                fputc(ch, fp2);
                fputc(']', fp2);
                fputc(' ', fp2);
            }
            continue;
        }

        for (i = 0; i < 8; ++i)
        {
            if (ch == operators[i])
            {
                char cd = fgetc(fp1);
                if (cd == '=' || cd == '+' || cd == '-' || cd == '|' || cd == '&')
                {
                    //printf("[op %c%c] ", ch, cd);
                    char key[] = "[op ";
                    int m = strlen(key);
                    int l = 0;
                    for (l = 0; l < m; l++)
                        fputc(key[l], fp2);
                    fputc(ch, fp2);
                    fputc(cd, fp2);
                    fputc(']', fp2);
                    fputc(' ', fp2);
                }
                else
                {
                    //printf("[op %c] ", ch);
                    char key[] = "[op ";
                    int m = strlen(key);
                    int l = 0;
                    for (l = 0; l < m; l++)
                        fputc(key[l], fp2);
                    fputc(ch, fp2);
                    fputc(']', fp2);
                    fputc(' ', fp2);
                }
            }
        }
        for (i = 0; i < 6; ++i)
        {
            if (ch == parenthesis[i])
            {
                //printf("[par %c] ", ch);
                char key[] = "[par ";
                int m = strlen(key);
                int l = 0;
                for (l = 0; l < m; l++)
                    fputc(key[l], fp2);
                fputc(ch, fp2);
                fputc(']', fp2);
                fputc(' ', fp2);
            }
        }

        if (ch == ',' || ch == ';' || ch == 39)
        {
            //printf("[sep %c] ", ch);
            char key[] = "[sep ";
            int m = strlen(key);
            int l = 0;
            for (l = 0; l < m; l++)
                fputc(key[l], fp2);
            fputc(ch, fp2);
            fputc(']', fp2);
            fputc(' ', fp2);
        }
        else if (isalnum(ch) || ch == '.' || ch == '_')
        {
            buffer[j++] = ch;
        }
        else if ((ch == ' ' || ch == '\n') && (j != 0))
        {
            buffer[j] = '\0';
            j = 0;

            if (keywordDetect(buffer) == 1)
            {
                //printf("[kw %s] ", buffer);
                char key[] = "[kw ";
                int m = strlen(key);
                int l = 0;
                for (l = 0; l < m; l++)
                    fputc(key[l], fp2);

                int n = strlen(buffer);
                int k = 0;
                for (k = 0; k < n; k++)
                    fputc(buffer[k], fp2);
                fputc(']', fp2);
                fputc(' ', fp2);
            }

            else
            {
                if (isdigit(buffer[0]))
                {
                    int num = num_rec(buffer);
                    if (num)
                    {
                        //printf("[num %s] ", buffer);
                        char num[] = "[num ";
                        int m = strlen(num);
                        int l = 0;
                        for (l = 0; l < m; l++)
                            fputc(num[l], fp2);

                        int n = strlen(buffer);
                        int k = 0;
                        for (k = 0; k < n; k++)
                            fputc(buffer[k], fp2);

                        fputc(']', fp2);
                        fputc(' ', fp2);
                    }
                    else
                    {
                        //printf("[unkn %s] ", buffer);
                        char unkn[] = "[unkn ";

                        int m = strlen(unkn);
                        int l = 0;
                        for (l = 0; l < m; l++)
                            fputc(unkn[l], fp2);

                        int n = strlen(buffer);
                        int k = 0;
                        for (k = 0; k < n; k++)
                            fputc(buffer[k], fp2);
                        fputc(']', fp2);
                        fputc(' ', fp2);
                        error = 1;
                    }
                }
                else
                {
                    if (buffer[0] == '.')
                    {
                        //printf("[num %s] ", buffer);
                        char num[] = "[num ";
                        int m = strlen(num);
                        int l = 0;
                        for (l = 0; l < m; l++)
                            fputc(num[l], fp2);

                        int n = strlen(buffer);
                        int k = 0;
                        for (k = 0; k < n; k++)
                            fputc(buffer[k], fp2);

                        fputc(']', fp2);
                        fputc(' ', fp2);
                    }
                    else
                    {
                        //printf("[id %s] ", buffer);
                        char iden[] = "[id ";
                        int m = strlen(iden);
                        int l = 0;
                        for (l = 0; l < m; l++)
                            fputc(iden[l], fp2);

                        int n = strlen(buffer);
                        int k = 0;
                        for (k = 0; k < n; k++)
                            fputc(buffer[k], fp2);
                        fputc(']', fp2);
                        fputc(' ', fp2);
                    }
                }
            }
        }
    }
    fclose(fp1);
    fclose(fp2);
    return error;
}

void printSecondStep(int error)
{
    FILE *fp1;
    char ch;
    fp1 = fopen("output.txt", "r");
    if (fp1 == NULL)
    {
        printf("error while opening the file\n");
        exit(0);
    }
    printf("\n\nStep 2:\n");
    while ((ch = fgetc(fp1)) != EOF)
    {
        printf("%c",ch);
    }
    if (error)
        printf("\n\nError! There are some unknown lexemes in the given input.\n");
    fclose(fp1);
}

void step2()
{
    printf("\n------------------------------------------------------------------------------\n");
    printf("Step 02: Lexical Analysis \n");
    printf("------------------------------------------------------------------------------\n");
    printInput();
    separateLexemes();
    printFirstStep();
    int e = processSecondStep();
    printSecondStep(e);
}

//Step 2 Ends Here


//Step 3 Starts Here

struct info
{
    string name,idtype,datatype,scope,value;
    info()
    {

    }
    info(string _name,string _idtype,string _datatype,string _scope,string _value)
    {
        name=_name;
        idtype=_idtype;
        datatype=_datatype;
        scope=_scope;
        value=_value;
    }
};

map<int,info>symboltable;
map<pair<string,string>,int >scopemap;
map<int,pair<string,string> >scopemaprev;
map<string,bool>TypeDetect;

void Display()
{
    cout<<endl<<endl;
    cout<<"Step 2:"<<endl<<endl;
    cout<<"Sl No.  Name  Id Type  Data Type Scope  Value"<<endl;

    for(auto it:symboltable)
    {
        cout<<it.first<<"\t"<<it.second.name<<"\t"<<it.second.idtype<<"\t"<<it.second.datatype<<"\t"<<it.second.scope<<"\t"<<it.second.value<<endl;
    }
    cout<<endl;
}

void Insert(string name,string idtype,string datatype,string scope,string value)
{
    int id=symboltable.size()+1;
    symboltable[id]= {name,idtype,datatype,scope,value};
    scopemap[ {name,scope}]=id;
    scopemaprev[id]= {name,scope};
}

void Update(int id,string name,string idtype,string datatype,string scope,string value)
{
    symboltable[id]= {name,idtype,datatype,scope,value};

}

void Delete(string Scope)
{
    vector<int>delids;
    map<int,info>::iterator it;
    for(it=symboltable.begin(); it!=symboltable.end(); it++)
    {
        if(it->second.scope==Scope)
        {
            delids.push_back(it->first);
        }
    }
    for(auto id:delids)
    {
        it=symboltable.find(id);
        symboltable.erase(it);
    }
}

bool Search(string name,string scope)
{
    if(scopemap.find({name,scope})==scopemap.end())
    {
        return false;
    }
    return true;
}

string delbrac(string Token)
{
    return Token.substr(1,Token.size()-2);
}

int num(char c)
{
    return c>='0' && c<='9';
}

int integer(string value)
{
    int i,neg=0;
    for(i=0; i<value.size(); i++)
    {
        if(value[i]=='-')
        {
            neg++;
            continue;
        }
        if(num(value[i])==0)
            return 0;
    }
    if(neg>1 || (neg==1 && value[0]!='-') || (neg==1 && value.size()<2))
        return 0;

    return 1;
}

int points(string value)
{
    int i,point=0;
    for(i=0; i<value.size(); i++)
    {
        if(value[i]=='.')
        {
            point++;
        }
    }
    return point;
}

int floatnum(string value)
{
    int i,point=points(value),flag=1,len=value.size();
    for(i=0; i<len; i++)
    {
        if(value[i]=='.')
            continue;
        if(num(value[i])==0)
            flag=0;
    }
    if(point!=1 || flag==0 || value[0]=='.' || value[len-1]=='.')
        return 0;
    return 1;
}

bool isnum(string word)
{
    return floatnum(word) || integer(word);
}

void step3()
{
    printf("\n------------------------------------------------------------------------------\n");
    printf("Step 03: Symbol Table Construction and Management \n");
    printf("------------------------------------------------------------------------------\n");
    TypeDetect["int"]=true;
    TypeDetect["float"]=true;
    TypeDetect["double"]=true;
    TypeDetect["char"]=true;

    string TokenName,Token,word;

    ifstream inputfile ("output.txt");
    ofstream outputfile;
    outputfile.open("output3.txt");

    cout<<"Step 1:"<<endl<<endl;

    if(inputfile.is_open())
    {
        while(getline(inputfile,word))
        {
            stringstream tokenstream(word);
            while(tokenstream>>TokenName>>Token)
            {
                string checkToken="";
                if(TokenName=="[id")
                    checkToken=TokenName+" "+Token;
                else
                    checkToken="["+Token;
                outputfile<<checkToken<<" ";
                cout<<checkToken<<" ";
            }
        }
        inputfile.close();
    }
    outputfile.close();
    ifstream inputfile2("output3.txt");
    string block="global";
    bool blockIsOpen=false;
    string lastname="",lastdatatype="";
    vector<string>TokenVector;

    if(inputfile2.is_open())
    {
        while(getline(inputfile2,word))
        {
            stringstream tokenstream(word);
            while(tokenstream>>TokenName)
            {
                TokenVector.push_back(TokenName);
            }
        }
    }
    inputfile2.close();
    int i=0,len=TokenVector.size();
    while(i<len)
    {
        TokenName=TokenVector[i];
        if(TokenName=="[id")
        {
            Token=TokenVector[++i];
            Token.pop_back();
            lastname=Token;
            string idtype="var";

            if(delbrac(TokenVector[i+1])=="(")
                idtype="Func";

            if(!isnum(Token))
                Token="";

            if(!Search(lastname,block))
            {
                Insert(lastname,idtype,lastdatatype,block,Token);
            }
        }
        else
        {
            string checkToken=delbrac(TokenName);

            if(checkToken=="(")
            {
                if(block=="global")
                {
                    blockIsOpen=true;
                    block=lastname;
                }
            }
            else if(checkToken=="}")
            {
                blockIsOpen=false;
                block="global";
            }
            else if(checkToken=="=")
            {
                Token=TokenVector[++i];
                Token=delbrac(Token);

                if(!isnum(Token))
                    Token="";

                if(!Search(lastname,block))
                {
                    string idtype="var";
                    Insert(lastname,idtype,lastdatatype,block,Token);
                }
                else
                {
                    int id=scopemap[ {lastname,block}];
                    string idtype="var";
                    Update(id,lastname,idtype,lastdatatype,block,Token);
                }
            }
            else
            {
                if(TypeDetect.find(checkToken)!=TypeDetect.end())
                {
                    lastdatatype=checkToken;
                }
            }
        }
        i++;
    }
    Display();

    ofstream outputfile2;
    outputfile2.open("output2.txt");
    cout<<"Step 3:"<<endl<<endl;
    ifstream inputfile3("output3.txt");
    if(inputfile3.is_open())
    {
        while(getline(inputfile3,word))
        {
            stringstream tokenstream(word);
            while(tokenstream>>TokenName)
            {
                int er;
                if(TokenName=="[id")
                {
                    tokenstream>>Token;
                    Token.pop_back();
                    cout<<TokenName<<" ";
                    outputfile2<<TokenName<<" ";
                    for(auto it:symboltable)
                    {
                        if(Token.compare(it.second.name)==0)
                        {
                            cout<<it.first <<"] ";
                            outputfile2<<it.first<<"] ";
                            er=it.first;
                            break;
                        }
                    }
                }
                else if(TokenName=="[}]")
                {
                    symboltable.erase(er);
                    cout<<TokenName<<" ";
                    outputfile2<<TokenName<<" ";
                }
                else
                {
                    cout<<TokenName<<" ";
                    outputfile2<<TokenName<<" ";
                }
            }
        }
        inputfile3.close();
    }
    outputfile2.close();
    cout<<endl<<endl;
}

//Step 3 Ends Here


//Step 4 Starts Here

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

void step4()
{
    printf("\n------------------------------------------------------------------------------\n");
    printf("Step 04: Detecting Simple Syntax Errors \n");
    printf("------------------------------------------------------------------------------\n");
    fileRead("input.txt");
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

//Step 4 Ends Here


//Step 5 Starts Here

char str[100];

bool stat(int sta, int fin);
bool asgn_stat(int sta, int fin);
bool idnt(int sta, int fin);
bool dscn_stat(int sta, int fin);
bool loop_stat(int sta, int fin);
bool expn(int sta, int fin);
bool smpl_expn(int sta, int fin);
bool extn(int sta, int fin);
int relop(int sta);
bool extn1(int sta, int fin);

bool term(int sta, int fin);
bool fact(int sta, int fin);
bool id(int i);
bool num(int i);

bool stat(int sta, int fin)
{
    if(asgn_stat(sta, fin))
        return true;
    else if(dscn_stat(sta, fin))
        return true;
    else if(loop_stat(sta, fin))
        return true;
    else
        return false;
}

bool asgn_stat(int sta, int fin)
{
    int j = sta;

    while(j <= fin && str[j] != '=')
        ++j;
    if(j == fin)
        return false;
    else if(str[j] == '=')
        return (idnt(sta, j-1) && expn(j + 1, fin));
    else
        return false;
}

bool dscn_stat(int sta, int fin)
{
    int j, k;
    if(str[sta] == 'i' && str[sta + 1] == 'f' && str[sta + 2] == '(')
    {
        j = sta + 3;

        while(str[j] != ')')
            ++j;

        if(expn(sta + 3, j - 1))
        {
            j += 2;
            k = j;

            while(j < fin && str[j] != ' ')
                ++j;
            if(j == fin)
                return stat(k, j);
            else if(stat(k, j-1))
                return extn1(j + 1, fin);
        }
        return false;
    }
    return false;
}

bool loop_stat(int sta, int fin)
{
    char ch[4];
    ch[0] = str[sta];
    ch[1] = str[sta + 1];
    ch[2] = str[sta + 2];
    ch[3] = '\0';

    int j, k;
    if(str[sta] = 'w' && str[sta + 1] == 'h' && str[sta + 2] == 'i' && str[sta + 3] == 'l' && str[sta + 4] == 'e')
    {
        j = sta + 5;

        if(str[j] == '(')
        {
            ++j;
            k = j;

            while(j <= fin && str[j] != ')')
                ++j;

            if(expn(k, j-1))
            {
                j += 2;
                return stat(j, fin);
            }
            else
                return false;
        }
        return false;
    }

    if(!strcmp(ch, "for"))
    {
        j = sta + 3;

        if(str[j] == '(')
        {
            ++j;
            k = j;

            while(j <= fin && str[j] != ';')
                ++j;

            if(asgn_stat(k, j-1))
            {
                ++j;
                k = j;

                while(j <= fin && str[j] != ';')
                    ++j;
                if(expn(k, j-1))
                {
                    ++j;
                    k = j;

                    while(j <= fin && str[j] != ')')
                        ++j;
                    if(asgn_stat(k, j-1))
                        return stat(j + 2, fin);
                    return false;
                }
                return false;
            }
            return false;
        }
        return false;
    }
    return false;
}

bool expn(int sta, int fin)
{
    int j = sta;

    while(j < fin && !relop(j))
        ++j;

    if(j == fin)
        return smpl_expn(sta, fin);
    else if(smpl_expn(sta, j - 1) && extn(j, fin))
        return true;
    else
        return false;
}

bool idnt(int sta, int fin)
{
    if(isalpha(str[sta]) || str[sta] == '_')
    {
        int j = sta + 1;

        while(j <= fin)
        {
            if(!isalnum(str[j]) && str[j] != '_')
                break;
            ++j;
        }

        if(j == fin + 1)
            return true;
        return false;
    }
    return false;
}

bool extn(int sta, int fin)
{
    int val = relop(sta);
    if(val == 1 && smpl_expn(sta+2, fin))
        return true;
    else if(val == 2 && smpl_expn(sta+1, fin))
        return true;
    return false;
}

int relop(int sta)
{
    char ch[3];
    ch[0] = str[sta];
    ch[1] = str[sta + 1];
    ch[2] = '\0';

    if(!strcmp(ch, "==") || !strcmp(ch, "!=") || !strcmp(ch, "<=") || !strcmp(ch, ">="))
        return 1;
    else if(ch[0] == '>' || ch[0] == '<')
        return 2;
    return 0;
}

bool extn1(int sta, int fin)
{
    if(str[sta] == 'e' && str[sta + 1] == 'l' && str[sta + 2] == 's' && str[sta + 3] == 'e' && str[sta + 4] == ' ')
        return stat(sta + 5, fin);
    return false;
}

bool smpl_expn(int sta, int fin)
{
    int j = sta;
    int topB = -1;

    while(j < fin)
    {
        if((str[j] == '+' || str[j] == '-') && topB == -1)
            break;
        if(str[j] == '(')
            ++topB;
        else if(str[j] == ')')
            --topB;
        ++j;
    }

    if(j == fin)
        return term(sta, fin);
    else if((str[j] == '+' || str[j] == '-') && term(sta, j - 1))
        return term(j + 1, fin);
    else
        return false;
}

bool term(int sta, int fin)
{
    int j = sta;
    int topB = -1;

    while(j < fin)
    {
        if((str[j] == '*' || str[j] == '/') && topB == -1)
            break;
        if(str[j] == '(')
            ++topB;
        else if(str[j] == ')')
            --topB;
        ++j;
    }

    if(j == fin)
        return fact(sta, fin);
    else if((str[j] == '*' || str[j] == '/') && fact(sta, j - 1))
        return fact(j + 1, fin);
    else
        return false;
}

bool fact(int sta, int fin)
{
    if(id(sta) && sta == fin)
        return true;
    else if(num(sta) && sta == fin)
        return true;
    else if(str[sta] == '(' && str[fin] == ')')
        return expn(sta + 1, fin - 1);
    else
        return false;
}

bool id(int i)
{
    if(str[i] == 'a' || str[i] == 'b' || str[i] == 'c' || str[i] == 'd' || str[i] == 'e')
        return true;
    return false;
}

bool num(int i)
{
    if(isdigit(str[i]))
        return true;
    return false;
}

void step5()
{
    printf("\n------------------------------------------------------------------------------\n");
    printf("Step 05: Use of CFGs for Parsing \n");
    printf("------------------------------------------------------------------------------\n");
    char *filename = "step01.txt";
    FILE *fp = fopen(filename, "r");
    if (fp == NULL)
    {
        printf("Error: could not open file %s", filename);
        return ;
    }
    // reading line by line, max 256 bytes
    const unsigned MAX_LENGTH = 256;
    char buffer[MAX_LENGTH];

    while (fgets(buffer, MAX_LENGTH, fp))
    {
        int len = strlen(buffer);
        if(stat(0, len - 1))
            printf("Accepted by Grammar.\n");
        else
            printf("Rejected by Grammar.\n");
    }
    fclose(fp);
}

//Step 5 Ends Here

// Main function
int main()
{
    step0();
    step1();
    step2();
    step3();
    step4();
    step5();
    return 0;
}
