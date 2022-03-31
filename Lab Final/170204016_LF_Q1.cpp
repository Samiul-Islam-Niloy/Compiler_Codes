/*
Ahsanullah University of Science and Technology
Department of Computer Science and Engineering
Lab Final Examination
Course: CSE 4130 (Formal Languages and Compilers Lab)
Year/Sems: 4/1                Session: FALL 2020
ID: 170204016                 Set: 2
*/
#include<stdio.h>
#include<string.h>

char array[10000];
int line  =1;
int main()
{
    FILE *fp = fopen("input.c","r");
    int f1 = 0,f2 = 0,f3=0,i,f0=0;
    while(fgets(array,1000,fp))
    {

        int cnt = 0;
        f1 = 0;
        for(i=0;i<strlen(array);i++)
        {
            if(array[i]==34)
            {
                if(f3==1)
                    f3 = 0;
                else
                    f3 = 1;
            }
            else if(array[i]=='\'')
            {
                if(f3==1 || f1==1)
                    continue;
                if(f2==1)
                    f2 = 0;
                else
                    f2 = 1;
            }
            else if(array[i]=='\\')
            {
                if(f3==1)
                    continue;
                if(i+1<strlen(array) && array[i+1]=='\\')
                {
                    f1 = 1;
                    i++;
                }
                else if(i+1<strlen(array) && array[i+1]=='*')
                {
                    f0 = 1;
                    i++;
                }
            }
            else if(array[i]=='*')
            {
                if(i+1<strlen(array) && array[i+1]=='\\')
                {
                    f1 = 0;
                    i++;
                }
                else
                {
                    cnt++;
                }
            }
            else if(array[i]=='+' || array[i]=='-' || array[i]=='/')
            {
                cnt++;
            }

        }
        if(cnt>0)
            printf("Line No. %d: %d\n",line,cnt);
        line++;
    }
}
