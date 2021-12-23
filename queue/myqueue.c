#include<stdio.h>
#include<stdlib.h>

#define MAXSIZE 100

void Init();
void Insert(int key);
void deletekey();
void myprint();
int isFull();
int isEmpty();

int* queue;
int front;
int end;

int main(int argc, char **argv)
{
    Init();
}

void Init()
{
    queue = (int*)malloc(sizeof(int) * MAXSIZE);
}

void Insert()
{
    
}