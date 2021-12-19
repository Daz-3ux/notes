#include<stdio.h>
#include<stdlib.h>
#include<string.h>

//双亲表示法[用顺序表更好]
typedef struct tree_node{
    int  data;//树中数据
    int parent;//-1代表没有父节点
}Node;

//全局变量
Node* node[5];//存放双亲表示法的顺序表表示 
int size;//当前元素个数
int maxSize;//元素总个数

void init();//初始化
void insert_root(int);//建立根节点，int是根节点的关键字
void insert_child(int,int);//插入元素，第一个int是待插入元素，第二个是父节点关键字
int find_parent(int);//查找父节点下标

int main(int argc, char** argv)
{
    init();
    insert_root(0);
    insert_child(1,0);
    insert_child(2,0);
}

void init()
{
    size = 0;
    maxSize = 5;
}

void insert_root(int key)//key为根节点关键字
{
    Node* new_node = (Node*)malloc(sizeof(node));
    new_node -> data = key;
    new_node -> parent = -1;
    node[size++] = new_node;
}//完事后size是1

void insert_child(int key,int parent)//一个待插入元素，一个父节点
{
    if(size == maxSize){
        printf("FULL");
        exit(1);
    }
    if(size == 0){
        printf("NO ROOT");
    }
    //首先判断 是否有父节点
    int parent_index = find_parent(parent);
    if(parent_index == -1){
        printf("NO PARENT");//没有该父节点
    }
    Node* new_node = (Node*)malloc(sizeof(node));
    new_node -> data = key;
    new_node -> parent = parent;
    node[size++] = new_node;
}

int find_parent(int parent)
{
    for(int i = 0;i < size;i++){
        if(parent == node[i] -> data){//找他爹。它的他爹关键字就是它爹的数据
            return i;//返回父节点下标
        }
    }
    return -1;//没找到
}