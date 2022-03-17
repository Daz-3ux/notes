/*使用孩子兄弟表示法+链表完成表示*/
/*注意所有暂时不用的指针都要指向NULL*/

#include<stdio.h>
#include<stdlib.h>

typedef struct tree_node{
    struct tree_node* child;//指向孩子节点（指向tree_node类型结构体的指针）
    struct tree_node* bro;//指向父节点
    int data;//节点数据域
}Node;

Node* root;//全局变量根节点
Node* temp;

void Init(int key);//，初始化根节点，为根节点插入数据
void insert(int key,int parent);
Node* get_node(Node* node,int parent);//定位到带插入节点

int main(int argc, char** argv)
{
    Init(0);
    insert(1,0);
    insert(2,0);
    insert(3,1);
    insert(4,2);
    insert(5,666);
}

void Init(int key)//初始化全局变量根节点
{
    root =(Node*)malloc(sizeof(Node));
    root -> data = key;
    root -> child = NULL;
    root -> bro = NULL;
}

void insert(int key,int parent)
{
    temp = get_node(root,parent);//temp是待被插入节点
    if(temp == NULL){
        printf("%d没有父节点啊哥们\n",key);
    }else{
        if(temp -> child != NULL){//如果该节点的孩子不为空
            temp = temp -> child;
            Node* node =(Node*)malloc(sizeof(Node));
            node -> data = key;
            node -> child = NULL;
            node -> bro = temp -> bro;
            temp -> bro = node;
        }else{//如果该节点的孩子为空
            Node* node = (Node*)malloc(sizeof(Node));
            node -> child = NULL;
            node -> bro = NULL;
            node -> data = key;
            temp -> child = node;
        }
    }

}

Node* get_node(Node* node,int parent)//定位父节点
{
    if(node -> data == parent){
        return node;
    }
    if(node -> bro != NULL){
        return get_node(node-> bro,parent);
    }
    if(node -> child != NULL){
        return get_node(node -> child,parent);
    }
    
    
    return NULL;
}