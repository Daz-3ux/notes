/*双亲表示法+顺序表 实现树*/
/*此程序使用-1表示无父节点*/

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#define MAX 10

typedef struct tree_node{
    int data;
    int parent;
}Node;//每一个节点都是这样子，和链表不一样的地方就在于\
        这里使用的是顺序表进行连接

Node* node[MAX];//定义三个全局变量
int size;//当前元素个数
int maxSize;//元素总个数

void Init();//初始化接口
void insert_root(int);//建立根节点
void insert_child(int data, int parent);//插入一个孩子节点，第一个元素为孩子元素的值，第二个元素为孩子元素的父亲\
                                        如果某一节点的data值等于形参中parent值，那么这个节点就是待插入元素的父节点
int find_parent(int parent);//只有一个作用：判断父节点是否存在

int main(int argc, char** argv)
{
    Init();
    insert_root(0);
    insert_child(2,0);
    insert_child(2,0);
    insert_child(3,2);//??
    for(int i=0; i < size;i++){
        printf("%d\n",node[i] -> data);
    }
}

void Init()//初始化就是确定当前元素个数和确定最大元素个数，但封装成函数总没错
{
    size = 0;
    maxSize = MAX;
}

void insert_root(int key)
{
    Node *new_node = (Node*)malloc(sizeof(Node));//分配一块node大小的内存，并返回一个指向node类型的指针
    new_node -> data = key;
    new_node -> parent = -1;//用-1代表无父节点，满足开头注释
    node[size/*++*/] = new_node;//node数组是一个结构体数组
    size++;//程序可读性
}

void insert_child(int key,int parent)
{
    if(size == maxSize){//健壮性
        printf("FULL");
    }
    if(size == 0){//还是健壮性
        printf("ROOT NOT EXIST");
    }
    int parent_index = find_parent(parent);
    if(parent_index == -1){
        printf("PARENT NOT EXIST");
    }
    //终于判断完了可以开始插入了
    Node* new_node = (Node*)malloc(sizeof(Node));
    new_node -> data = key;
    new_node -> parent = parent;
    node[size] = new_node;
    size++;
}

int find_parent(int parent)
{
    for(int i = 0;i < size;i++){//遍历目前所有元素
        /*Node* node[5]*/
        if(parent == node[i] -> data){//parent = 几的元素，几就是它的父节点
            return i;//返回父节点下标
        }
    }
    return -1;//返回-1代表没找到
}