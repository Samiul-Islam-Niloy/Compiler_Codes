#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

void printInput()
{
    FILE *fp1;
    char ch;
    fp1 = fopen("input.txt", "r");
    if (fp1 == NULL)
    {
        printf("error while opening the file\n");
        exit(0);
    }
    printf("Sample Input:\n");
    while ((ch = fgetc(fp1)) != EOF)
    {
        printf("%c",ch);
    }
    fclose(fp1);
}

void separateLexemes()
{
    FILE *fp1, *fp2;
    fp1 = fopen("input.txt", "r");
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

int main()
{
    printInput();
    separateLexemes();
    printFirstStep();
    int e = processSecondStep();
    printSecondStep(e);
    return 0;
}
