/*
    特殊线性表，||管道||
    先进先出（尾进头出）
    双端队列：两个指针控制四种操作{
        左插右出，右插左出：队列
        左插左出，右插右出：栈
    }
    第“1”个格子：保真代码逻辑性
    缓存淘汰机制：时间；Redis：次数；底层都为队列；
    延迟队列，阻塞队列
 */ 

#include<stdlib.h>
#include<stdio.h>

#define maxSize 5//当前容量

int isEmpty();//队空
int isFull();//队满
void insert(int);
void deletekey();

int* queue;//用数组实现队列
int front;//头指针，因为是数组，所以用下标代表指针
int end;//尾指针


int main(int argc, char* argv[])
{
    insert(01);
}

int isEmpty()//队空
{

    if(front == end){
        return 1;
    }else{
        return 0;
    }
}


int isFull()//队满
{
    if((end+1) % maxSize){
        return 1;
    }else{
        return 0;
    }
}


void insert(int key) 
{
    if(isFull()){
        printf("FULL");
    }else{
        end  =(end+1) % maxSize;
        queue[end] = key;
    }
}


void deletekey()
{
    if(isEmpty()){
        printf("EMPTY");
    }else{
        front = (front + 1) % maxSize;
    }
}

