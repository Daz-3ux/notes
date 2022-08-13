@[TOC](从pwd命令的实现理解ls-R的逻辑)

# 引言
- 本文不再赘述ls命令与递归
- ls -R在man手册中的定义为：
```
list subdirectories recursively
```
即通过递归的手段列出所有子目录

在我们为-R指定好目录后，其便会开始展示从当前目录开始，**往下**每一层的所有子目录，在代码实现中关于递归的部分是比较难以理解的一个点
- pwd在man手册中的定义为：
```
print name of current/working directory
```
即打印当前工作目录

pwd命令想要实现，就需要从当前目录开始，**向上**遍历直至抵达当前目录的根目录，pwd命令同样可以通过递归实现

- 区别于-R的自上向下，pwd的递归为自下向上，且pwd的实现较为简单，我们可以通过对pwd的学习来帮助我们理解-R的逻辑

---

# pwd的逻辑
- 追踪链接，读取目录，逐目录沿文件树向上追踪
- 每步都记录“.”的inode号
- 在父目录中查找记录值inode对应的名称
- 达到树的顶端即停止
- 逻辑细节请参见代码实现
- 图示：
![pwd的逻辑](https://img-blog.csdnimg.cn/198fa7f19553470b87a7b5e2af99957a.png?x-oss-process=image/watermark,type_d3F5LXplbmhlaQ,shadow_50,text_Q1NETiBAeXBkLg==,size_20,color_FFFFFF,t_70,g_se,x_16#pic_center)



# pwd的代码实现
```c
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<dirent.h>

ino_t get_inode(char *);
void printpathto(ino_t);
void inum_to_name(ino_t , char *, int );

int main()
{
	printpathto(get_inode( "." ));//打印路径
	putchar('\n');//换行
	return 0;
}

void printpathto(ino_t this_inode)
//利用inode号去递归，保存每一次记录到的文件夹名，从而得到路径
{
	ino_t	my_inode;
	char	its_name[BUFSIZ];

	if (get_inode("..") != this_inode)//this_inode的初始值参见12行
	{
		chdir("..");//利用chdir()函数跳转至父目录

		inum_to_name(this_inode,its_name,BUFSIZ);//利用inode号获取当前目录名称

		my_inode = get_inode( "." );//获取父目录inode号，更新my_node
		printpathto( my_inode );//递归
		printf("/%s", its_name );//递归至顶层，所以打印从初始目录的根目录开始，符合逻辑
	}
}

void inum_to_name(ino_t inode_to_find , char *namebuf, int buflen)
/*
在当前文件夹中找到inode号与记录值this_inode匹配的文件夹
其就是下一级目录
保存其名字到namebuf数组
*/
{
	DIR	*dir_ptr;//DIR结构体
	struct dirent *direntp;//dirent结构体

	dir_ptr = opendir(".");
	if (dir_ptr == NULL){//处理失败情况
		perror(".");
		exit(1);
	}

    //在文件夹中搜索指定的inode号
	while ((direntp = readdir(dir_ptr)) != NULL){
		if ( direntp->d_ino == inode_to_find ){
			strncpy( namebuf, direntp->d_name, buflen);
			namebuf[buflen-1] = '\0';   /* just in case */
			closedir( dir_ptr );
			return;
		}
    }
    //处理失败情况
    fprintf(stderr, "error looking for inum %ld\n", inode_to_find);
    exit(1);//程序异常终止信号
}

ino_t get_inode( char *fname )
//返回inode号
{
	struct stat info;

	if (stat(fname, &info ) == -1){
		fprintf(stderr, "Cannot stat ");
		perror(fname);
		exit(1);
	}
	return info.st_ino;
}
```

# ls -R逻辑
此处逻辑仅为简单逻辑，具体实现细节请参看：[ls实现](https://blog.csdn.net/m0_61536749/article/details/123363236?spm=1001.2014.3001.5501)
1. 从指定目录开始递归（若未指定则从当前目录开始）
2. 首先遍历展示当前目录内容，若目录内容之一为目录，则递归进入
3. 进入执行2，执行完毕后返回父目录
4. 遍历完所有目录的所有文件
5. 采用**尾递归**

---
# 参考：
- 《Unix/Linux编程实践教程》