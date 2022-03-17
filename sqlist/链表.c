#include <stdio.h>
#include <stdlib.h>
/*带头结点的单链表*/
typedef int ElementType;

typedef struct NodeList {
	int element;//存储具体的数值
	struct NodeList* next;//指向下一个结点的指针
}Node;

Node *InitList(Node*);//初始化单向链表,这是一个返回结构体的函数
void head_insert(Node*, ElementType);//像单链表中插入数据
void PrintList(Node*);//遍历单链表
void delete1(Node*,ElementType);//删除指定的元素
int find(Node*, ElementType);//查询元素

int main()
{
	Node* linklist = InitList(linklist);;
	
	head_insert(linklist, 1);
	head_insert(linklist, 2);
	head_insert(linklist, 3);
	delete1(linklist,2);
	PrintList(linklist);
}

Node* InitList(Node* node)//“多此一举初始化”
{
	//初始化一个头结点（只有指针域，数据域看需求）
	node = (Node*)malloc(sizeof(Node));
	if (node == NULL) {
		printf("函数已执行，但内存分配失败，初始化单链表失败\n");
	}
	else {
		node->next = NULL;
	}
	return node;
}

void head_insert(Node* node, ElementType key)
{
	Node* temp = (Node*)malloc(sizeof(Node));
	temp->element = key;
	temp->next = node->next/*NULL*/;
	node->next = temp; 
}

void delete1(Node* node, ElementType key)
{
	//是否有该元素
	int index = find(node,key);
	if (index == -1){
		printf("此元素不存在");
	}
	else{
		int i = 0;
		Node* temp = node;
		while (i < index){
			temp = temp->next;//假设index为3，当i=3时，temp\
            ->next 指向第二个元素
			i++;
		}
		Node* free_node = temp->next; //指向该被释放的第三个元素
		temp->next = temp->next->next;//跨过被释放的直接指
		free(free_node);
	}
}

int find(Node* node, ElementType key)
{
	Node* temp;
	temp = node->next;
	int i = 0;
	while (temp != NULL)
	{
		if (temp->element == key)
		{
			return i;
		}
		temp = temp->next;
		i++;
	}
	return -1;
}

void PrintList(Node* list)
{
	Node* temp;
	temp = list->next;
	while (temp != NULL)
	{
		printf("%d ", temp->element);
		temp = temp->next;
	}
}