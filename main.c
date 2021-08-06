#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAXSIZE 100
#include <math.h>
///****stack****
///data type for the stack to handle operations and numbers in the expression
typedef union
{
    float fData;
    char  cData;
} Item;

///stack struct
typedef struct
{
    Item items[MAXSIZE];
    int top;
} Stack;

///creating a stack
Stack * initialize()
{
    Stack * s= malloc(sizeof(Stack));
    s->top=0;
    return s;
}

///checking if the stack is empty
int isEmpty(Stack *s)
{

    return (s->top==0);
}
///returns the top of the stack
Item top(Stack *s)
{
    if(!isEmpty(s))
        return s->items[s->top-1];

}
///popping an item out of the stack
Item pop(Stack *s)
{
    if(!isEmpty(s))
        return s->items[--s->top];

}
///pushing an item in the stack
void push(Stack *s, Item val)
{

    if(s->top<MAXSIZE+1)
    {
        s->items[s->top]=val;
        s->top++;
    }
    else
    {
        printf("Overflow");

    }

}
/*
*
*/
///****trees****
///node struct for the tree and heap
typedef struct
{
    char *key;
    float value;
    struct Node *left,*right;
} Node;

///creating a new node
Node * newNode(char *keyIn,float valueIn)
{
    Node* n=malloc(sizeof(Node));
    n->key=malloc(30*sizeof(char));
    strcpy(n->key,keyIn);
    n->value=valueIn;
    n->left=NULL;
    n->right=NULL;
    return n;
}

///printing a tree in order according to the variable's names
void InOrdertraverse(Node *n)
{
    if (n!=NULL)
    {
        InOrdertraverse(n->left);
        printf("%s = %g\n",n->key,n->value);
        InOrdertraverse(n->right);
    }
}
///searching for the node in the tree by the variable name
Node* search(Node *n,char * data)
{
    if (n==NULL||strcasecmp(data,n->key)==0)
        return n;
    else if(strcasecmp(data,n->key)<0)
    {
        return search(n->left,data);
    }
    else
    {
        return search(n->right,data);
    }
}
///inserting a node in the tree
Node* insert(Node* node, char * keyIn,float valueIn)
{

    if (node == NULL)
        return newNode(keyIn,valueIn);
    if(search(node,keyIn)==NULL)
    {

        if (strcasecmp(keyIn,node->key)<0)
            node->left = insert(node->left, keyIn,valueIn);
        else if (strcasecmp(keyIn,node->key)>0)
            node->right = insert(node->right, keyIn,valueIn);
    }
    else

    {
        search(node,keyIn)->value=valueIn;
    }

    return node;
}
/*
*
*/
///****heap*****
///heap struct
typedef struct
{
    Node array[MAXSIZE];
    int Current;
} Heap;

///swapping data of 2 nodes
void swap(Node *node1,Node *node2)
{
    Node temp=*node1;
    *node1=*node2;
    *node2=temp;
}

///inserting a node in the heap
Heap *insertHeap(Heap *heap,float value,char *key)
{
    int lastIndex=heap->Current; //save pointer to the last index of the array

    if(heap->Current==MAXSIZE-1)
        printf("Overflow \n");

    else
    {
        //adding the data to the last index in the array
        heap->array[heap->Current].value=value;
        heap->array[heap->Current].key=malloc(sizeof(30));
        strcpy(heap->array[heap->Current].key,key);

        //identifying its parent node and compare it
        int parentIndex=floor(((heap->Current)-1)/2);

        //sift up process
        while(heap->array[parentIndex].value<heap->array[heap->Current].value)
        {
            swap(&heap->array[parentIndex],&heap->array[heap->Current]);

            heap->Current=parentIndex;
            parentIndex=floor(((heap->Current)-1)/2);
        }

    }

    heap->Current=lastIndex+1;

    return heap;
}

///printing the heap
void printHeap(Heap *heap)
{
    int i;
    for(i=0; i<heap->Current; i++)
        printf("%s = %g\n",heap->array[i].key,heap->array[i].value);
}

