#include<stdio.h>
#include<stdlib.h>

typedef struct sort_tree {
    int data;
    struct sort_tree *left;//指针域，左指针
    struct sort_tree *right;//右指针
}Node;

/*全局变量*/
Node* root;//根节点

void Init(int key);//形参为根节点的值
void insert(int key);
void show(Node* node);//传入根节点进行中序遍历

int main(int argc, char** argv)
{
    Init(4);
    insert(1);
    insert(1);
    insert(3);
    insert(5);
    insert(2);
    insert(7);
    insert(10);
    insert(0);
    show(root);
}

void Init(int key)
{
    root = (Node*) malloc(sizeof(Node));
    root -> data = key;
    root -> left = NULL;
    root -> right = NULL;
}

void insert(int key)
{
    //找父节点
    Node* temp = root;//定义一个临时指针用于移动,以及跳出循环
    Node* pre = NULL;//定位到待插入位置的前一个节点
    while(temp != NULL){
        pre = temp;
        if(key < temp -> data){
            temp = temp -> left;
        }else if(key > temp -> data){
            temp = temp -> right;
        }else if(key == temp -> data){
            printf("%d已存在",key);
            return;
        }
    }
    if(key < pre -> data){
        pre -> left = (Node*)malloc(sizeof(Node));
        pre -> left -> data = key;
        pre -> left -> left = NULL;
        pre -> left -> right = NULL;
    }
    if(key > pre -> data){
        pre -> right = (Node*)malloc(sizeof(Node));
        pre -> right -> data = key;
        pre -> right -> left = NULL;
        pre -> right -> right = NULL;
    }
}

void show(Node * node)
{
    if(node == NULL){
        return;
    }
    show(node -> left);
    printf("%d\n",node -> data);
    show(node -> left);
}