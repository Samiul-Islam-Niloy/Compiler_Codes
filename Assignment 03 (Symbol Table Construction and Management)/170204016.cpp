#include <bits/stdc++.h>
using namespace std;

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
    symboltable[id]={name,idtype,datatype,scope,value};
    scopemap[{name,scope}]=id;
    scopemaprev[id]={name,scope};
}

void Update(int id,string name,string idtype,string datatype,string scope,string value)
{
    symboltable[id]={name,idtype,datatype,scope,value};

}

void Delete(string Scope)
{
    vector<int>delids;
    map<int,info>::iterator it;
    for(it=symboltable.begin();it!=symboltable.end();it++)
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
    for(i=0;i<value.size();i++)
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
    for(i=0;i<value.size();i++)
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
    for(i=0;i<len;i++)
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

int main()
{
    TypeDetect["int"]=true;
    TypeDetect["float"]=true;
    TypeDetect["double"]=true;
    TypeDetect["char"]=true;

    string TokenName,Token,word;

    ifstream inputfile ("input.txt");
    ofstream outputfile;
    outputfile.open("output.txt");

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
    ifstream inputfile2("output.txt");
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
                    int id=scopemap[{lastname,block}];
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
    ifstream inputfile3("output.txt");
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
                        if(Token.compare(it.second.name)==0){
                            cout<<it.first <<"] ";
                            outputfile2<<it.first<<"] ";
                            er=it.first;
                            break;
                        }
                     }
                }
                else if(TokenName=="[}]"){
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
