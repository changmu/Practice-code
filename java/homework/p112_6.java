public class p112_6 {
    public static void main(String[] args) {
        Student stu = new Student();
        stu.setVal("ZhangSan", 99.5);
        stu.getVal();

        Student stu2 = new Student("LiMing", 100);
        stu2.getVal();

        Father.Child child = new Father(). new Child();
        child.introFather();
    }
}
/*
 * 1
 * */
class Student {
    private String name;
    private double score;

    public Student() {}
    public Student(String _name, double _score) {
        name = _name;
        score = _score;
    }

    public void getVal() {
        System.out.println(name + " " + score);
    }
    public void setVal(String _name, double _score) {
        name = _name;
        score = _score;
    }
}
/*
 * 2
 * */
class Father {
    private String name = "zhangjun";

    class Child {
        public void introFather() {
            System.out.println(name);
        }
    }
}
