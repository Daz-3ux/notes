#include<stdio.h>
#include<stdlib.h>

int isEmpty();
int isFUll();

void left_insert(int);//左插
void right_insert(int);
void left_delete();
void right_delete();

void InitQueue();

int *queue; //指针数组 queue是一个指针，指向一个元素为int类型的数组
int maxSize; //最大元素个数
int size; //当前元素个数
int left;//左指针
int right;//右指针

int main(int argc, char *argv[])
{
    InitQueue();
    right_insert(1);
    left_insert(3);
}

void InitQueue()
{
    int maxSize = 5;
    queue = (int*)malloc(maxSize * sizeof(int));
    size = 0;
}

int isEmpty()//队满
{
    if(size == 0){
        return 1;
    }else{
        return 0;
    }
}

int isFull()//队空
{
    if(size == maxSize){
        return 1;
    }else{
        return 0;
    }
}

void left_insert(int key)
{
    if(isFull()){
        printf("满");
    }else{
        if(isEmpty()){
            left = right = 0;
            queue[left] = key;
        }else{
            if(left == 0){
                left = maxSize;
            }
            queue[--left] = key;
        }
        size++;
    }
}

void right_insert(int key)
{
    if(isFull){
        printf("满");
    }else{
        if(isEmpty()){
            left = right = 0;
            queue[right] = key;
        }else{
            if(right == maxSize -1){
                right = -1;
            }
            queue[++right] = key;
        }
        size++;
    }
}

void left_delete()
{
    if(isEmpty()){
        printf("空");
    }else{
        if(left == maxSize -1){
            printf("%d",queue[left]);
            left = 0;
            size--;
        }else{
            printf("%d",queue[left]);
            left++;
            size--;
        }
    }
}

void right_delete()
{
    if(isEmpty()){
        printf("空");
    }else{
        if(right == 0){
            printf("%d",queue[right]);
            right = maxSize -1;
            size--;
        }else{
            printf("%d",queue[right]);
            right--;
            size--;
        }
    }
}