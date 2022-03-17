#include<stdio.h>
#include<stdlib.h>
typedef struct LinkList {
	int element;
	struct LinkList* next;
}List;//看到33分钟了

List* InitList();
void head_insert(List* head, int key);
int main() 
{
	List* list = InitList();
	
	head_insert(list, 1);
	head_insert(list,5);
	printf("%d", list->next->element);

}

List* InitList()
{
	List* head = (List*)malloc(sizeof(List));
	head->element = 0;
	head->next = head;
	return head;
}

void head_insert(List* head,int key)
{
	List* new_node = (List*)malloc(sizeof(List));
	new_node->element = key;
	new_node->next = head->next;
	head->next = new_node;
	head->element++;
}