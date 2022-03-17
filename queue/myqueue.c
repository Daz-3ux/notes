#include<stdio.h>
#include<stdlib.h>

#define MAXSIZE 100

void Init();
void Insert(int key);
void deletekey();//只有队满了才能用
void myprint();
int isFull();
int isEmpty();

int* queue;
int front;
int end;

int main(int argc, char **argv)
{
    Init();
    Insert(0);
    Insert(1);
    Insert(2);
    //deletekey();
    myprint();
}

void Init()
{
    queue = (int*)malloc(sizeof(int) * MAXSIZE);
}

int isEmpty()
{
    if(front == end){
        return 1;
    }
    return 0;
}

int isFull()
{
    if((end + 1) % MAXSIZE == front){
        return 1;
    }
    return 0;
}

void Insert(int key)
{
    if(isFull()){
        printf("FULL\n");
    }else{
        end = (end + 1) % MAXSIZE;
        queue[end] = key;
    }
}

void deletekey()
{
    if(isEmpty()){
        printf("ENPTY\n");
    }else{
        front = (front + 1) % MAXSIZE;
    }
}

void myprint()
{   
    int temp = 1;//从1开始计数，保证逻辑
    while(temp <= end){
        printf("%d\n", queue[temp]);
        temp++;
    }
}