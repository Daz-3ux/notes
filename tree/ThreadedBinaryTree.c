/*
 * =====================================================================================
 *
 *       Filename:  ThreadedBinaryTree.c
 *
 *    Description: learn tree 
 *
 *        Version:  1.0
 *        Created:  2022年01月24日 21时37分35秒
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  lin3ux 
 *
 * =====================================================================================
 */
#include <stdlib.h>
#include <stdio.h>

typedef struct Threaded{
	struct Thread *left_node;
    struct Thread *right_node;//左右指针
	int data;//数据
	int left_type;//左 类型标志
	int right_type;//右 类型标志
	/* 0代表左右孩子，1代表前驱或后继 */
}Node;

//以中序线索化为例

Node *pre = NULL;//前驱节点的变量
Node *head;//头指针，指向某种遍历的第一个节点


void inOrderThreadTree(Node *node)
{
    //如果当前结果为NULL，直接返回
    if(node == NULL){
        return;
    }
    //先处理左子树
    inOrderThreadTree(node -> left_node);
    if(node -> left_node == NULL){
        //设置前驱节点
        node -> left_type = 1;
        node -> left_node = pre;
    }
    //如果节点的右子节点为NULL 处理前驱的右指针
    if(pre != NULL && pre -> right_node == NULL){
        //设置后继
        pre -> right_node = node;
        pre -> right_type = 1;
    }
    //没处理一个节点，当前节点就是下一个节点的前驱
    pre = node;
    //最后处理右子树
    inOrderThreadTree(node -> right_node);
}

void inOrderTraverse(Node* root)
{
    //从根节点开始先找到左边
    if(root == NULL){
        return;
    }
    Node *temp = root;
    //先找到最左边节点 然后根据线索化直接向右遍历
    while(temp != NULL && temp -> left_type == 0){
        temp = temp -> left_node; 
    }
    while(temp != NULL){
        //输出
        printf("%d",temp -> data);
        //遍历
        temp = temp -> right_node;
    }
}

int main(int argc,char **argv)
{

}





