/*
Ahsanullah University of Science and Technology
Department of Computer Science and Engineering
Lab Final Examination
Course: CSE 4130 (Formal Languages and Compilers Lab)
Year/Sems: 4/1                Session: FALL 2020
ID: 170204016                 Set: 2
*/
#include <bits/stdc++.h>
using namespace std;
#define fi first
#define se second
#define pb push_back
#define pi pair<int,int>
#define pii pair<int,char>
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
map<int,info>table;
map<pair<string,string>,int >mp;
map<int,pair<string,string> >rmp;
map<string,bool>ids;
set<pi>errors;
void Display()
{
    cout<<"Sl No.  Name  Id Type  Data type Scope Value"<<endl;
    for(auto it:table)
    {
        cout<<it.fi<<" "<<it.se.name<<" "<<it.se.idtype<<" "<<it.se.datatype<<" "<<it.se.scope<<" "<<it.se.value<<endl;
    }
}
void Insert(string name,string idtype,string datatype,string scope,string value)
{
    int id=table.size()+1;
    table[id]={name,idtype,datatype,scope,value};
    mp[{name,scope}]=id;
    rmp[id]={name,scope};
}
void Update(int id,string name,string idtype,string datatype,string scope,string value)
{
    table[id]={name,idtype,datatype,scope,value};

}
void Delete(string Scope)
{
    vector<int>delids;
    map<int,info>::iterator it;
    for(it=table.begin();it!=table.end();it++)
    {
        if(it->se.scope==Scope)
        {
            delids.pb(it->fi);
        }
    }
    for(auto id:delids)
    {
        it=table.find(id);
        table.erase(it);
    }
}
bool Search(string name,string scope)
{
    if(mp.find({name,scope})==mp.end() && mp.find({name,scope})==mp.end())
    {
        return false;
    }
    return true;
}
string delbrac(string a)
{
    return a.substr(1,a.size()-2);
}
int sep(char c)
{
    return c==' ' || c==',' || c==';' || c=='\'' || c=='>' || c=='<';
}
int op(char c)
{
    return c=='+' || c=='-' || c=='*' || c=='/' || c=='%' || c=='=';
}
int par(char c)
{
    return c=='(' || c==')' || c=='{' || c=='}';
}
int doubleop(char c,char d)
{
    if(c=='>' && d=='=')
        return true;

    else if(c=='<' && d=='=')
        return true;

    else if(c=='+' && d=='+')
        return true;

    else if(c=='-' && d=='-')
        return true;

    else if(c=='+' && d=='=')
        return true;

    else if(c=='-' && d=='=')
        return true;

    else if(c=='*' && d=='=')
        return true;

    else if(c=='/' && d=='=')
        return true;

    else if(c=='%' && d=='=')
        return true;

    else if(c=='=' && d=='=')
        return true;

    return false;
}
int num(char c)
{
    return c>='0' && c<='9';
}
int integer(string a)
{
    int i,neg=0;
    for(i=0;i<a.size();i++)
    {
        if(a[i]=='-')
        {
            neg++;
            continue;
        }
        if(num(a[i])==0)
            return false;
    }
    if(neg>1 || (neg==1 && a[0]!='-') || (neg==1 && a.size()<2))
        return false;
    return true;
}
int points(string a)
{
    int i,point=0;
    for(i=0;i<a.size();i++)
    {
        if(a[i]=='.')
        {
            point++;
        }
    }
    return point;
}
int floatnum(string a)
{
    int i,point=points(a),flag=1,len=a.size();
    for(i=0;i<len;i++)
    {
        if(a[i]=='.')
            continue;
        if(num(a[i])==0)
            flag=0;
    }
    if(point!=1 || flag==0 || a[0]=='.' || a[len-1]=='.')
        return false;
    return true;
}
bool isnum(string s)
{
    return floatnum(s) || integer(s);
}
int keyword(string a)
{
    if(a.size()==1)
        return false;
    vector<string> keywords={"if","int","char","else","float","double","return"};
    for(int i=0;i<keywords.size();i++)
    {
        if(a==keywords[i])
            return true;
    }
    return false;
}
string func(string a)
{
    string temp="";
    int p=points(a);
    if(p)
    {
        if(floatnum(a))
        {
            temp="[num "+a+"] ";
            return temp;
        }
        else
        {
            temp="[unkn "+a+"] ";
            return temp;
        }
    }
    if(integer(a))
    {
        temp="[num "+a+"] ";
        return temp;
    }
    if(a.size()==1)
    {
        if(op(a[0]))
        {
            temp="[op "+a+"] ";
            return temp;
        }
        if(par(a[0]))
        {
            temp="[par "+a+"] ";
            return temp;
        }
        if(sep(a[0]))
        {
            temp="[sep "+a+"] ";
            return temp;
        }
    }
    if(a.size()==2)
    {
        if(doubleop(a[0],a[1]))
        {
            temp="[op "+a+"] ";
            return temp;
        }
    }
    if(keyword(a))
    {
        temp="[kw "+a+"] ";
        return temp;
    }
    temp="[id "+a+"] ";
    return temp;
}
void step1()
{
    ifstream infile ("input.txt");
    ofstream outfile1;
    outfile1.open("stepone.txt");
    bool multiline=false;
    if(infile.is_open())
    {
        string a,s,t="";
        while(getline(infile,s))
        {
            stringstream ss(s);
            bool singlecomment=false;
            while(ss>>a)
            {
                if(a.size()>=2)
                {
                    if(a.substr(0,2)=="//" && !multiline)
                    {
                        singlecomment=true;
                    }
                    if(a.substr(0,2)=="/*")
                    {
                        multiline=true;
                    }
                    else if(a.substr(a.size()-2,2)=="*/")
                    {
                        multiline=false;
                    }
                    else
                    {
                        if(!multiline && !singlecomment)
                        {
                            t+=a+" ";
                        }
                    }
                }
                else
                {
                    if(!multiline && !singlecomment)
                    {
                        t+=a+" ";
                    }
                }
            }
            t+='\n';
            singlecomment=false;
        }
        outfile1<<t;
        infile.close();
    }
    outfile1.close();
}
void step2()
{
    ifstream infile ("stepone.txt");
    ofstream outfile1;
    outfile1.open("steptwo_1.txt");
    if(infile.is_open())
    {
        string s;
        while(getline(infile,s))
        {
            int i=0,j=0,len=s.size();
            string t="";
            while(i<len)
            {
                if(sep(s[i]) || op(s[i]) || par(s[i]))
                {
                    if(!t.empty() && t[t.size()-1]!=' ')
                        t+=' ';
                    if(s[i]!=' ')
                    {
                        int flag=0;
                        if(doubleop(s[i],s[i+1]))
                        {
                            flag=1;
                        }
                        if(s[i]=='-' && num(s[i+1]))
                        {
                            t+=s[i++];
                            continue;
                        }
                        if(flag)
                        {
                            t+=s[i++];
                            t+=s[i++];
                            t+=' ';
                        }
                        else
                        {
                            t+=s[i++];
                            t+=' ';
                        }
                    }
                    else
                       i++;
                }
                else
                {
                    t+=s[i++];
                }
            }
            t+='\n';
            outfile1<<t;
            i=0;
        }
        infile.close();
    }
    outfile1.close();
    ifstream infile2 ("steptwo_1.txt");
    ofstream outfile2;
    outfile2.open("steptwo_2.txt");
    if(infile2.is_open())
    {
        string s,a;
        while(getline(infile2,s))
        {
            stringstream ss(s);
            while(ss>>a)
            {
                string t=func(a);
                outfile2<<t;
            }
        }
        infile2.close();
    }
    outfile2.close();
}
void step3()
{
    ids["int"]=true;
    ids["float"]=true;
    ids["double"]=true;
    ids["char"]=true;
    string a,b,s;
    ifstream infile ("steptwo_2.txt");
    ofstream outfile1;
    outfile1.open("stepthree.txt");
    if(infile.is_open())
    {
        while(getline(infile,s))
        {
            stringstream ss(s);
            while(ss>>a>>b)
            {
                string temp="";
                if(a=="[id")
                    temp=a+" "+b;
                else
                    temp="["+b;
                outfile1<<temp<<" ";
            }
        }
        infile.close();
    }
    outfile1.close();
    ifstream infile2("stepthree.txt");
    string block="global";
    bool blockopen=false;
    string lastname="",lastdatatype="";
    vector<string>vec;
    if(infile2.is_open())
    {
        while(getline(infile2,s))
        {
            stringstream ss(s);
            while(ss>>a)
            {
                vec.pb(a);
            }
        }
    }
    infile2.close();
    int i=0,len=vec.size();
    while(i<len)
    {
        a=vec[i];
        if(a=="[id")
        {
            b=vec[++i];
            b.pop_back();
            lastname=b;
            string idtype="var";
            if(delbrac(vec[i+1])=="(")
                idtype="func";
            if(!isnum(b))
                b="";
            if(!Search(lastname,block))
            {
                Insert(lastname,idtype,lastdatatype,block,b);
            }
        }
        else
        {
            string temp=delbrac(a);
            if(temp=="(")
            {
                if(block=="global")
                {
                    blockopen=true;
                    block=lastname;
                }
            }
            else if(temp=="}")
            {
                blockopen=false;
                block="global";
            }
            else if(temp=="=")
            {
                b=vec[++i];
                b=delbrac(b);
                if(!isnum(b))
                    b="";
                if(!Search(lastname,block))
                {
                    string idtype="var";
                    Insert(lastname,idtype,lastdatatype,block,b);
                }
                else
                {
                    int id=mp[{lastname,block}];
                    string idtype="var";
                    Update(id,lastname,idtype,lastdatatype,block,b);
                }
            }
            else
            {
                if(ids.find(temp)!=ids.end())
                {
                    lastdatatype=temp;
                }
            }
        }
        i++;
    }
}
void step4()
{
    int cnt=0;
    ifstream infile2 ("steptwo_1.txt");
    if(infile2.is_open())
    {
        string s;
        int ifcnt=0;
        while(getline(infile2,s))
        {
            cnt++;

            if(!s.empty())
                s.pop_back();
            if(s.empty())
                continue;

            stringstream ss(s);
            string t,prev;

            ss>>prev;
            if(prev=="if")
                ifcnt++;
            if(prev=="else")
            {
                if(ifcnt>0)
                    ifcnt--;
                else
                {
                    errors.insert({cnt,5});
                }
            }
            while(ss>>t)
            {

                if(t=="if")
                    ifcnt++;
                if(t=="}")
                    ifcnt=0;
                if(t=="else")
                {
                    if(ifcnt>0)
                        ifcnt--;
                    else
                    {
                        errors.insert({cnt,5});
                    }
                }
                prev=t;
            }
        }
    }
    infile2.close();
    for(auto e:errors)
    {
        cout<<"Unmatched else at line " <<e.fi<<endl;
    }
}
int main()
{
    step1();
    step2();
    step3();
    step4();
}
