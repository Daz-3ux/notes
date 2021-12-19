#include<stdio.h>
#include<stdlib.h>

typedef struct nodelist{
    int element;//存储数据值
    struct nodelist* next;//指向下一个节点的指针
}Node;

// Node* initlist(Node*);//初始化单线链表，函数形参为指向结构体的指针，返回也是指向结构体的指针
Node* InitList();//返回值为指向node的指针，在函数内部直接初始化，因为malloc分配的内存在出去后不会被释放
void head_insert(Node*,int);
void tail_insert(Node*,int);
void mydelete(Node*,int);
void myprint(Node*);//遍历单链表
int myfind(Node*,int);

int main(int argc, char* argv[])
{
    Node* linklist = InitList();
    head_insert(linklist,0);
    head_insert(linklist,1);
    head_insert(linklist,2);
    mydelete(linklist,1);
    tail_insert(linklist,66);
    myprint(linklist);

}

Node* InitList()//直接初始化
{
    Node* head = (Node*) malloc(sizeof(Node));
    if(head == NULL){
        printf("Error");
    }
    head->element = 0;//记录元素个数，方便遍历输出
    head->next = NULL;//指向自己，完成初始化
    return head;
}

void tail_insert(Node* node, int key)
{
    Node* new_node = (Node*)malloc(sizeof(Node));
    new_node -> element = key;
    Node* temp = node;
    while(temp -> next != NULL){
        temp = temp -> next;
    }
    temp -> next = new_node;
    new_node -> next = NULL;
}

void head_insert(Node* node,int key)
{
    Node* new_node = (Node*)malloc(sizeof(Node));//为新节点分配空间
    new_node -> element = key;//新节点值
    new_node -> next = node -> next;//注意这一步和下一步不能位置错乱，很妙
    node -> next = new_node;
    node -> element++;//头结点的记录值++
}

void mydelete(Node* node,int key)
{
    int index = myfind(node,key);
    if(index == -1){
        printf("NO EXIST");
    }else{
        int i = 1;
        Node* temp = node;
        while(i < index){
            temp = temp -> next;//temp被复制为目标节点的前一个节点
            i++;
        }
        Node* free_node = temp -> next;
        temp -> next = temp -> next->next;
        free(free_node);
    }
}

int myfind(Node* node,int key)
{
    Node* temp;//定义一个临时结构体用来查找
    temp = node -> next;//temp是首元节点
    int i = 1;//将首元节点下标定义为1
    while(temp != NULL){
        if(temp -> element == key){
            return i;
        }
        temp = temp -> next;
        i++;
    }//多次执行判断，直到找到数据或者一无所获
    return -1;
}

void myprint(Node *node)
{
    Node* temp = node -> next;
    while(temp != NULL){
        printf("%d\n", temp -> element);
        temp = temp -> next;
    }
}