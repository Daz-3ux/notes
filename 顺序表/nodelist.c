#include<stdio.h>
#include<stdlib.h>

typedef int ElementType;

typedef struct nodelist{
    int element;
    struct nodelist *next;
}Node;

Node * InitList(Node *);//指针函数，接受一个结构体，返回一个指向结构体的指针
void head_insert(Node *,ElementType);
void tail_insert(Node *,ElementType);
void deletekey(Node *,ElementType);
int findkey(Node *,ElementType);
void printlist(Node *);//遍历单链表



int main(int argc, char *argv[])
{
    Node * mylist = InitList(mylist);
    tail_insert(mylist,9);
    head_insert(mylist,1);
    head_insert(mylist,2);
    head_insert(mylist,3);
    tail_insert(mylist,3);
    head_insert(mylist,3);
    head_insert(mylist,1);
    tail_insert(mylist,9);
    
    printlist(mylist);
}

Node * InitList(Node * node)//初始化头结点
{
    //只有指针域
    node = (Node *) malloc(sizeof(Node));
    if(node == NULL) {
        printf("Error\n");
    }else{
        node->next = NULL;
    }
    return node;
}

void head_insert(Node * node,ElementType key)
{
    Node* temp = (Node*)malloc(sizeof(node));
    temp->element = key;
    temp->next = node->next;
    node->next = temp;
}

void tail_insert(Node * node,ElementType key)
{
    Node* temp = (Node*)malloc(sizeof(Node));
    while(node->next != NULL){
        node = node->next;
    }
    node -> next = temp;
    temp -> element = key;
    temp -> next = NULL;
}

int findkey(Node * node,ElementType key)
{
    Node *temp;
    temp = node->next;
    int i = 0;//第一个为零，这很C语言
    while(temp != NULL){
        if(temp->element == key){
            return i;
        }else{
            temp = temp->next;
            i++;
        }
    }

    return -1;
}

void deletekey(Node * node,ElementType key)
{
    int index = findkey(node,key);//index为待查找元素的下标
    if(index == -1){
        printf("Error");
    }else{
        int i = 0;
        Node* temp = node;
        while(i < index){
            temp = temp->next;//假设index为3，当i=3时，temp\
            指向第二号元素
            i++;
        } 
        Node* free_node = temp->next;//指向该被释放的第三号元素
        temp -> next = temp->next->next;
        free(free_node);
    }
}

void printlist(Node* node)
{
    Node* temp;
    temp = node->next;
    while(temp != NULL){
        printf("%d ",temp -> element);
        temp = temp -> next;
    }
}