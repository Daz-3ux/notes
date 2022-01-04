#include<stdio.h>

//路径压缩：
/*
因为在特殊状况下 这棵树可能是一个巨长的链，所以设链的最后一个节点为x
每次执行find相当于遍历整个链条
只需把遍历过的节点都改为根的子节点，查询会块很多
*/

#define MAXSIZE 100

int uset[MAXSIZE];//定义一个足够长数组,用下标表示节点

int rank[MAXSIZE];//树的高度

/*构造并查集*/
void makeSet(int size);//size为节点数
int find(int i);
void unite(int x,int y);

int main(int argc, char** argv)
{
    // makeSet(MAXSIZE);
    // unite(3,4);//4为3的子树
    // unite(2,3);//2为3的子树
    // unite(0,2);//~~2为0的子树~~错了，正确的是0是3的子树，不愧是路径压缩
    // unite(4,5);//5为3的子树
    // int i = find(4);
    // int i1 = find(2);
    // int i2 = find(0);
    // int i3 = find(5);
    // printf("%d %d %d %d",i,i1,i2,i3);

    /*可以体现非路径压缩并查集的劣势的情况如下*/
    makeSet(MAXSIZE);
    unite(3,4);
    unite(3,5);
    unite(3,2);
    /*在路径压缩后得到了优化*/
    int i = find(3);
    printf("%d\n",i);
}

void makeSet(int size)
{
    for(int i = 0;i < size; i++) {
        //每一个元素都指向自己，各自为各自的代表
        uset[i] = i;
        //树的高度
        rank[i] = 0;
    }
}

/*找到元素所在集合代表 如果位于同一集合则不合并*/
int find(int i)
{
    if(i == uset[i]){
        return i;
    }
    return uset[i] = find(uset[i]);//在第一次查找时直接将节点连接到根
}

void unite(int x,int y)
{
    //先找到节点所对应代表
    int i = find(x);
    int j = find(y);
    if(i == j){
        return;//不合并
    }
    //判断两棵树的高度 再决定谁为子树
    if (rank[i] < rank[j]){
		uset[i] = j;
	}else{
		uset[j] = i;
		if (rank[i] == rank[j]){
			rank[i]++;
		}
	}
}