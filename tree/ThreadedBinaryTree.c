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
	struct Thread* left_node, right_node;//左右指针
	int data;//数据
	int legt_type;//左 类型标志
	int right_type;//右 类型标志
	/* 默认0代表左右孩子，1代表前驱或后继 */
}Node;

//以中序线索化为例

void inOrderThreadTree(Node* node)
{
    
}










