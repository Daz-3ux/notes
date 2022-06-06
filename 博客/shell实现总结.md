# intro
- 争取在自己的简易shell中可以实现以下命令
```
xxx@xxx ~ $ ./xxx-super-shell
xxx@xxx ~ $ echo ABCDEF
xxx@xxx ~ $ echo ABCDEF > ./1.txt
xxx@xxx ~ $ cat 1.txt
xxx@xxx ~ $ ls -t >> 1.txt
xxx@xxx ~ $ ls -a -l | grep abc | wc -l > 2.txt
xxx@xxx ~ $ python < ./1.py | wc -c
xxx@xxx ~ $ mkdir test_dir
xxx@xxx ~/test_dir $ cd test_dir
xxx@xxx ~ $ cd -
xxx@xxx ~/test_dir $ cd -
xxx@xxx ~ $ ./xxx-super-shell # shell 中嵌套 shell
xxx@xxx ~ $ exit
xxx@xxx ~ $ exit
```
## 任务解析
- 任务目标:
    - 实现 `管道` (也就是 |)
    - 实现 `输入输出重定向`(也就是 < > >>)
    - 实现 `后台运行`（也就是 & ）
    - 实现 `cd`，要求支持能切换到 **绝对路径**，**相对路径**和**支持 `cd -`**
    - `屏蔽`一些信号（如 ctrl + c 不能终止）
    - 界面`美观`
    - **不得出现内存泄漏，内存越界等错误**

- 核心为掌握`Linux系统编程`中`进程`的相关部分,能够正确调用相应API完成任务,保证每个函数的逻辑正确

