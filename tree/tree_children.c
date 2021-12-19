/*孩子表示法：数组+链表*/
/*从上往下实现，首先给出根节点*/
/*-1代表无父节点*/
#include <stdio.h>
#include<stdlib.h>

typedef struct LinkList{
    int data;
    struct LinkList* next;
}Node;

Node* node_array[100];//存储节点的数组
int size;//数组中当前元素个数

void Init(int);//初始化,顺便给出根节点，从上往下实现比较简单
void creat_tree(int parent,int key);//父节点的值与孩子节点的值
int find_parent(int parent);

int main(int argc, char** argv)
{
    Init(1);
    creat_tree(1,2);//先父节点再孩子
    creat_tree(1,3);
    creat_tree(1,4);
    creat_tree(1,5);
    creat_tree(1,6);
    creat_tree(1,7);
    creat_tree(1,8);
    creat_tree(111,1111);
    creat_tree(1111,123);
    for(int i = 0; i < size;i++){
        printf("父节点为：%d\n",node_array[i] -> data);
        Node* temp = node_array[i] -> next;
        while(temp != NULL){
            printf("孩子节点为：%d\n",temp -> data);
            temp = temp -> next;
        }
    }
}

void Init(int key)
{//初始化并且建立根节点
    node_array[size] = (Node*)malloc(sizeof(Node));//*全局变量没有初始化的话默认初始化为0
    node_array[size] -> data = key;
    node_array[size] -> next = NULL;
    size++;//当前元素个数++
}

void creat_tree(int parent,int key)
{
    node_array[size] = (Node*)malloc(sizeof(Node));//先将孩子节点添加到数组当中，从这开始孩子节点就可以是一个新的父节点了
    node_array[size] -> data = key;//给新节点赋值
    node_array[size] -> next = NULL;
    size++;//当前元素个数++
    //找父节点
    int index = find_parent(parent);
    if(index == -1){
        printf("NO PARENT：%d\n",parent);//没有父节点的话就是一个新的父节点
    }else{
        Node* new_node = (Node*)malloc(sizeof(Node));//链表实现一个节点带多个孩子
        new_node -> data = key;
        new_node -> next = node_array[index] -> next;//头插法
        node_array[index] -> next = new_node;//头插法
    }
}

int find_parent(int parent)
{
    for(int i = 0; i < size;i++){
        if(node_array[i] -> data == parent){
            return i;//返回父节点下标
        }
    }
    return -1;
}