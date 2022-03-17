#include<stdio.h>
#include<stdlib.h>

typedef int ElementType;

typedef struct DoubleList{
    ElementType element;
    struct DoubleList* front;
    struct DoubleList* next;
}Dlist;//双向链表

void Initialize();
void head_insert(ElementType);
int findkey(ElementType key);

//全局变量
Dlist* head;
Dlist* end;

int main(int argc, char** argv)
{

}

void InitLsit()
{
    head = (Dlist*)malloc(sizeof(Dlist));
    end = (Dlist*)malloc(sizeof(Dlist));
    if(head == NULL || end == NULL){
        printf("Error");
        exit(1);
    }
    head -> next = end;
    end -> front = head;
    head -> front = NULL;
    end -> next = NULL;
}

void head_insert(ElementType key)
{
    Dlist* new_node =(Dlist*)malloc(sizeof(Dlist));
    if(new_node == NULL){
        printf("Error");
    }
    new_node -> element = key;
    new_node -> next = head -> next;
    new_node -> front = head;
    head -> next ->front = new_node;
    head -> next = new_node;
}

int findkey(ElementType key)
{
    Dlist* temp = head -> next;//指向首元节点
    int i = 1;//让第一个是1
    while(temp -> next != NULL){
        if(temp -> element == key){
            return i;
        }else{
            temp = temp -> next;
            i++;
        }
    }
    return 0;
}

