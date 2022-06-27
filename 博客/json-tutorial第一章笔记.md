![](https://img-blog.csdnimg.cn/img_convert/54e60afdf2764a07539da3136f3ce3e4.png)
本作品采用[知识共享署名-非商业性使用-相同方式共享 4.0 国际许可协议](https://creativecommons.org/licenses/by-nc-sa/4.0/)进行许可。

---

# intro
- 本系列博客是关于学习[json-tutorial项目](https://github.com/miloyip/json-tutorial)的一些感悟
- 感觉学习此项目对于使用`C语言`开发一个实际有用的东西很有帮助,而且作者的教导非常详细,代码也很规范,就决定学习一下
- 此项目是一个从零开始的JSON库教程,难度适合于学习过`基本C/C++编程`的同学
- 我的编码环境是`GNU/Linux` Mint OS,使用gcc编译代码

- 这篇博客是关于[json-tutorial](https://github.com/miloyip/json-tutorial)第一章的内容的理解与思考
- 内容主要偏向于对当时觉得难懂的地方的复盘,而不是详细的笔记

# turtorial01.md
## 要实现什么
- 在初次打开教程的时候,真的是满头雾水,即使作者的教程写的十分详细,但还是有种无从下手的感觉,那么关于这个项目,我们到底要实现什么呢?

- 关于要实现的JSON库,其需要实现三种需求
    1. 把JSON文本解析为一个`树状`数据结构--(`parse`)
    2. 提供接口访问该数据结构--(`access`)
    3. 把数据结构转换成JSON文本--(`stringify`)
![](https://raw.githubusercontent.com/Daz-3ux-Img/Img-hosting/master/202206241254385.png)

- 主目标已经确定,显然很复杂,但是第一章的目标其实很简单,只需要实现对`null`与`boolean`的解析即可

## 要怎么实现
- 为了实现对`null与boolean`的解析,我们需要:
    - 在`leptjson.h`中定义后续所需的`结构`与`函数签名`
    - 在`leptjson.c`中实现`解析器`
    - 在`test.c`中实现一个针对该需求的`单元测试`
- 其实需要`coding`的地方就这三处,三个文件中两个实现需求,一个保证需求正常实现,总体框架并不复杂

## 比较难懂的地方
- 在第一章中理解作者所提供的`宏`是一个小小的难点,这个宏很简单,但是如果之前没有接触过比较长宏的话还是比较难懂,我们主要需要去理解它的嵌套
- 需要理解如果程序在`断言(assert)`处崩溃是因为程序员的code存在问题

# SHOW ME THE CODE
- 此处的代码都是实现任务目标后的代码
- 代码才是此项目最核心的部分,让我们来为代码加上详细的注释吧!
- 此项目采用C89,所以注释部分只能使用`/* something */`

## leptjson.h
```c
#ifndef LEPTJSON_H__
#define LEPTJSON_H__

/*关于JSON数据类型的枚举值*/
typedef enum { LEPT_NULL, LEPT_FALSE, LEPT_TRUE, LEPT_NUMBER, LEPT_STRING, LEPT_ARRAY, LEPT_OBJECT } lept_type;

/*JSON的数据结构,第一章中只需要lept_type类型就够了*/
typedef struct {
    lept_type type;
}lept_value;

/*关于返回值的枚举值*/
enum {
    LEPT_PARSE_OK = 0,
    LEPT_PARSE_EXPECT_VALUE, /* 表示只含有空白 */
    LEPT_PARSE_INVALID_VALUE, /* 空白之后还有其他字符 */
    LEPT_PARSE_ROOT_NOT_SINGULAR
};

/*解析JSON的函数*/
int lept_parse(lept_value* v, const char* json);

/*访问返回值结果,获取其类型函数*/
lept_type lept_get_type(const lept_value* v);

#endif /* LEPTJSON_H__ */
```

## leptjson.c
- .c的文件较长,我们分开来看
### 宏与结构体
- `宏`
```c
#define EXPECT(c, ch)       do { assert(*c->json == (ch)); c->json++; } while(0)
```
- 这个宏就很简单,就是为了断言函数传进来的参数是我们想要的,如果在此处崩溃,则说明coding时调用了错误的API

- `结构体`
```c
typedef struct {
    const char* json;
}lept_context;
```
- 将JSON信息放到一个结构体里,主要是为了避免后续解析函数之间传递多个参数

### 解析器
#### `lept_parse`
- 传入实际想要比较的字符(`v`) 和 标准字符(`json`)
```c
int lept_parse(lept_value* v, const char* json) {
    /*
    v是我实际想要比较的字符, json是标准字符
    例如 lept_parse(&v, "null")
    */

    /* 因为要对v所指字符串做改动,所以定义一个临时变量c*/
    lept_context c;
    int ret;
    /* 并不是说v的字符串常量不能是null,而是v所指向的空间不能是空 */
    assert(v != NULL);
    c.json = json;
    /*
        若 lept_parse() 失败，会把 v 设为 null 类型
        所以这里先把它设为 null
        让 lept_parse_value() 写入解析出来的根值
    */
    v->type = LEPT_NULL;
    /*
    JSON-text = ws value ws
    传入c, 比较时用到的是 c->json
    */
    lept_parse_whitespace(&c);
    if((ret = lept_parse_value(&c, v)) == LEPT_PARSE_OK){
        lept_parse_whitespace(&c);
        if(*c.json != '\0'){
            ret = LEPT_PARSE_ROOT_NOT_SINGULAR;
        }
    }
/*
    return lept_parse_value(&c, v);
*/
    return ret;
}
```

#### lept_parse_value
```c
static int lept_parse_value(lept_context* c, lept_value* v) {
    /* 识别value后， 开始进行匹配*/
    switch (*c->json) {
        case 'n':  return lept_parse_null(c, v);
        case 'f':  return lept_parse_false(c, v);
        case 't':  return lept_parse_true(c, v);
        case '\0': return LEPT_PARSE_EXPECT_VALUE;
        default:   return LEPT_PARSE_INVALID_VALUE;
    }
}
```

#### lept_get_type
- 只要v是一个有意义值，就返回v的类型
```c
lept_type lept_get_type(const lept_value* v) {
    assert(v != NULL);
    /* v的type 在 lept_parse()调用中指定 */
    return v->type;
}
```

#### 剩余函数
- 剩余的函数都很容易理解，都是被分别调用执行单个解析任务

## test.c
- 一个简单的测试单元,能够提供测试点测试我们的代码是否可以完成目标需求

### 宏
- 测试单元的宏是第一章的一个小难点,其实只需要理解`\`在C语言中的作用以及两个宏之间的所属关系就可以很好的理解这个宏了,而且可以感受到宏的方便之处~~(宏魔法)~~

```c
static int main_ret = 0;
static int test_count = 0;
static int test_pass = 0;

#define EXPECT_EQ_BASE(equality, expect, actual, format) \
    do {\
        /* 每次被调用都意味着测试点数量加一 */ \
        test_count++;\
        if (equality)\
        /* 通过 */ \
            test_pass++;\
        else {\
        /* 因为传入的是一个枚举值,所以是%d */ \
            fprintf(stderr, "%s:%d: expect: " format " actual: " format "\n", __FILE__, __LINE__, expect, actual);\
            main_ret = 1;\
        }\
    } while(0)

/* 程序调用第二个宏,只需输入两个值,第二个宏会调用第一个宏完成检查 */
#define EXPECT_EQ_INT(expect, actual) EXPECT_EQ_BASE((expect) == (actual), expect, actual, "%d")
```

### test_parse()
```c
/* 依次执行,逐个检查 */
static void test_parse() {
    test_parse_null();
    test_parse_true();
    test_parse_false();
    test_parse_expect_value();
    test_parse_invalid_value();
    test_parse_root_not_singular();
}
```

---
- 第一章的实现到这里就结束了,都是一些很基础的内容,但果然还是写出来才能感觉更通透一点

# 参考
- [从零开始的 JSON 库教程](https://github.com/miloyip/json-tutorial)