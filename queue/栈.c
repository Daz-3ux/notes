/*  
    栈的全名：堆栈。
    数据结构的栈与内存中的栈不一样
    数据结构的栈：抽象数据类型ADT
    物理中的栈：CPU不够用了找内存条（寄存器）放一下临时数据的地方
    ||先进后出，后进先出||
    栈顶栈底，栈空栈满
*/

#include<stdio.h>
#include<stdlib.h>

#define SIZE 5//可以自己设置也可以让用户来决定

int* stack;//用数组来表示栈的操作
int top;//栈顶指针 因为是数组所以用下标表示 用来判断数组是否已满
int end;//栈底指针                      控制数据的存取

int main(int argc, char **argv)
{

}

void InitStack()
{
    stack = (int*)malloc(sizeof(int));
    if(stack == NULL){
        printf("ERROR");
    }
    top = SIZE - 1;
    end = -1;
}

void insert(int key)//进栈
{
    if(end == top){
        printf("满了大哥");
    }else{
        stack[end++] = key;
    }
}

void deletekey(int key)//出栈
{
    if(end == -1){
        printf("空了大哥");
    }else{
        //如果存的是节点记得free；
        end--;//从表面上删了就行了
    }
}
