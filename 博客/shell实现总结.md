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

# 框架函数
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
- 这个函数就像是一个中转站，将处理完的命令在这里统一识别与处理，一旦发现命令中的“**特征**”，就调用相应的函数，来完成任务
- 特殊命令处理:
    - command_with_Pipe(buf)
    - command_with_OutRe(buf)
    - command_with_InRe(buf)
    - command_with_OutRePlus(buf)
    - command_with_Back(buf)
- 内置命令的处理：
    - 不能在子进程中进行的内置命令手动实现
    - `callcd()`
    - 可以在子进程进行的内置命令交由`execvp()`函数实现

# 具体函数
## callcd()
- 关于`cd - `的实现还有一定的bug,具体的修复我想到了一个绝妙的方法,但这里地方太小我写不下,所以还是交给聪明的读者去修复这个讨厌的bug吧
```c
char oldPath[BUFFSIZE]; 
void callCd(int argc){
    int result = 1;
    if(argc == 1) {
        int ret = chdir("/home");
        return;
    }else{
        int ret = 0;
        int flag_gang = 0;
        int flag_piao = 0;
        int flag;
        for(int i = 0; COMMAND[1][i]; i++) {
            if(COMMAND[1][i] == '-'){
                flag_gang = 1;
            }
            if(COMMAND[1][i] == '~'){
                flag_piao = 1;
            }
        }
        if(flag_gang){// "-"
            if((ret = chdir(oldPath)) == -1){
                my_error("chdir",__LINE__);
            }
            getcwd(oldPath, BUFFSIZE);
        }else if(flag_piao){// "~"
            getcwd(oldPath, BUFFSIZE);
            char *home;
            home = getenv("HOME");
            if((ret = chdir(home)) == -1){
                my_error("chdir",__LINE__);
            }
        }else{
            getcwd(oldPath, BUFFSIZE);
            ret = chdir(COMMAND[1]);
        }
        if(ret){
            return;
        }
    }
}
```

## command_with_OutRe(buf)
- `dup2()`的使用是实现重定向的灵魂
- 利用父子进程完成命令的执行与输出(子进程执行命令并输出到文件后关闭,由父进程负责回收)
```
void command_with_OutRe(char *buf)
{//command > file
    char OutFile[1024];
    memset(OutFile, 0, BUFFSIZE);
    int RedNum = 0;
    for(int i = 0; buf[i]; i++){
        if(buf[i] == '>'){
            RedNum++;
            break;
        }
    }
    if(RedNum != 1){//重定向符号多余1就错误
        my_error("error num of OutRe",__LINE__);
    }
    int fg = 0;
    for(int i =0;i < argc; i++){//与分割好的命令逐个比较，确定重定向文件
        if(strcmp(COMMAND[i], ">") == 0){
            if(i+1 < argc){//因为有argv[argc] == NULL,所以不用<=
                strcpy(OutFile,COMMAND[i+1]);
                fg = i-1;
            }else{
                my_error("missing output file",__LINE__);
            }
        }
    }
    for (int j = 0; j < strlen(buf); j++) {
        if (buf[j] == '>') {
            buf[j - 1] = '\0';
            break;
        }
    }
    
    parse(buf);//重定向符号后面的为文件，所以需要重新解析命令
    // 子进程执行命令,利用重定向将结果输出到文件中
    pid_t pid = fork();
    if(pid < 0){
        my_error("fork",__LINE__);
    }
    if(pid == 0){
        int fd;
        fd = open(OutFile, O_RDWR | O_CREAT | O_TRUNC , S_IRUSR | S_IWUSR);
        if(fd < 0){
            my_error("open",__LINE__);
        }
        dup2(fd,STDOUT_FILENO);//灵魂
        execvp(argv[0], argv);
        if(fd != STDOUT_FILENO){
            close(fd);
        }
        my_error("fault argu",__LINE__);
    }else{
        int status;
        waitpid(pid, &status, 0);
        int err = WEXITSTATUS(status);
        if(err){
            printf("Error:%s\n",strerror(err));
        }
    }
}
```

## command_with_InRe(buf)
- 逻辑与OutRe相同
- 具体代码请参阅源码,本文不再展示

## command_with_OutRePlus(buf)
- 执行追加模式的输出重定向
- 逻辑与OutRe相同
- 使用`O_APPEND`标志即可

## command_with_Back(buf)
- 伪后台执行,此处并非后台执行的真正实现
- 将标准输入与标准输出重定向至`/dev/null`这个特殊的文件夹后再执行命令
- `/dev/null`:Linux系统的垃圾桶
void command_with_Back(char *buf)
{
    char BackBuf[strlen(buf)];
    memset(BackBuf, 0, strlen(buf));
    //提取 & 前的命令
    for(int i = 0; i < strlen(buf); i++){
        BackBuf[i] = buf[i];
        if(buf[i] == '&'){
            BackBuf[i-1] = '\0';
            break;
        }
    }

    pid_t pid = fork();
    if(pid < 0){
        my_error("Fork",__LINE__);
    }

    if(pid == 0){
        //FILE *freopen(const chat*pathname, const char*mode, FILE *stream);
        freopen("/dev/null", "w", stdout); 
        freopen("/dev/null", "r", stdin);
        signal(SIGCHLD, SIG_IGN);
        parse(BackBuf);
        execvp(argv[0], argv);
        my_error("execvp",__LINE__);
    }else{
        exit(0);//父进程直接退出
    }
}

