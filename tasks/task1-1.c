/*用链表实现加法~~吃饱了撑的~~*/

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
    Node* LinkList1 = Init(LinkList1);
    Node* LinkList2 = Init(LinkList2);
    int n,m;
    scanf("%d %d",&n,&m);
    int cnt1 = 0,cnt2 = 0;
    int max;
    for(int i = n;i > 0 ;i /= 10){
        head_insert(LinkList1,i % 10);
        cnt1++;
    }
    for(int i = m;i > 0 ;i /= 10){
        head_insert(LinkList2,i % 10);
        cnt2++;
    }
    if(cnt1 > cnt2){
        max = cnt1;
    }else{
        max = cnt2;
    }

    LinkList1 = LinkList1 -> next;
    LinkList2 = LinkList2 -> next;

    int nums[max];
    for(int i = 0;i < max;i++){
        nums[i] = LinkList1 -> element + LinkList2 -> element;
        LinkList1 = LinkList1 -> next;
        LinkList2 = LinkList2 -> next;
    }

    for(int i = 0; i < max;i++){
        printf("%d",nums[i]);
    }


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