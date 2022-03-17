/*双向链表*/
#include<stdio.h>
#include<stdlib.h>

typedef int ElementType;
typedef struct DoubleList{
    ElementType element;
    struct DoubleList* front;
    struct DoubleList* next;
}Dlist;

void Initialize();
void head_insert(ElementType);
void tail_insert(ElementType);
void mydelete(Dlist*,ElementType);
void myprint();
int findkey(ElementType);

Dlist* head;//定义全局变量头尾节点
Dlist* end;//尾

int main(int argc, char * argv[])
{
    //因为有全局变量，所以初始化并不需要返回任何东西
    Initialize();
    head_insert(1);
    head_insert(6);
    tail_insert(9);
    myprint();

    return 0;
}

void Initialize()
{
    head = (Dlist*)malloc(sizeof(Dlist));
    end = (Dlist*)malloc(sizeof(Dlist));
    if(head == NULL || end == NULL){
        printf("ERROR");
        exit(1);
    }
    head -> next = end;
    head -> front = NULL;
    end -> next = NULL;
    end -> front = head;
    //end -> element = 666;
}

void head_insert(ElementType key)
{
    Dlist* new_node = (Dlist*)malloc(sizeof(Dlist));
    if(new_node == NULL){
        printf("Error");
    }
    new_node -> element = key;//一个赋值运算
    new_node -> next = head -> next;//四个逻辑赋值
    new_node -> front = head;
    head -> next -> front = new_node;
    head -> next = new_node;
}

void tail_insert(ElementType key)
{
    Dlist* new_node = (Dlist*)malloc(sizeof(Dlist));
    if(new_node == NULL){
        printf("Error");
    }
    new_node -> element = key;
    end -> front -> next = new_node;
    new_node -> front = end -> front;
    new_node -> next = end;
    end -> front = new_node;
}

void mydelete(Dlist* node,ElementType key)
{
    Dlist* temp = node -> next;
    int index = findkey(key);
    if(index == -1){
        printf("俺寻思没你这个key");
    }else{
        int i = 1;
        while(i < index){
            temp = temp -> next;
            i++;
        }//循环结束后temp即为要删除元素
        Dlist* free_node = temp;
        temp -> front -> next = temp -> next;
        temp -> next -> front = temp -> front;
        free(free_node);
    }
}



void myprint()
{
    Dlist* temp = head -> next;
    while(temp -> next != NULL){
        printf("%d\n",temp -> element);
        temp = temp -> next;
    }
}


int findkey(ElementType key)
{
    Dlist* temp = head -> next;
    int i = 1;
    while(temp -> next != NULL){
        if(temp -> element != key){
            temp = temp -> next;
            i++;
        }else if(temp -> element = key){
            return i;
        }
    }
    return -1;//遍历完全部还没找到那就返回-1\
    -1有个好处就是不管初始下标是0还是1反正返回负一总没错
}