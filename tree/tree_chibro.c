#include<stdio.h>
#include<stdlib.h>

typedef struct ChildBro{
    struct ChildBro* child;//指向孩子变量
    struct ChildBro* bro;//指向兄弟变量
    int data;//当前节点数据域
}Node;

Node* root;//全局变量根节点
Node* tempNode;//临时变量，方便从根节点开始遍历

void Init(int);//初始化根节点
void insert(int,int);//待插入数据+父节点
Node* get_node(Node*,int);//定位到待插入节点

int main(int argc, char** argv)
{
    Init(1);
    insert(1,1);
    insert(2,1);
}

void Init(int key)
{
    root = (Node*) malloc(sizeof(Node));
    root -> data = key;
    root -> child = NULL;//刚开始时候根节点的child和bro都是空的[所有不用的指针记得指向NULL]
    root -> bro = NULL;
}

void insert(int key,int parent)//待插入数据+父节点
{
    //先定位父节点
    tempNode = get_node(root,parent);
    if (tempNode == NULL){
        printf("ERROR,没有父节点");//没有父节点
    }else{
        if(tempNode -> child != NULL){//如果父节点的孩子不是空
            tempNode = tempNode -> child;
            Node* node = (Node*)malloc(sizeof(Node));
            node -> data = key;
            node -> child = NULL;
            node -> bro = tempNode -> bro;
            tempNode -> bro = node;
        }else{//如果父节点的孩子是空的
            Node* node = (Node*)malloc(sizeof(Node));
            node -> child = NULL;
            node -> bro = NULL;
            node -> data = key;
            tempNode -> child = node;
        }
    }
}

Node* get_node(Node* node,int parent)//定位父节点，拿递归写,node刚开始就是根节点
{
    //node刚开始是根节点，所以第一个数据插得很顺利
    if(node -> data == parent){
        return node;
    }
    //只要理解递归，那么就能明白下面两个函数先后顺序不重要
    if(node -> child != NULL){//如果孩子不是空，那就把它的孩子传到递归中
        get_node(node->child,parent);
        return NULL;
    }
    if(node -> bro != NULL){//如果它的兄弟不为空，那就把它的兄弟传到递归中
        get_node(node->bro,parent);
        return NULL;
    }

    return NULL;//要是递归完了还没找到那就是没有，返回NULL
}