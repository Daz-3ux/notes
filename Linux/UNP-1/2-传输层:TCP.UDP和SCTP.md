- TCP连接是全双工的,需要的话也可以转为单工连接
- UDP可以是全双工的

- 三次握手:(TCP连接建立)
![](https://raw.githubusercontent.com/Daz-3ux-Img/Img-hosting/master/202205302136625.png)

- 四次握手(四分组交换序列):(TCP连接终止)
![](https://raw.githubusercontent.com/Daz-3ux-Img/Img-hosting/master/202205302140419.png)
![](https://raw.githubusercontent.com/Daz-3ux-Img/Img-hosting/master/202205302140494.png)

- TCP状态转换
![](https://raw.githubusercontent.com/Daz-3ux-Img/Img-hosting/master/202205302141940.png)

- TCP_WAIT状态存在的两个理由:
    - 可靠的实现TCP全双工连接的终止
    - 允许老的重复分节在网络中消逝

