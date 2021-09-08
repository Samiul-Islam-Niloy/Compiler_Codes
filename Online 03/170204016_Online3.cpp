 /*
                  Ahsanullah University of Science and Technology
                  Department of Computer Science and Engineering
                  CSE 4130 (FALL 2020)            Exam: Online 3
                  Set: 2                          ID: 170204016
*/

#include<bits/stdc++.h>
using namespace std;
struct data{
    int no;
    char name[100];
    char type[100];
}strctd[100];


int size = 0;

void delete_row(int id)
{
    int i;
    int flag = 0;
    for(i=0;i<size;i++)
    {
        if(strctd[i].no==id)
        {
            flag = 1;
            continue;
        }
        if(flag==1)
        {
            strctd[i-1]  = strctd[i];
            strctd[i-1].no--;
        }

    }
    size--;
}

void display()
{
    int i=0;
    for(i=0;i<size;i++)
    {
        printf("%d %s %s\n",strctd[i].no,strctd[i].name,strctd[i].type);
    }
}
int main()
{
    FILE* fp = fopen("symbolTable.txt","r");
    int i=0;
    while(fscanf(fp,"%d %s %s",&strctd[i].no,strctd[i].name,strctd[i].type)!=EOF)
    {
        i++;
    }
    size = i;
    int id;
    printf("Delete Row No: ");
    scanf("%d",&id);
    delete_row(id);
    printf("\nThe updated Symbol Table:\n\n");
    display();

}
