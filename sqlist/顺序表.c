#include <stdio.h>
#include <stdlib.h>
#define Size 5          //初始化以及扩容的长度
#define ElementType int //int 取别名

typedef struct ArrayList
{
    ElementType *e; //存放数据元素的数组
    int length;     //顺序表中数据的总数量
    int size;       //当前元素的数量
} MyArray;

MyArray initArray();  //初始化操作,返回类型为结构体的函数
void add(MyArray *arr, ElementType key);                            //添加一个元素
void delete1(MyArray *arr, ElementType key);                        //删除指定的元素
int find(MyArray *arr, ElementType key);                            //查询指定的元素
void modification(MyArray arr, ElementType key/*ElementType index*/); //修改指定的元素
void show(MyArray arr);                                             //展示当前顺序表的元素
int index(MyArray *arr, ElementType key);                           //寻找关键字所在的下标

int main()
{
    MyArray a;
    a = initArray();
    add(&a, 1);
    add(&a, 2);
    add(&a, 3);
    add(&a, 4);
    add(&a, 5);
    if(a.size % 5 == 0){
        show(a);
    }
    add(&a, 6);
    show(a);
}

MyArray initArray()
{
    MyArray array;

    array.e = (ElementType *)malloc(Size * sizeof(int));//初始化数组，分配一块int类型的空间
    if (!array.e)
    {
        printf("初始化失败");
        exit(0);
    }
    array.length = 5;
    array.size = 0;

    return array;
}

/*
MyArray* arr :待添加元素的数组
ElementType key : 待添加的关键字
*/
void add(MyArray *arr, ElementType key)
{
    if (arr->size < arr->length)
    {
        arr->e[arr->size] = key;
        arr->size++;
    }
    else
    {
        printf("元素已满，按e扩容5个单位");
        char ch;
        scanf("%c",&ch);
        if(ch == 'e'){
            arr->length += Size;
            arr->e[arr->size] = key;
            arr->size++;
        }
    }
}

/*
MyArray* arr 待删除的数组
ElementType key 待删除的元素
*/
void delete1(MyArray *arr, ElementType key)
{
    //找到关键字所在的下标
    int temp_index = index(arr, key);
    //判断关键字是否存在
    if (temp_index != -1)
    {
        //待删除的关键字存在 移动
        while (temp_index < arr->size)
        {
            arr->e[temp_index] = arr->e[temp_index + 1];
            temp_index++;
        }
        //移动完 数组元素 - 1
        arr->size--;
    }
    else
    {
        printf("没有该元素");
    }
}

int index(MyArray *arr, ElementType key)
{
    int i = 0; //数组下标变量 从0开始
    while (i < arr->size)
    {
        //如果第i个元素和关键字相等 返回下标i
        if (arr->e[i] == key)
        {
            return i;
        }
        //下标+1
        i++;
    }
    return -1; //没有找到下标
}

int find(MyArray *arr, ElementType key)
{
    if (arr->size == 0)
    {
        printf("当前没有元素");
    }
    else
    {
        int i = 0;
        while (i < arr->size)
        {
            if (arr->e[i] == key)
            {
                return 1;
            }
        }
    }

    return 0;
}
//修改元素：根据值修改 根据下标修改
void modification(MyArray arr, ElementType key)
{
}

void show(MyArray arr)
{
    if (arr.size == 0)
    {
        printf("当前没有元素");
    }
    else
    {
        int i = 0;
        while (i < arr.size)
        {
            printf("%d ", arr.e[i]);
            i++;
        }
    }
}