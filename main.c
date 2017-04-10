#include <stdio.h>
#include <stdlib.h>
#include <strings.h>
#include <math.h>
//#include <ctype.h>
#include <limits.h>
#define STACKSIZE 50
#define TYPE double

typedef struct
{
    char name[50];
    double value;

} parameter;

typedef struct
{
    parameter fpar[50];
    char fname[50];
    char fcode[50];
    int countpar;


} fn;
fn fns[100];
int counter=0;

int priority(char c);
TYPE eval(char *str);
void print_postfix(char *str,char *post);
char* assignment(char*str);
int Delete(char *del);
void Del(int n);
int search(char *key);
void calc(char *str);
TYPE Excfn(char *name,char* par);
void savefn(char* name,char* par,char* code);
int findfn(char* name);


double ans;

typedef struct
{
    TYPE data;
    char key[30];
    struct Node *next;
} Node;

Node * newNode(TYPE value,char *key)
{
    Node *n=malloc(sizeof(Node));
    n->data=value;
    strcpy(n->key,key);
    n->next=NULL;
    return n;
}

typedef struct
{
    Node *head, *tail;
} LinkedList;

void initLinkedList(LinkedList*ll)
{
    ll->head=NULL;
    ll->tail=NULL;
}

void printLinkedList(Node *n)
{
    while (n)
    {
        printf("%s=%lf\n",n->key,n->data);
        n=n->next;
    }
}

void addHead(LinkedList*ll,TYPE value,char *key)
{
    Node *n=newNode(value,key);
    n->next=ll->head;
    ll->head=n;
    if(!ll->tail) ll->tail=n;
}

void addTail(LinkedList*ll,TYPE value,char *key)
{
    Node *n=newNode(value,key);
    Node *tail=ll->tail;
    ll->tail=n;
    if (tail) tail->next=n;
    else ll->head=n;
}

LinkedList ll;

void tostring (char *outstr,TYPE val)
{
    sprintf(outstr,"%g",val);
}

typedef struct
{
    int top;
    double items[STACKSIZE];
} Stack;

void initialize(Stack *s)
{
    s->top=0;
}

void push(Stack *s,TYPE value)
{
    s->items[s->top++]=value;
}

TYPE pop(Stack *s)
{
    return s->items[--s->top];
}

int isfull(Stack *s)
{
    return s->top<STACKSIZE?0:1 ;
}

int isempty(Stack *s)
{
    return s->top==0?1:0;
}

int priority(char c)
{
    if (c>='0' && c<= '9') return 0;
    switch (c)
    {
    case '.' :
        return 0;
    case '+' :
    case '-' :
        return 1;
    case '*' :
    case '%' :
    case '/' :
        return 2;

    case '(' :
        return 3;

    case ')' :
        return -1;
    case '<' :
        return -3;

    default :
        return -2;
    }
}

TYPE peep( Stack *s)
{
    double x=pop(s);
    push(s,x);
    return x;
}

TYPE findvalue(char *key)
{
    Node *n=(&ll)->head;
    while (n)
    {
        if(strcmp(n->key,key)==0) return n->data;
        n=n->next;
    }
    return -999999.999999;

}

void copyn(char*from,char*to,char*out)
{
    int i=0;
    while(from<=to)
    {
        out[i]=*from++;
        i++;
    }
    out[i]='\0';
}


TYPE eval(char *str)
{
    char ch;
    Stack stack,*data=&stack;
    initialize(data);
    double y;
    char num[30];
    double x;
    double res;
    if (strcmp(str,"-999999.999999")==0)return -999999.999999;
    while (ch=*str++)
    {
        if (priority(ch)==0)
        {
            int i=0;
            while (priority(*(str+i))==0)i++;
            copyn(str-1,str+i-1,num);
            x=strtod(num,NULL);
            push(data,x);
            str=str+i;
        }
        else
        {
            switch(ch)
            {
            case ' ':
                continue;
            case '+':
                if(isempty(data))
                {
                    printf("Error Invalid Expression\n");
                    return -999999.999999;
                }
                res= pop(data);
                if(isempty(data))
                {
                    printf("Error Invalid Expression\n");
                    return -999999.999999;
                }
                res+=pop(data);

                break;
            case '-':
                if(isempty(data))
                {
                    printf("Error Invalid Expression\n");
                    return -999999.999999;
                }
                res= pop(data);
                if(isempty(data))
                {
                    printf("Error Invalid Expression\n");
                    return -999999.999999;
                }
                res-=pop(data);
                res*=-1;

                break;
            case '%':
                if(isempty(data))
                {
                    printf("Error Invalid Expression\n");
                    return -999999.999999;
                }
                res= pop(data);
                if(isempty(data))
                {
                    printf("Error Invalid Expression\n");
                    return -999999.999999;
                }
                res=fmod(pop(data),res);
                break;
            case '*':
                if(isempty(data))
                {
                    printf("Error Invalid Expression\n");
                    return -999999.999999;
                }
                res= pop(data);
                if(isempty(data))
                {
                    printf("Error Invalid Expression\n");
                    return -999999.999999;
                }
                res*=pop(data);
                break;
            case '/':
                if(isempty(data))
                {
                    printf("Error Invalid Expression\n");
                    return -999999.999999;
                }
                res=pop(data);
                if(isempty(data))
                {
                    printf("Error Invalid Expression\n");
                    return -999999.999999;
                }
                res/=pop(data);
                res=1/res;
                break;
            }
            push(data,res);
        }
    }
    y=pop(data);
    if(!isempty(data))
    {
        printf("Error Unvalid Expression\n");
        return -999999.999999;
    }

    return y;
}