Heap *heapify(Heap *heap,int n,int i)
{
    int largest=i;
    int left=2*i+1;
    int right=2*i+2;

    //to get the largest value at the top root

    if(heap->array[left].value>heap->array[largest].value && left<n)
        largest=left;
    if(heap->array[right].value>heap->array[largest].value &&right <n)
        largest=right;
    //means it has changed then swap
    if(largest!=i)
    {
        swap(&heap->array[i],&heap->array[largest]);
        heap=heapify(heap,n,largest);
    }
    return heap;
}

///for Max heap
void heapSort(Heap *heap)
{

    int i;

    for(i=(heap->Current)-1; i>0; i--)
    {

        swap(&heap->array[i],&heap->array[0]);

        heap=heapify(heap,i,0);
    }
    printHeap(heap);
}
/*
*
*/

///****logic****

///determines the precedence of operations
int percedence(char x)
{
    if(x=='^')
        return 1;
    else if(x=='*'||x=='/')
        return 2;
    else if(x=='+' || x=='-')
        return 3;
    else if(x=='(')
        return 4;
}
///converting the infix expression to postfix
void infixToPostfix(char * infix, char * postfix,Node *root)
{


    Item myitem;
    int i,j=0;
    postfix[0]='\0';
    char variable[30];
    variable[0]='\0';

    Stack * s=initialize();

    for(i=0; i<strlen(infix); i++)
    {
        static int x=0;

        //handling regular numbers
        if(isdigit(infix[i]))
        {
            if(isdigit(infix[i+1])|| infix[i+1]=='.')
            {
                postfix[j++]=infix[i];
                postfix[j]='\0';
            }

            else
            {

                postfix[j++]=infix[i];
                postfix[j++]=' ';
                postfix[j]='\0';
            }

        }
        //handling floating point
        else if(infix[i]=='.')
        {
            postfix[j++]=infix[i];
            postfix[j]='\0';
        }
        //handling negative sign
        else if((infix[i]=='-' && i==0 )|| (infix[i]=='-'&&infix[i-1]=='(') ||(infix[i]=='-'&&(!isdigit(infix[i-1])&&!isalpha(infix[i-1])&&infix[i-1]!=')')))
        {
            postfix[j++]=infix[i];
            postfix[j]='\0';

        }
        //handling brackets
        else if(infix[i]=='(')
        {
            myitem.cData=infix[i];
            push(s,myitem);
        }
        //handling operations
        else if(infix[i]=='^')
        {
            myitem.cData=infix[i];
            push(s,myitem);
        }
        else if(infix[i]=='*'|| infix[i]=='/')
        {
            if(isEmpty(s) || percedence(infix[i])<percedence(top(s).cData))
            {
                myitem.cData=infix[i];
                push(s,myitem);
            }
            else
            {
                while(percedence(infix[i])>=percedence(top(s).cData)&&!isEmpty(s))
                {

                    myitem=pop(s);
                    postfix[j++]=myitem.cData;
                    postfix[j++]=' ';
                    postfix[j]='\0';
                }
                myitem.cData=infix[i];
                push(s,myitem);
            }

        }
        else if(infix[i]=='+'|| infix[i]=='-')
        {
            if(isEmpty(s) || percedence(infix[i])<percedence(top(s).cData))
            {
                myitem.cData=infix[i];
                push(s,myitem);
            }
            else
            {
                while(percedence(infix[i])>=percedence(top(s).cData)&&!isEmpty(s))
                {
                    myitem=pop(s);
                    postfix[j++]=myitem.cData;
                    postfix[j++]=' ';
                    postfix[j]='\0';
                }
                myitem.cData=infix[i];
                push(s,myitem);
            }

        }
        //handling closing the bracket
        else if(infix[i]==')')
        {
            while(top(s).cData!='('&& !isEmpty(s))
            {
                myitem=pop(s);
                postfix[j++]=myitem.cData;
                postfix[j++]=' ';
                postfix[j]='\0';
            }
            pop(s);
        }
        //handling variables in the expression
        else if(isalpha(infix[i])!=0)
        {
            variable[x++]=infix[i];

            if(isalpha(infix[i+1])!=0)
                continue;

            variable[x]='\0';
            Node *n=search(root,variable);


            if(n!=NULL)
            {
                sprintf(variable, "%g", n->value);
                strcat(postfix,variable);
                j+=strlen(variable);
                postfix[j++]=' ';
                postfix[j]='\0';
                strcpy(variable,"");
                x=0;

            }
            else
            {
                postfix[j++]='0';
                postfix[j++]=' ';
                postfix[j]='\0';
                strcpy(variable,"");
                x=0;
            }



        }

    }


    while(!isEmpty(s))
    {
        postfix[j++]=pop(s).cData;
        postfix[j++]=' ';

    }
    //ending the postfix expression with null
    postfix[j]='\0';

}
///calculating the postfix expression value
float evaluatePostfix(char* postfix)
{
    Stack *s;
    char * token;
    Item myitem;
    float num1,num2,value,result;
    int i,check=0;
    token=strtok(postfix," ");

    myitem.fData=strtof(token,NULL);

    push(s,myitem);
    token= strtok(NULL," ");
    if(token!=NULL)
    {
        myitem.fData=strtof(token,NULL);
        push(s,myitem);
        token= strtok(NULL," ");
    }
    while(token!=NULL)
    {
        if(isdigit(token[0])||(token[0]=='-'&&strlen(token)>1))
        {
            myitem.fData=strtof(token,NULL);
            push(s,myitem);
        }

        else
        {
            switch(token[0])
            {
            case '+':
                num2=pop(s).fData;
                num1=pop(s).fData;
                value=num1+num2;
                myitem.fData=value;
                push(s,myitem);
                break;
            case '-':
                num2=pop(s).fData;
                num1=pop(s).fData;
                value=num1-num2;
                myitem.fData=value;
                push(s,myitem);
                break;
            case '*':
                num2=pop(s).fData;
                num1=pop(s).fData;
                value=num1*num2;
                myitem.fData=value;
                push(s,myitem);
                break;
            case '/':
                num2=pop(s).fData;
                num1=pop(s).fData;
                value=num1/num2;
                myitem.fData=value;
                push(s,myitem);
                break;
            case '^':
                num2=pop(s).fData;
                num1=pop(s).fData;
                value=pow(num1,num2);
                myitem.fData=value;
                push(s,myitem);
                break;
            }

        }
        token= strtok(NULL," ");

    }

    return pop(s).fData;

}

