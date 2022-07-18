/*
某公司有 经理、销售员、小时工 等多类人员。
经理按周计算薪金；
销售员每月底薪800元，然后加销售提成，每销售1件产品提取销售利润的5%；
小时工按小时计算薪金。
每类人员都有姓名和身份证号等数据。
为简化问题，把各类人员的共有信息抽象成基类Employee，其他人员则继承该类的功能。 
*/
#include <iostream>
#include <string>

using namespace std;

//实现一个人员管理的基类
class Employee{
public:
    Employee(string Name, string Id) : name(Name), id(Id) {};
    string getName() const { return name; }         //返回姓名
    string getId() const { return id; }             //返回身份证号
    virtual float getSalary() const { return 0.0;}  //返回薪水
    virtual void print() const{                     //输出姓名与身份证号
        cout << "姓名: " << name << "\t\t 编号: " << id << endl; 
    }
private:
    string name;
    string id;
};

// 继承自基类的 经理类
// 经理按周计算薪资
class Manager : public Employee{
public:
    Manager(string name, string id, int week):Employee(name, id){
        weeklySalary = week*1000;
    }
    float getSalary() const {return weeklySalary;}  //获取经理周薪
    void print() const {
        cout << "经理: " << getName() << "\t\t 编号: " << getId()
        << "\t\t 总工资: " << getSalary() << endl;
    }

private:
    float weeklySalary;                               //周薪
};

// 继承自基态的 销售员类
// 底薪加提成为薪资
class SaleWorker : public Employee{
public:
    SaleWorker(string name, string id, int profit, int x):Employee(name, id){
        workerSalary = baseSalary + x*0.05*profit;
    }
    float getSalary() const {
        return workerSalary;
    }
    void print() const {
        cout << "销售员: " << getName() << "\t\t 编号: " << getId()
        << "\t\t 总工资: " << getSalary() << endl;
    }

private:
    float baseSalary = 800.0;
    float workerSalary;
};

// 继承自基类的 小时工类
// 按小时计算薪资
class HourWorker : public Employee{
public:
    HourWorker(string name, string id, int hour):Employee(name, id){
        totalMoney = hour*hourMoney;
    }
    float getSalary() const {
        return totalMoney;
    }
    void print() const {
        cout << "小时工: " << getName() << "\t\t 编号: " << getId()
        << "\t\t 总工资: " << getSalary() << endl;
    }

private:
    float hourMoney = 100.0;
    float totalMoney;
};

int main(int argc, char ** argv){
    cout << "请输入您要压榨员工多少周: " << endl;
    int week;
    cin >> week;
    Manager m ("小王", "123", week);
    m.print();

    cout << "请输入销售利润: " << endl;
    int profit;
    cin >> profit;
    cout << "请输入销售件数: " << endl;
    int x;
    cin >> x;
    SaleWorker s("小李", "321", profit, x);
    s.print();

    cout << "请输入压榨时长: ";
    int hour;
    cin >> hour;
    HourWorker h("小达", "213", hour);
    h.print();

    return 0;
}