## command_with_Pipe(buf)
- 整个目标中最难实现的部分
- 还需搭配`parse_pipe()`函数一起使用
- 下列代码将附带详细注释
```c
void command_with_Pipe(char *buf)
{
    int i, j;
    int cmd_num = 0, pipe_num = 0;
    //使用二维数组将fd符存储起来,pipe()函数支持传入一个数组
    int fd[16][2];
    char *curcmd;
    char *nextcmd = buf;
    for (int k = 0; buf[k]; k++){
        if(buf[k] == '|'){//统计多重管道具体数目
            pipe_num++;
        }
    }
    //使用strsep命令分割命令
    while ((curcmd = strsep(&nextcmd, "|"))){
        flag_out = 0;
        flag_in = 0;
        if(parse_pipe(curcmd, cmd_num++) < 0){
            cmd_num--;
            break;
        }
        if(cmd_num == 17)//16根管道最多支持17条命令
            break;
    }   

    for (i = 0; i < pipe_num; i++){//创建管道
        if(pipe(fd[i])){
            my_error("pipe", __LINE__);
        }
    }

    pid_t pid;
    for (i = 0; i <= pipe_num; i++){ //管道数目决定创建子进程个数
        if((pid = fork()) == 0)
            break;
    }

    if(pid == 0){
        if(pipe_num != 0){
            if (i == 0){ //第一个创建的子进程
            //管道的输入为标准输入
                dup2(fd[0][1], STDOUT_FILENO);
                close(fd[0][0]);

                for (j = 1; j < pipe_num; j++){
                    close(fd[j][0]);
                    close(fd[j][1]);
                }
            }else if (i == pipe_num){ //最后一个创建的子进程
            //管道的输出为标准输出
                dup2(fd[i-1][0], STDIN_FILENO);
                close(fd[i-1][1]);

                for (j = 0; j < pipe_num - 1; j++){
                    close(fd[j][0]);
                    close(fd[j][1]);
                }
            }else{
                //重定中间进程的标准输入至管道读端
                dup2(fd[i-1][0], STDIN_FILENO); 
                close(fd[i-1][1]);
                //重定中间进程的标准输出至管道写端
                dup2(fd[i][1], STDOUT_FILENO);
                close(fd[i][0]);

                for (j = 0; j < pipe_num; j++){ //关闭不使用的管道读写两端
                    if (j != i || j != (i - 1)){
                        close(fd[j][0]);
                        close(fd[j][1]);
                    }
                }
            }
        }
        if(flag_in){//用户指定了输入重定向
            int file_fd = open(cmd[i].in, O_RDONLY);
            dup2(file_fd, STDIN_FILENO);
        }
        if(flag_out){//用户使用了输出重定向
            int file_fd = open(cmd[i].out, O_WRONLY | O_CREAT | O_TRUNC, 0644);
            dup2(file_fd, STDOUT_FILENO);
        }
        execvp(cmd[i].argv[0], cmd[i].argv); //执行用户输入的命令
        my_error("execvp",__LINE__);
    }else{// parent
    //关闭父进程两侧管道
        for (i = 0; i < pipe_num; i++){
                close(fd[i][0]);
                close(fd[i][1]);
            }
        for(int i = 0; i < cmd_num; i++){
            wait(NULL);
        }
    }
}
```

### parse_pipe()
- 为多重管道的实现提供基础,管道符两侧可加空格也可不加空格
```c
int flag_out = 0;
int flag_in = 0 ;
int parse_pipe(char *buf,int cmd_num)
{
    int n = 0;
    char *p = buf;
    cmd[cmd_num].in = NULL;
    cmd[cmd_num].out = NULL;
    while(*p != '\0'){
        if(*p == ' '){
            *p++ = '\0';
            continue;
        }
        //判断管道是否需要与重输出重输出搭配使用
        if(*p == '<'){
            *p = '\0';
            flag_in = 1;
            while(*(++p) == ' '){
                ;
            }
            cmd[cmd_num].in = p;
            continue;
        }
        if(*p == '>'){
            *p = '\0';
            flag_out = 1;
            while(*(++p) == ' ');
            cmd[cmd_num].out = p;
            continue;
        }
        //去除空格
        if(*p != ' ' && ((p == buf) || *(p-1) == '\0')){
            if(n < MAX_CMD){
                cmd[cmd_num].argv[n++] = p++;
                continue;
            }else{
                return -1;
            }
        }
        ++p;
    }
    if(n == 0){
        return -1;
    }
    return 0;
}
```

# 信号处理 与 错误处理
- 使用`signal()`调用忽略所有信号
```c
void my_signal()
{
    signal(SIGHUP, SIG_IGN);
    signal(SIGINT, SIG_IGN);
    signal(SIGTTIN, SIG_IGN);
    signal(SIGTTOU, SIG_IGN);
    signal(SIGTSTP, SIG_IGN);
}
```

- 输出错误原因与行号
```c
void my_error(char *string, int line)
{// 用法示例: myerror("malloc", __LINE__);
    fprintf(stderr, "Line:%d,error:\n", line);
    fprintf(stderr, "%s:%s\n", string, strerror(errno));
    printf("***********************\n");
}
```

-----
# 参考:
1. 《UNIX/Linux编程实践教程》
2. 《TLPI》
3. [Linux——实现简单的交互式shell](https://blog.csdn.net/xiaoan08133192/article/details/105099371)