///checking if the key is valid
int validKey(char *key)
{
    int i;
    for(i=0; i<strlen(key); i++)
    {
        if(isalpha(key[i])==0 && key[i]==" ")
            return 0;
    }
    return 1;
}
///checking if the expression is valid
int valid(char *key,char *infix)
{
    int i;
    Stack *s=initialize();
    Item temp;
    //key validation
    for(i=0; i<strlen(key); i++)
    {
        if(isalpha(key[i])==0 && key[i]!=' ' && key[i]!='\n')
        {
            return 0;
        }
    }

    //parenthesis validation
    for(i=0; i<strlen(infix); i++)
    {
        if(infix[i]=='(')
        {
            temp.cData=infix[i];
            push(s,temp);
        }

        else if(infix[i]==')')
        {
            if(top(s).cData!='(')
                return 0;

            else
                temp=pop(s);
        }
    }
    if(!isEmpty(s))
        return 0;



    //Invalid symbol validation
    int x=strlen(infix);
    for(i=0; i<x; i++)
    {
        if(isalpha(infix[i]==0)) //check for variable
            if(!(infix[i] >= '0' && infix[i] <= '9')) //check for number
                if(!(infix[i]=='+' || infix[i]=='-' || infix[i]=='*'|| infix[i]==' ' || infix[i]=='/' || infix[i]=='^'|| infix[i]=='('|| infix[i]==')'|| infix[i]=='%'||infix[i]=='.') ) //check for operation
                    return 0;
       if(i==0)
       {   //if the expression starts with something other than a digit or a variable
           if(!isdigit(infix[i])&&!isalpha(infix[i])&&infix[i]!='-'&&infix[i]!='(')
                return 0;
            //handling negative with brackets in the beginning of an expression
           if(infix[i]=='-')
           {
               if(infix[i+1]=='(')
               {
               int j;
                   for(j=strlen(infix);j>0;j--)
                       infix[j+2]=infix[j];

                   infix[1]='1';
                   infix[2]='*';
                   infix[strlen(infix)]='\0';
               }

           }
       }
       else if(infix[i]=='-')
       {   //if there is a minus before a negative number it should pass but other than it results in an error
           if(infix[i-1]=='-')
            if(i<2||(!isdigit(infix[i-2])&&!isalpha(infix[i-2])&&infix[i-2]!=')'))
               return 0;
           //if a closed bracket comes after - or the expression ends with -, it results in an error
           if(infix[i+1]==')'||infix[i+1]=='\0')
            return 0;

       }
       else if(infix[i]=='+'||infix[i]=='*'||infix[i]=='/')
       {    //not two operators should follow each other or come directly after an open bracket
           if(infix[i-1]=='('||infix[i-1]=='+'||infix[i-1]=='-'||infix[i-1]=='*'||infix[i-1]=='/')
                return 0;
           //if a closed bracket comes after an operator OR if the expression ends with an operator it results in an error
           if(infix[i+1]==')'||infix[i+1]=='\0')
            return 0;


       }
       else if(infix[i]=='.')
       {  //handling floating numbers
           if(!isdigit(infix[i-1])||!isdigit(infix[i+1])||infix[i-1]=='.')
            return 0;
       }
    }
    return 1;
}
///missing equal error handling
int isMissingEqual(char *expression)
{
    int i;
    for(i=0; i<strlen(expression); i++)
    {
        if(expression[i]=='=')
            return 1;
    }
    return 0;
}
///remove spaces from expressions to avoid any errors
char * removeSpaces(char *expression)
{
    char * newExpression=malloc(strlen(expression));
    newExpression[0]='\0';
    char *token;
   token=strtok(expression," ");

    while(token)
    {
        strcat(newExpression,token);
        token=strtok(NULL," ");
    }
    newExpression[strlen(newExpression)]='\0';
    return newExpression;
}
///reading the file
void readFile(char * filename)
{
    FILE *f=fopen(filename,"r");
    char *key=malloc(30);
    char *infix=malloc(256);
    char *postfix=malloc(256);
    char* temp=malloc(30+257);
    Node * root=NULL;
    Heap *heap=malloc(sizeof(Heap));
    int lineCounter=0;

    while(!feof(f))
    {   lineCounter++;
        fscanf(f,"%[^\n]%*c",temp);
        //check if the expression has an equal
        if(isMissingEqual(temp))
        {   //remove spaces from expression
            temp=removeSpaces(temp);
            key=strtok(temp,"=");
            infix=strtok(NULL,"=");
            //check if the key and the infix is right
            if(valid(key,infix))
            {   //change infix to postfix
                infixToPostfix(infix,postfix,root);
                //calculate the variable's value
                float temp =evaluatePostfix(postfix);
                //insert variable in tree and heap
                root=insert(root,key,temp);
                heap=insertHeap(heap,temp,key);

            }
            else
                printf("invalid expression in line (%d)\n",lineCounter);
        }
        else
            printf("Missing equal in line (%d)\n",lineCounter);
    }

    printf("Sorting by key:\n");
    InOrdertraverse(root);
    printf("\nSorting by value:\n");
    heapSort(heap);
    fclose(f);
}

/*
*
*/
int main()
{
    char filename[256];
    printf("Please Enter The File Name: ");
    scanf("%s",filename);
    readFile(filename);


    return 0;
}
