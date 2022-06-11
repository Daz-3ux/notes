#include <iostream>
#include <string>
#include <vector>
#include <memory>
#include <initializer_list>
#include <exception>

using std::string;
using std::vector;

class StrBlob
{
public:
    typedef std::vector<std::string>::size_type size_type;

    StrBlob(): data(std::make_shared<vector<string> >()){};
    StrBlob(std::initializer_list<string> il): data(std::make_shared<vector<string> > (il)) {}

    size_type size() const
    { 
        return data->size(); 
    }

    bool empty() const 
    {
        return data->empty(); 
    }
    // 删除 / 添加 元素
    void push_back(const std::string &t) 
    {
        data->push_back(t);
    }
    void pop_back() const
    {
        check(0, "pop_back on empty StrBlob");
        data->pop_back();
    }

    // 元素访问
    string front()
    {
        check(0, "front on empty StrBlob");
        return data->front();
    }

    string back()
    {
        check(0, "back on empty StrBlob");
        return data->back();
    }

    const string &front() const
    {
        check(0, "front on empty StrBlob");
        return data->front();
    }

    const string &back() const
    {
        check(0, "back on empty StrBlob");
        return data->back();
    }

private:
    void check(size_type i, const string &msg) const
    {//先检查是否越界
        if(i >= data->size()){
            throw std::out_of_range(msg);
        }
    }

private:
    std::shared_ptr<std::vector<std::string>> data;//size / empty / push_back通过指向底层vector的data完成工作
};