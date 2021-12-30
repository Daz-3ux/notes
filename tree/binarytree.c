/*new：使用孩子兄弟表示法为基础完成二叉树遍历*/

/*使用孩子兄弟表示法+链表完成表示*/
/*注意所有暂时不用的指针都要指向NULL*/

#include<stdio.h>
#include<stdlib.h>



#define maxSize 5//当前容量

void InitQueue();
int isEmpty();//队空
int isFull();//队满
void insertqueue(int);
void deletekey();

int* queue;//用数组实现队列
int front;//头指针，因为是数组，所以用下标代表指针
int end;//尾指针






typedef struct tree_node{
    struct tree_node* child;//指向孩子节点（指向tree_node类型结构体的指针）
    struct tree_node* bro;//指向父节点
    int data;//节点数据域
}Node;

Node* root;//全局变量根节点
Node* temp;

void Init(int key);//，初始化根节点，为根节点插入数据
void insert(int key,int parent);
void get_node(Node* node,int parent);//定位到带插入节点

/*new*/void Traverse(Node* node);//遍历二叉树
void levelOrder(Node*);//层序遍历二叉树

int main(int argc, char** argv)
{
    Init(0);
    insert(1,0);
    insert(2,0);
    insert(3,1);
    insert(4,2);
    insert(5,666);
    Traverse(root);//中序
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
    temp = NULL;
    //temp = get_node(root,parent);//temp是待被插入节点
    get_node(root,parent);
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

void get_node(Node* node,int parent)//定位父节点
{
    if(node -> data == parent){
        temp = node;
        return;
    }
    if(node -> child != NULL){
        get_node(node -> child,parent);
    }
    if(node -> bro != NULL){
        get_node(node-> bro,parent);
    }
    
}

// Node* get_node(Node* node,int parent)//定位父节点
// {
//     if(node -> data == parent){
//         return node;
//     }
//     if(node -> bro != NULL){
//         return get_node(node-> bro,parent);
//     }
//     if(node -> child != NULL){
//         return get_node(node -> child,parent);
//     }
    
    
//     return NULL;
// }

void Traverse(Node *node)
{
    if(node == NULL){
        return;//边界
    }


    //printf("%d\n",node -> data);先序遍历
    Traverse(node -> child);//往左
    printf("%d\n",node -> data);//中序遍历
    Traverse(node -> bro);//往右
    //printf("%d\n",node -> data);后序遍历
}

void levelOrder(Node* node)
{
    //先初始化队列
    InitQueue();
    if(node != NULL){
        insertqueue(node -> data);
    }
    while(!isEmpty()){
        //4int temp = deletekey();

    }
}

void InitQueue()
{
    queue = (int*)malloc(sizeof(int) * maxSize);
}

int isEmpty()//队空
{

    if(front == end){
        return 1;
    }else{
        return 0;
    }
}


int isFull()//队满
{
    if((end+1) % maxSize == front){
        return 1;
    }else{
        return 0;
    }
}


void insertqueue(int key) 
{
    if(isFull()){
        printf("FULL");
    }else{
        end  =(end+1) % maxSize;
        queue[end] = key;
    }
}

void deletekey()
{
    if(isEmpty()){
        printf("EMPTY");
    }else{
        front = (front + 1) % maxSize;
    }
}