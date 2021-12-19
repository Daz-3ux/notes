#include<stdio.h>
#include<stdlib.h>

typedef struct node_list{
    int element;
    struct node_list* next;
}Node;

Node* Init(Node*);
void head_insert(Node* node,int key);
int sums[100];

int main(int argc, char** argv)
{
    Node* LinkList = Init(LinkList);
    int n,m;
    

}

Node* Init(Node* new_node)
{
    new_node =(Node*) malloc(sizeof(Node));
    if(new_node == NULL){
        printf("ERROR");
    }else{
        new_node->element = 0;
        new_node->next = NULL;
    }
    return new_node;
}

void head_insert(Node* node,int key)
{   
    Node* new_node =(Node*)malloc(sizeof(Node));
    if(new_node == NULL){
        printf("ERROR");
    }
    new_node -> element = key;
    new_node -> next = node->next;
    node -> next = new_node;
}