void print_postfix(char *str,char *post)
{

    char ch,out[30],buffer[30];
    Stack stack, *s=&stack;
    initialize(s);
    int i,j=0;
    str[strlen(str)]=0;
    while(ch=*str)
    {

        if(priority(ch)==0)
        {

            i=1;
            post[j++]=*(str);

            while (priority(*(str+i))==0)
            {

                post[j++]=*(str+i);

                i++;


            }
            str=str+i-1;
            post[j++]=' ';

        }
        else if(priority(ch)==-2)
        {
            i=1;

            while (str[i] && priority(*(str+i))!=1&&priority(*(str+i))!=2&&priority(*(str+i))!=3&&priority(*(str+i))!=-1&&priority(*(str+i))!=-3)
            {
                i++;
            }

            if(priority(*(str+i))==-3)
            {

                char* start=strchr(str,'<')+1;
                char* end=strchr(str,'>')-1;
                char name[50],par[50];
                copyn(start,end,par);
                copyn(str,start-2,name);
                str=end+1;



                tostring(buffer,Excfn(name,par));

                int k;
                for(k=0; k<strlen(buffer); k++)
                {
                    post[j++]=buffer[k];
                }
                post[j++]=' ';


            }


            else

            {

                copyn(str,str+i-1,out);
                str=str+i-1;
                double z=findvalue(out);
                if(z==-999999.999999)
                {
                    strcpy(post,"-999999.999999");
                    printf("Error unassigned variable\n");
                    return;
                }
                tostring(buffer,z);
                int k;
                for(k=0; k<strlen(buffer); k++)
                {
                    post[j++]=buffer[k];
                }
                post[j++]=' ';
            }

        }


        else if (ch==')')
        {
            while(!isempty(s) && peep(s)!='(')
            {
                post[j++]=pop(s);
                post[j++]=' ';
            }
            pop(s);
        }

        else
        {

            while(!isempty(s) && priority(peep(s))>=priority(ch) && peep(s)!='(')
            {

                char x=pop(s);
                if (x!='(')
                {
                    post[j++]=x;
                    post[j++]=' ';
                }
            }
            push(s,ch);
        }

        str++;

    }

    while(!isempty(s))
    {
        post[j++]=pop(s);
        post[j++]=' ';
    }
    post[j++]=0;


}

char* assignment(char*str)
{
    while(*str!='='&&*str)str++;
    if(*str==0)return NULL;
    return str;
}

int Delete(char *del)
{
    Node *n=(&ll)->head,*d;

    if(isempty(&ll))return 1;

    if(strcmp(n->key,del)==0)
    {

        d=n->next;
        if(d==0)
        {
            (&ll)->head=(&ll)->tail=NULL;
            free(d);
            return 0;
        }
        (&ll)->head=d;
        free(n);

        return 0;
    }




    while (n->next)
    {
        d=n->next;

        if(strcmp(d->key,del)==0)
        {
            if((&ll)->tail==n->next)
            {
                (&ll)->tail=n;
                free(d);
                return 0;
            }
            n->next=d->next;
            free(d);
            return 0;
        }
        n=n->next;
    }
    return 1;


}

void Del(int n)
{
    int i;
    for(i=0;i<n;i++)
    {
        (&ll)->head=(&ll)->head->next;
    }
}

int search(char *key)
{
    Node *n=(&ll)->head;
    while (n)
    {
        if(strcmp(n->key,key)==0)return 0;
        n=n->next;
    }
    return 1;


}


