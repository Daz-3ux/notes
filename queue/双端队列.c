#include<stdio.h>
#include<stdlib.h>

void InitQueue();
int isEmpty();
int isFull();
void left_insert(int);//左插
void right_insert(int);//右插
void left_delete();
void right_delete();


int* queue;
int maxSize;
int size;//当前元素个数
int left;//左指针
int right;//右指针

int main()
{
	InitQueue();
	right_insert(1);
	right_insert(2);
	right_insert(3);
	left_delete();
	left_delete();
}


void InitQueue()
{
	queue = (int*)malloc(5 * sizeof(int));
    //printf("Queue: %d\n",_msize(queue));
	maxSize = 5;
	size = 0;
}

int isEmpty()
{
	if (size == 0)
	{
		return 1;
	}
	else
	{
		return 0;
	}
}
int isFull()
{
	if (size == maxSize)
	{
		return 1;
	}
	else
	{
		return 0;
	}
}
void left_insert(int key)
{
	if (isFull())
	{
		printf("FULL了大哥");
	}
	else
	{
		if (isEmpty())
		{
			left = right = 0;
			queue[left] = key;
		}
		else
		{
			if (left == 0)
			{
				left = maxSize;
			}
			queue[--left] = key;
		}
		size++;
	}
}
void right_insert(int key)
{
	if (isFull())
	{
        printf("FULL了大哥");
	}
	else
	{
		if (isEmpty())
		{
			left = right = 0;
			queue[right] = key;
		}
		else
		{
			if (right == maxSize - 1)
			{
				right = -1;
			}
			queue[++right] = key;
			
		}
		size++;
	}
}

void left_delete()
{
	if (isEmpty())
	{
	}
	else
	{
		if (left == maxSize - 1)
		{
			printf("%d", queue[left]);
			left = 0;
			size--;
		}
		else
		{
			printf("%d", queue[left]);
			left++;
			size--;
		}
	}
}

void right_delete()
{
	if (isEmpty())
	{
	
	}
	else
	{
		if (right == 0)
		{
			printf("%d", queue[right]);
			right = maxSize - 1;
			size--;
		}
		else
		{
			printf("%d", queue[right]);
			right--;
			size--;
		}
	}
}