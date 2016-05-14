#include <iostream>
#include <string>

using namespace std;

class Student {
private:
    string name;
    int age;
    static Student ss;
    static const int num = 2;
    static int test;
    static constexpr int test2 = 2 + 3;
public:
    Student(): name("changmu"), age(22) {}
    explicit Student(int a): Student() {}
    void print(int a = test2) {
        cout << num << endl;
        cout << test2 << endl;
        cout << a << endl;
    }
};
int Student::test = 3;
// const double Student::num = 2;

int main()
{
    Student stu;
    stu.print();
    return 0;
}
