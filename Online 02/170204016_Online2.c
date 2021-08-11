/*
                  Ahsanullah University of Science and Technology
                  Department of Computer Science and Engineering
                  CSE 4130 (FALL 2020)            Exam: Online 2
                  Set: 2                           ID: 170204016
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

int identifyPattern(char *lex)
{
    int i, l, s;
    i = 0;
    if ((lex[i])>= 'a' && (lex[i])<= 'z')
    {
        s = 1;
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
            else if (lex[i] == '_')
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
        if ((lex[i])=='S')
        {
            s = 1;
            i++;
        }
        else
            s = 0;
    if (s == 1)
        if ((lex[i])=='B')
        {
            s = 1;
            i++;
        }
        else
            s = 0;
    if (s == 1)
        if (isdigit(lex[i]))
        {
            s = 1;
        }
        else
        {
            s = 0;
        }
    return s;
}

void processPattern()
{
    char buffer[20];
    printf("Input String: ");
    gets(buffer);
    if (identifyPattern(buffer) == 1)
    {
        printf("String follows the pattern.");

    }
    else
    {
        printf("String does not follow the pattern.");
    }
}

int main()
{
    processPattern();
}