- 难点:
    - 最难的就是实现管道,而且是多重管道ಥʖ̯ಥ,关于实现管道的的**基本原理**可以参考我的这篇博客——[Linux下实现一个简单的单向管道及其理解](https://blog.csdn.net/m0_61536749/article/details/124047437?spm=1001.2014.3001.5501)
    - 代码的`去耦合`与`功能分块`也是一个难点，但这次做的还行
    - 写完之后再回想,想不起来很多当时觉得难的不行的东西了,其实经历完绝望之谷之后,不仅技术会提升,心态也会更好

# 主要函数
## main()
- 从main函数入手来剖析我们需要实现的所有功能是一个不错的选择
```c
int main()
{
    my_signal();                                //屏蔽信号
    while(1){                                   //一直循环,直至手动退出
        char place[BUFFSIZE];                   //存放当前路径
        getcwd(place, BUFFSIZE);                //获取当前路径
        printf(BEGIN(36,36)"%s:"CLOSE, place);  //色彩显示
        // readline库的使用
        char *command = readline(BEGIN(33,33)"ypd-super-shell ￥$ "CLOSE);
        if(!command){
            my_error("readline",__LINE__);
        }
        add_history(command);                   //存放历史命令
        write_history(NULL);                    //写入历史命令
        parse(command);                         //解析命令
        do_cmd(argc,argv);                      //执行命令
        argc = 0;                               //将argc置0,重新读取命令
        free(command);                          //释放堆区空间,等待重新分配(由readline库函数完成)
    }
}
```

- 在main函数中,我们首先会注意到`readline`这个完全陌生的库,它帮助我们完成了很多工作:
    - 用户命令的获取
    - 动态内存的申请
    - 历史命令的存写
    - 那么,我们怎么才能掌握这么有用的库呢,这个问题的答案碍于篇幅我就不再展开，请大家参考这篇博客——[readline库的简单使用](https://blog.csdn.net/xuancbm/article/details/81436681?ops_request_misc=&request_id=&biz_id=102&utm_term=readline&utm_medium=distribute.pc_search_result.none-task-blog-2~all~sobaiduweb~default-1-81436681.142^v11^control,157^v13^control&spm=1018.2226.3001.4187)

- `parse()` : 顾名思义，这个函数就是用来解析命令的
- `do_cmd()`：核心函数,执行用户输入的命令

## parse(command)
- 解析`command`

SHOW ME THE CODE:
```c
void parse(char *command)
{
/*
command 为用户输入的命令
*/
    //初始化argv与argc
    for(int i = 0; i < MAX_CMD; i++){
        argv[i] = NULL;
        for(int j = 0;j < MAX_CMD_LEN; j++){
            COMMAND[i][j] = '\0';
        }
    }
    argc = 0;//命令数计数器
    memset(backupCommand,0,sizeof(backupCommand));//非常重要,因为漏了这一句被整自闭了好久
    strcpy(backupCommand, command);//备份命令
    
    int j = 0;
    int len = strlen(command);
    for(int i = 0; i < len; i++){
        if(command[i] != ' '){
            COMMAND[argc][j++] = command[i];
        }else{//command[i] == ' '
            if(j != 0){//j为0则为连续空格情况
                COMMAND[argc][j] = '\0';
                argc++;
                j = 0;
            }
        }
    }
    if(j != 0){//处理命令行末尾
        COMMAND[argc][j] = '\0';
    }

    /*处理__内置命令__  | isspace()调用是为了处理空格*/
    argc = 0;
    int flg = OUT;
    for(int i = 0; command[i] != '\0'; i++){
        if(flg == OUT && !isspace(command[i])){
            flg = IN;
            argv[argc++] = command + i;
        }else if(flg == IN && isspace(command[i])){
            flg = OUT;
            command[i] = '\0';
        }
    }
    argv[argc] = NULL;
}
```
- 依靠注释这段代码并不难理解,其为接下来的所有功能提供基础,将同一段命令解析两次是为了满足接下来所有函数的要求
    - COMMAND:通过与COMMAND数组匹配,确定
    - argv:

## do_cmd(argc, argv)
- 其argc与argv与main函数无关,为`parse()`函数解析出来的结果

SHOW ME THE CODE:
```c
void do_cmd(int argc, char **argv)
{
    char buf[1024];//存放原始命令
    ///识别管道命令
    for(int i = 0;i < BUFFSIZE; i++){
        if(backupCommand[i] == '|'){
            strcpy(buf,backupCommand);
            command_with_Pipe(buf);
            return;
        }
    }
    //识别输出重定向
    for(int j = 0;j < MAX_CMD; j++){
        if(strcmp(COMMAND[j], ">") == 0){
            strcpy(buf,backupCommand);
            command_with_OutRe(buf);
            return;
        }
    }
    //识别输入重定向
    for(int j = 0;j < MAX_CMD; j++){
        if(strcmp(COMMAND[j], "<") == 0){
            strcpy(buf,backupCommand);
            command_with_InRe(buf);
            return;
        }
    }
    //识别追加写重定向
    for(int j = 0;j < MAX_CMD; j++){
        if(strcmp(COMMAND[j], ">>") == 0){
            strcpy(buf,backupCommand);
            command_with_OutRePlus(buf);
            return;
        }
    }
    //识别后台运行命令
    for(int j = 0;j < MAX_CMD; j++){
        if(strcmp(COMMAND[j], "&") == 0){
            strcpy(buf,backupCommand);
            command_with_Back(buf);
            return;
        }
    }

    pid_t pid;
    //匹配shell内置命令
    //!!!!下列命令交给子进程执行的话没有意义,所以单独列出自己实现
    if(strcmp(COMMAND[0], "cd") == 0){
        callCd(argc);
    }else if(strcmp(COMMAND[0], "history") == 0){
        HIST_ENTRY **history = NULL;//readline
        history = history_list();
        for(int i = 0; history[i] != NULL; i++){
            printf("%s\n",history[i] -> line);
        }
    }else if(strcmp(COMMAND[0], "exit") == 0){
        printf("--------------GoodBye---------------\n");
        exit(0);
    }else{
        switch(pid = fork()){
            case -1:
                my_error("fork",__LINE__);
            case 0://子进程执行任务
                execvp(argv[0],argv);
                my_error("execvp",__LINE__);
            default:{//父进程等待子进程结束
                int status;
                waitpid(pid, &status, 0);//等待任何组进程
                int err_num = WEXITSTATUS(status);//宏用来指出子进程是否正常退出
                if(err_num){
                    printf("Error: %s\n", strerror(err_num));
                }
            }
        }
    }
}
```


