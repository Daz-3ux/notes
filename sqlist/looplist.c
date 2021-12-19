#include<stdio.h>
#include<stdlib.h>

typedef struct LinkList{
    int element;
    struct LinkList* next;
}List;

List* InitList();
void head_insert(List*,int);
void printlist(List*);


int main(int argc, char **argv)
{
    List* list = InitList();

    head_insert(list,1);
    head_insert(list,2);
    head_insert(list,1);
    printlist(list);
}

List* InitList()//直接初始化
{
    List* head = (List*) malloc(sizeof(List));
    if(head == NULL){
        printf("Error");
    }
    head->element = 0;//记录元素个数，方便遍历输出
    head->next = head;//循环链表精髓
    return head;
}

// List* list = InitList();
// Node * mylist = InitList(mylist);
// Node * InitList(Node * node)//初始化头结点
// {
//     //只有指针域
//     node = (Node *) malloc(sizeof(Node));
//     if(node == NULL) {
//         printf("Error\n");
//     }else{
//         node->next = NULL;
//     }
//     return node;
// }

void head_insert(List* node,int key)
{
    List* new_node = (List*) malloc(sizeof(List));
    new_node->element = key;
    new_node->next = node->next;
    node -> next = new_node;
    node -> element++;
}

void printlist(List* node)
{
    List* temp;
    temp = node->next;
    while(temp != node){
        printf("%d ",temp -> element);
        temp = temp -> next;
    }
}