#include <vector>
#include <map>
#include <string>
#include <string.h>
#include <iostream>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

void func1()
{// key-value
  json js;
  /*
  {} 是字典 
  [] 是列表
  */
  js["name"] = "daz";
  js["age"] = "19";

  /*
  dump: 将C++对象转换为json
  load: 将json转换为C++对象
  */
  std::string s = js.dump();

  std::cout << "func1: " << s.c_str() << std::endl;
}

void func2()
{// 序列化vector
  json js;
  std::vector<int> vc = {1,2,3,4,5,6};

  js["vc"] = vc;

  std::string s = js.dump();

  std::cout << "func2: " << s.c_str() << std::endl;
}

void func3()
{// 序列化map
  json js;
  std::map<std::string, std::string> mss;

  mss.insert({"name", "daz"});
  mss.insert({"age", "19"});

  js["path"] = mss;
  std::string s = js.dump();

  std::cout << "func3: " << s.c_str() << std::endl;
}

void func4()
{// 反序列化
  json js;

  std::map<std::string, std::string> mss;

  mss.insert({"name", "daz"});
  mss.insert({"age", "19"});

  std::vector<int> vc = {1,2,3,4,5,6};

  js["name"] = "daz";
  js["age"] = "19";
  js["path"] = mss;
  js["vc"] = vc;

  std::string s = js.dump();

  // 解析
  json js2 = json::parse(s);
  std::cout << js2 << std::endl;

  // 反序列化key-value
  std::string name = js2["name"];
  std::cout << "name:" << name << std::endl;

  // 反序列化vector容器
  std::vector<int> v = js2["vc"];
  for(int val : v) {
    std::cout << val << std::endl;
  }

  // 反序列化map容器
  std::map<std::string, std::string> m2;
  m2 = js2["path"];
  for(auto p : m2) {
    std::cout << p.first << " " << p.second << std::endl;
  }
}

int main()
{
  // 序列化
  func1();
  func2();
  func3();

  // 反序列化
  func4();

  return 0;
}










