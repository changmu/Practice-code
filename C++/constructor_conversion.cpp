// use constructor to do the type conversion
#include <iostream>
#include <string>

using namespace std;

class Student {
private:
    string name;
    int age;
public:
    Student(string str) : name(str), age(22) {}
    Student() : Student("changmu") {}
    void add(Student stu) {
        name += stu.name;
        cout << name << endl;
    }
};

int main()
{
    Student changmu;
    changmu.add(string(", Heurei"));
    // error as `changmu.add("hello")`, once can only convert one type to
    // another
    return 0;
}