void calc(char *str)
{
    char post[30],key[30],b[30];
    char *ch=assignment(str);
    double x;
    int y;
    if(ch==NULL)
    {
        print_postfix(str,post);
        x=eval(post);
        if(x==-999999.999999)return;
        ans=x;
        if(ans==-0)ans=0;
        printf("ans=%lf\n",ans);


    }
    else
    {
        copyn(str,ch-1,key);

        char* ret=strstr(key,"fn");

        if (ret)
        {

            char* start=strchr(key,'<')+1;
            char* end=strchr(key,'>')-1;
            char name[50],par[50];
            copyn(start,end,par);
            copyn(ret+2,start-2,name);
            savefn(name,par,ch+1);
            return;

        }



        if(!priority(*key))
        {
            printf("Error invalid variable\n");
            return;
        }
        if(strchr(key,40)||strchr(key,41)||strchr(key,42)||strchr(key,43)||strchr(key,45)||strchr(key,47))
        {
            printf("Error invalid variable\n");
            return;
        }


        print_postfix(ch+1,post);
        x=eval(post);
        if(x==-999999.999999)return;
        y=search(key);
        if(y)
        {
            addHead(&ll,x,key);
            ans=x;
            if(ans==-0)ans=0;
            printf("%s=%lf\n",key,ans);
        }
        else
        {
            printf("The Variable is already registered \ntype Y to replace it or N to leave it:");
            scanf("%s",b);
            if(strcmp(b,"Y")==0)
            {
                Delete(key);
                addHead(&ll,x,key);
                ans=x;
                if(ans==-0)ans=0;
                printf("%s=%lf\n",key,ans);


            }

        }



    }

}


TYPE Excfn(char *name,char* par)
{

    int j=0;
    int i=findfn(name);
    double z;
    const char s[2]=",",s2[2]=";";
    char *token,out[50];
    token = strtok(par,s);
    if(priority(*token)==0)fns[i].fpar[j++].value=strtod(token,NULL);
    else  fns[i].fpar[j++].value=findvalue(token);


    while( token != NULL )
    {
        token = strtok(NULL,s);
        if(token==NULL)break;
        if(priority(*token)==0)fns[i].fpar[j++].value=strtod(token,NULL);
        else  fns[i].fpar[j++].value=findvalue(token);

    }





    for(j=0; j<fns[i].countpar; j++)
    {
        addHead(&ll,fns[i].fpar[j].value,fns[i].fpar[j].name);
    }
    token = strtok(fns[i].fcode,s2);

    print_postfix(token,out);
    z=eval(out);
    while( token != NULL )
    {
        token = strtok(NULL,s2);
        if(token==NULL)break;
        print_postfix(token,out);
        z=eval(out);


    }
      Del(fns[i].countpar);

    return z;

}


void savefn(char* name,char* par,char* code)
{
    strcpy(fns[counter].fname,name);
    strcpy(fns[counter].fcode,code);
    fns[counter].countpar=0;

    const char s[2]=",";
    char *token;
    token = strtok(par,s);
    strcpy(fns[counter].fpar[fns[counter].countpar++].name,token);
    while( token != NULL )
    {
        token = strtok(NULL,s);
        if(token==NULL)
        {
            counter++;
            return;
        }
        strcpy(fns[counter].fpar[fns[counter].countpar++].name,token);

    }


}

int findfn(char* name)
{
    int i;
    for(i=0; i<=counter; i++)
    {
        if(strcmp(fns[i].fname,name)==0)return i;
    }
    return -1;

}




int main()
{
    char qu[50]="x+y+z;",v[50]="bibo",n[20]="x,y,z",yi[50]="1,2,3" , hg[50]="bibo<1,2,3>",fo[50];
    //savefn(v,n,qu);
    //double hkj=5.5;
    //fns[0].fpar[0].value=hkj;
    //printf("%lf",fns[0].fpar[0].value);
    //Excfn(v,yi);
    //print_postfix(hg,fo);
    //printf("\n\n%lf\n",eval(fo));




    int x=1;
    int y=1;
    int z;
    char str[100];
    char postfix[100];
    char del[100];
    initLinkedList(&ll);

    while(x)
    {
        y=1;
        printf(">>");
        str[0]='\0';
        postfix[0]='\0';
        del[0]='\0';
        scanf("%s",str);
        str[strlen(str)]=0;
        if (strcmp(str,"quit")==0)break;
        if (strcmp(str,"clc")==0)
        {
            system("cls");
            y=0;
        }
        if (strcmp(str,"ans")==0)
        {
            printf("ans=%lf\n",ans);;
            y=0;
        }
        if (strcmp(str,"delete")==0)
        {
            printf("Enter The Varible Name:");
            scanf("%s",del);
            z=Delete(del);
            if(z)printf("Error Not Listed Variable\n");
            else printf("%s has been successfully deleted\n");
            y=0;

        }
        if (strcmp(str,"print")==0)
        {
            printLinkedList((&ll)->head);
            y=0;
        }
        if(y)
        {
            calc(str);
        }



    }
    return 0;
}
