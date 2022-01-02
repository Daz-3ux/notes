#include<stdio.h>
#include<stdlib.h>
typedef struct SortTree {
	int data;//Žæ·ÅÊýŸÝµÄÊýŸÝÓò
	struct SortTree* left;//ÖžÕëÓò ×óÖžÕë
	struct SortTree* right;//ÖžÕëÓò ÓÒÖžÕë
}Node;
/*È«ŸÖ±äÁ¿*/
Node* root;//žùœÚµã

void Init(int);//³õÊŒ»¯²Ù×÷
void insert(int);//²åÈë²Ù×÷
void show(Node*);
int delete_node(Node*, int);
Node* prev_node(Node*, Node*, int);
int main()
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

/*³õÊŒ»¯žùœÚµã
int key : žùœÚµãµÄÖµ
*/
void Init(int key)
{
	root = (Node*)malloc(sizeof(Node));
	root->data = key;
	root->left = NULL;
	root->right = NULL;
}

void insert(int key)
{
	//¶šÒåÒ»žöÁÙÊ±ÖžÕë ÓÃÓÚÒÆ¶¯
	Node* temp = root;//·œ±ãÒÆ¶¯ ÒÔŒ° Ìø³öÑ­»·
	Node* prev = NULL;//¶šÎ»µœŽý²åÈëÎ»ÖÃµÄÇ°Ò»žöœáµã
	while (temp != NULL)
	{
		prev = temp;
		if (key < temp->data)
		{
			temp = temp->left;
		}
		else if(key > temp->data)
		{
			temp = temp->right;
		}
		else
		{
			return;
		}
	}

	if (key < prev->data)
	{
		prev->left = (Node*)malloc(sizeof(Node));
		prev->left->data = key;
		prev->left->left = NULL;
		prev->left->right = NULL;
	}
	else
	{
		prev->right = (Node*)malloc(sizeof(Node));
		prev->right->data = key;
		prev->right->left = NULL;
		prev->right->right = NULL;
	}
}

void show(Node* root)
{
	if (root == NULL)
	{
		return;
	}
	show(root->left);
	printf("%d ", root->data);
	show(root->right);
}

int delete_node(Node* node, int key)
{
	if (node == NULL)
	{
		return -1;
	}
	else
	{
		if (node->data == key)
		{
			//µ±ÎÒÖŽÐÐÉŸ³ý²Ù×÷ ÐèÒªÏÈ¶šÎ»µœÇ°Ò»žöœáµã
			Node* tempNode = prev_node(root, node, key);
			Node* temp = NULL;
			/*
			Èç¹ûÓÒ×ÓÊ÷Îª¿Õ Ö»ÐèÒªÖØÐÂÁ¬œÓœáµã
			Ò¶×ÓµÄÇé¿öÒ²°üº¬œøÈ¥ÁË Ö±œÓÉŸ³ý
			*/
			if (node->right == NULL)
			{
				temp = node;
				node = node->left;
				/*ÎªÁËÅÐ¶Ï ŽýÉŸ³ýœáµãÊÇÇ°Ò»žöœáµãµÄ×ó±ß»¹ÊÇÓÒ±ß*/
				if (tempNode->left->data == temp->data)
				{
					Node* free_node = temp;//ÊÍ·ÅÓÃµÄÖžÕë
					tempNode->left = node;
					free(free_node);
					free_node = NULL;
				}
				else
				{
					Node* free_node = temp;//ÊÍ·ÅÓÃµÄÖžÕë
					tempNode->right = node;
					free(free_node);
					free_node = NULL;
				}
			}
			else if (node->left == NULL)
			{
				temp = node;
				node = node->right;
				if (tempNode->left->data == temp->data)
				{
					Node* free_node = temp;//ÊÍ·ÅÓÃµÄÖžÕë
					tempNode->left = node;
					free(free_node);
					free_node = NULL;
				}
				else
				{
					Node* free_node = temp;//ÊÍ·ÅÓÃµÄÖžÕë
					tempNode->right = node;
					free(free_node);
					free_node = NULL;
				}
			}
			else//×óÓÒ×ÓÊ÷¶Œ²»Îª¿Õ
			{
				temp = node;
				/*Íù×ó×ÓÊ÷ ÕÒ×îŽóÖµ*/
				Node* left_max = node;//ÕÒ×îŽóÖµµÄÁÙÊ±ÖžÕë
				left_max = left_max->left;//ÏÈµœ×óº¢×Óœáµã
				while (left_max->right != NULL) 
				{
					temp = left_max;
					left_max = left_max->right;
				}
				node->data = left_max->data;
				if (temp != node)
				{
					temp->right = left_max->left;
					free(left_max);
					left_max = NULL;
				}
				else
				{
					temp->left = left_max->left;
					free(left_max);
					left_max = NULL;
				}
			}
			
		}
		else if(key < node->data)
		{
			delete_node(node->left, key);
		}
		else if (key > node->data)
		{
			delete_node(node->right, key);
		}
	}

    return -1;
}
/*¶šÎ»µœŽýÉŸ³ýœÚµãµÄÇ°Ò»žöœáµã
Node* root ŽÓžùœÚµã¿ªÊŒ
Node* node ŽýÉŸ³ýµÄœáµã
int key ŽýÉŸ³ýÊýŸÝ
*/
Node* prev_node(Node* root, Node* node, int key)
{
	if (root == NULL || node == root)
	{
		return node;
	}
	else
	{
		if (root->left != NULL && root->left->data == key)
		{
			return root;
		}
		else if(root->right != NULL && root->right->data == key)
		{
			return root;
		}
		else if (key < root->data)
		{
			return prev_node(root->left, node, key);
		}
		else
		{
			return prev_node(root->right, node, key);
		}
	}
}