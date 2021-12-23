/*||先进后出，后进先出||*/
#include<stdio.h>
#include<stdlib.h>

#define MAX 5

int* stack;
int top;
int end;

void Init();
void insert(int key);
void deletekey();
void myprint();

int main(int argc, char** argv)
{
    Init();
    insert(0);
    insert(1);
    insert(2);
    insert(3);
    insert(4);
    //insert(5);
    deletekey();
    deletekey();
    myprint();
}

void Init()
{
    stack = (int*)malloc(sizeof(int) * MAX);
    if(stack == NULL){
        printf("出错啦!");
    }
    top = MAX - 1;
    end = -1;//插入第一个元素后，首元素下标就是零，很c语言
}

void insert(int key)
{
    if(end == top){
        printf("已近...已近..满了，不能再插入...数据了\n");
    }else{
        stack[++end] = key;
    }
}

void deletekey()
{
    if(end == -1){
        printf("空空如也");
    }else{
        end--;//因为存的是整型所以直接删除，如果存的是节点得记得free
    }
}

void myprint()
{
    int temp = 0;
    while(temp <= end){
        printf("%d\n",stack[temp]);
        temp++;
    }
}
