#include<stdio.h>
#include<stdlib.h>

#define MAXSIZE 100

int uset[MAXSIZE];//定义一个足够长数组,用下标表示节点

/*构造并查集*/
void makeSet(int size);//size为节点数

int main(int argc, char** argv)
{

}

void makeSet(int size)
{
    for(int i = 0;i < size; i++) {
        //每一个元素都指向自己，各自为各自的代表
        uset[i] = i;
    }
}

/*找到元素所在集合代表 如果位于同一集合则不合并*/
int find(int i)
{
    if(i == uset[i]){
        return i;
    }
    return find(uset[i]);
}

void unite(int x,int y)
{
    //先找到节点所对应代表
    int x = find(x);
    int y = find(y);
    if(x == y){
        return;//不合并
    }
    uset[x] = y;
}