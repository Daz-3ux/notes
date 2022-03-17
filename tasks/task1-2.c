#include<stdio.h>
#include<stdlib.h>
#include<string.h>
/*压栈，然后判断*/

#define SIZE 5//可以自己设置也可以让用户来决定

int* stack;//用数组来表示栈的操作
int top;//栈顶指针 因为是数组所以用下标表示 用来判断数组是否已满
int end;//栈底指针                      控制数据的存取

void InitStack();
void insert(char key);
void deletekey(char key);

int main(int argc, char **argv)
{
    InitStack();

}

void InitStack()
{
    stack = (char*)malloc(sizeof(char));
    if(stack == NULL){
        printf("ERROR");
    }
    top = SIZE - 1;
    end = -1;
}

void insert(char key)//进栈
{
    if(end == top){
        printf("满了大哥");
    }else{
        stack[end++] = key;
    }
}

void deletekey(char key)//出栈
{
    if(end == -1){
        printf("空了大哥");
    }else{
        //如果存的是节点记得free；
        end--;//从表面上删了就行了
    }
}
