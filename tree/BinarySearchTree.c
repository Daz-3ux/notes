#include<stdio.h>
#include<stdlib.h>

typedef struct sort_tree {
    int data;
    struct sort_tree* left;//指针域，左指针
    struct sort_tree* right;//右指针
}Node;

/*全局变量*/
Node* root;//根节点

void Init(int key);//形参为根节点的值
void insert(int key);
void show(Node* node);//传入根节点进行中序遍历
int delete_node(Node* node,int key);//从根节点开始遍历删除指定元素
/*定位到待删除节点的前一个节点*/
Node* pre_node(Node* root,Node* node,int key);//根节点，待删除节点,待删除数据

int main(int argc, char** argv)
{
    Init(8);
	insert(4);
	insert(2);
	insert(5);
	insert(10);
	insert(9);
	insert(13);
	show(root);
	delete_node(root, 8);
	delete_node(root, 13);
	printf("\n");
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
        }else{
            printf("%d已存在",key);
            return;
        }
    }

    if(key < pre -> data){
        pre -> left = (Node*)malloc(sizeof(Node));
        pre -> left -> data = key;
        pre -> left -> left = NULL;
        pre -> left -> right = NULL;
    }else if(key > pre -> data){
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
    printf("%d ",node -> data);
    show(node -> right);
}

int delete_node(Node* node,int key)
{
    if(node == NULL){
        printf("已经没有数据了");
        return -1;
    }else{
        if(node -> data == key){
            //删除操作:首先定位到待删除元素前一个节点
            Node* tempNode = pre_node(root,node,key);//从根节点遍历，找到待删除节点及其待删除元素，\
            后两者可以一起写，这里分开是为了方便理解
            Node* temp = NULL;
            /*如果右子树为空，只需重新连接节点(包含了叶子情况)*/
            if(node -> right == NULL){//如果右子树为空
                temp = node;
                node = node -> left;//此时有三个节点，tempNode指向待删除节点的前一个节点，temp指向待删除，node指向下一个
                if(tempNode -> left -> data == temp -> data) {
                    Node* free_node = temp;//释放用指针
                    tempNode -> left = node;
                    free(free_node);
                    free_node = NULL;
                }else{
                    Node* free_node = temp;//释放用指针
                    tempNode -> right = node;
                    free(free_node);
                    free_node = NULL;
                }
            }else if(node -> left == NULL){//如果左子树为空
                temp = node;
                node = node -> right;//此时有三个节点，tempNode指向待删除节点的前一个节点，temp指向待删除，node指向下一个
                if(tempNode -> left -> data == temp -> data) {
                    Node* free_node = temp;//释放用指针
                    tempNode -> left = node;
                    free(free_node);
                    free_node = NULL;
                }else{
                    Node* free_node = temp;//释放用指针
                    tempNode -> right = node;
                    free(free_node);
                    free_node = NULL;
                }
            }else{//左右子树都不为空
                temp = node;
                /*往左子树 找最大值*/
                Node* left_max = node;//找最大值的临时指针
                left_max = left_max -> left;//先找到左子树
                while(left_max -> right != NULL){
                    temp = left_max;//指向前一个
                    left_max = left_max -> right;
                }
                node -> data = left_max -> data;
                if(temp != node){
                    temp -> right = left_max -> left;
                    free(left_max);
                    left_max = NULL;
                }else{
                    temp -> left = left_max -> left;
                    free(left_max);
                    left_max = NULL;
                }
            }

        }else if(key < node -> data){//node -> data != key
            delete_node(node -> left,key);
        }else if(key > node -> data){
            delete_node(node -> right,key);
        }
    } 

    return -1;
}

Node* pre_node(Node* root,Node* node,int key)
{
    if(root == NULL || node == root){
        return node;
    }else{
        if(root -> left != NULL && root -> left -> data == key){
            return root;
        }else if(root -> right != NULL && root -> right -> data == key){
            return root;
        }else if(key < root -> data){
            return pre_node(root -> left,node,key);
        }else if(key > root -> data){
            return pre_node(root -> right,node,key);
        }
    }

    return NULL;
}