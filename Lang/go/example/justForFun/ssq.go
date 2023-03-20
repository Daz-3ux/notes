// v1
// generate by Claude+

package main

import (
"fmt"
"io/ioutil"
"net/http"
"regexp"
"strconv"
"time"
)

// 爬取数据
func crawler() string {
resp, err := http.Get("https://www.zhcw.com/kjxx/ssq/")
if err != nil {
fmt.Println(err)
}
defer resp.Body.Close()
body, err := ioutil.ReadAll(resp.Body)
if err != nil {
fmt.Println(err)
}
reg := regexp.MustCompile(<span class="red">[\d]{6}</span><span class="blue">[\d]{1}</span>)
result := reg.FindString(string(body))
return result
}

// 用户输入
var userInput string
var userRedBalls []int
var userBlueBall int

// 结果写入
func output(matchRed int, isMatchBlue bool) {
// 20xx年x月x日:x个红球匹配,(是/否)蓝球匹配
now := time.Now().Format("2006年01月02日")
var blueMatch string
if isMatchBlue {
blueMatch = "是"
} else {
blueMatch = "否"
}
output := now + ":" + strconv.Itoa(matchRed) + "个红球匹配," + blueMatch + "蓝球匹配\n"
ioutil.WriteFile("output.txt", []byte(output), 0644)
}

func main() {
// 解析用户输入的红蓝球号码
fmt.Print("请输入六个红球号码,以空格分隔:")
fmt.Scanln(&userInput)
userRedBalls = make([]int, 6)
for i := 0; i < 6; i++ {
userRedBalls[i], _ = strconv.Atoi(userInput[i2 : i2+2])
}
fmt.Print("请输入蓝球号码:")
fmt.Scanln(&userBlueBall)

// 定期爬取开奖数据并匹配
for {
    result := crawler() 
    winRedBalls := make([]int, 6) 
    var winBlueBall int 
    for i := 0; i < 6; i++ {
        winRedBalls[i], _ = strconv.Atoi(result[i*2 : i*2+2]) 
    }
    winBlueBall, _ = strconv.Atoi(result[12:14]) 
    
    // 匹配红球数
    matchRed := 0 
    for _, ball := range userRedBalls {
        for _, winBall := range winRedBalls {
            if ball == winBall {
                matchRed++ 
                break 
            }
        }
    }
    // 判断蓝球是否匹配
    isMatchBlue := false 
    if userBlueBall == winBlueBall {
        isMatchBlue = true 
    }
    // 写入结果
    output(matchRed, isMatchBlue) 

    // 等待下一期
    time.Sleep(time.Second * 60 * 60 * 3) 
}
}