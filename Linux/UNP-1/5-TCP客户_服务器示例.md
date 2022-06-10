- 简单的 **回射**客户/服务器
- 全双工的TCP连接
![](https://raw.githubusercontent.com/Daz-3ux-Img/Img-hosting/master/202206101506435.png)

- 通配地址:`INADDR_ANY` **((in_addr_t) 0x00000000)**

![](https://raw.githubusercontent.com/Daz-3ux-Img/Img-hosting/master/202206101532436.png)
- 如图,由于"UNIX信号一般不排队"的性质,在五个信号到达后信号处理函数只运行一次,所以需要一种方法避免无法处理信号而产生僵尸进程
- 方法为调用`waitpid`:(指定WNOHANG选项)
```
waitpid(-1, &stat, WNOHANG);
```

![](https://raw.githubusercontent.com/Daz-3ux-Img/Img-hosting/master/202206101025965.png)

![](https://raw.githubusercontent.com/Daz-3ux-Img/Img-hosting/master/202206101542007.png)

![](https://raw.githubusercontent.com/Daz-3ux-Img/Img-hosting/master/202206101542540.png)
