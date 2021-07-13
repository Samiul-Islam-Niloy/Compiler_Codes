#include <stdio.h>
int main()
{
    FILE *p1, *p2;
    char c;

    p1 = fopen("input1.c", "r");
    p2 = fopen("output.txt", "w");

    while((c= getc(p1))!=EOF)
    {
        if(c=='/')
        {
            if((c=getc(p1))=='*' )
            {
                do
                {
                    c=getc(p1);
                }
                while(c!='*');

                c=getc(p1);
                if(c=='/'){
                    c=getc(p1);
                    }
                if(c=='\n'){
                    c=getc(p1);
                    }
            }
            else
            {
                if(c=='/')
                {
                    do
                    {
                        c=getc(p1);
                    }
                    while(c!=10);
                    c=getc(p1);
                }
            }
        }
        if(c !=32)
           if(c!=10)
        putc(c,p2);

    }
    fclose(p1);
    fclose(p2);

    p2 = fopen("output.txt", "r");
    while ((c= fgetc(p2))!=EOF)
        printf(("%c"),c);
    fclose(p2);

    return 0;
}