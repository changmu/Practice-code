/************************************************************* 
 * ����friend member function��ҪС�Ĺ�����������࣬
 * ��һ��������˵������Teacher class��Ҫ������Student classǰ��
 * ���Ҳ��ܶ�����ȫ����Student������friend����ֻ�����������ʵ����Ҫ
 * �ŵ�Student��ʵ�ֺ��涨��.
 *************************************************************/ 
#include <iostream>
#include <vector>
#include <string>

using namespace std;

class Student;  // declare for the vector<Student>;
class Teacher {
private:
    string name;
    int age;
    vector<Student> vec;
public:
    Teacher(string str, int _age) : name(str), age(_age) {}
    Teacher() : Teacher("changmu", 22) {
        cout << "Delegrate constructor is running!" << endl;
    }
    void addStudent(Student &stu) {
        vec.push_back(stu);
        cout << "Add student completed!" << endl;
    }
    void educateStudent(Student &stu);
};


class Student {
friend void Teacher::educateStudent(Student &stu);
private:
    string name;
    int age;
    mutable int happiness;
public:
    Student() = default;
    Student(string str) : name(str), age(0), happiness(0) {}
    void game() const {
        cout << "I'm playing!" << endl;
    }
    void learn() const {
        cout << "I'm learning!" << endl
             << "My happiness is " << happiness << endl;
    }
    void grow() const {
        // ++age;
        ++happiness;
    }
};

void Teacher::educateStudent(Student &stu) {
    ++stu.happiness;
    cout << "Student " << stu.name << " has been educated." << endl;
}

int main()
{
    Student heurei("heurei");
    heurei.learn();
    heurei.grow();
    heurei.learn();

    Teacher changmu;
    // changmu.addStudent(heurei);
    changmu.educateStudent(heurei);
    heurei.learn();
    return 0;
}
