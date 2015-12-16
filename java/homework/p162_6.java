public class p162_6 {
    public static void main(String args[]) {
        // 1
        Student stu = new Student("LiMing", 22);
        Undergraduate stu2 = new Undergraduate("XiaoMing", 20, "Bachelor");

        stu.show();
        stu2.show();

        // 2
        Square squ = new Square();
        Circle cir = new Circle();

        System.out.println(squ.area(2) + " " + cir.area(3));

        // 3
        Player pla = new Player();
        try {
            pla.play(11);
        } catch (NoThisSoundException e) {
            System.out.println(e.getMessage());
        }
    }
}

/*
 * 1
 * */
class Student {
    String name;
    int age;

    public Student(String _name, int _age) {
        name = _name;
        age = _age;
    }

    public void show() {
        System.out.println(name + " " + age);
    }
}

class Undergraduate extends Student {
    String degree;

    public Undergraduate(String _name, int _age, String _degree) {
        super(_name, _age);
        degree = _degree;
    }

    public void show() {
        super.show();
        System.out.println(degree);
    }
}

/*
 * 2
 * */
interface Shape {
    double area(double t);
}

class Square implements Shape {
    public double area(double len) {
        return len * len;
    }
}

class Circle implements Shape {
    public double area(double r) {
        return 3.1415926 * r * r;
    }
}

/*
 * 3
 * */
class NoThisSoundException extends Exception {
    public NoThisSoundException() { super(); }
    public NoThisSoundException(String str) { super(str); }
}

class Player {
    public static void play(int index) throws NoThisSoundException {
        if (index > 10)
            throw new NoThisSoundException("您播放的歌曲不存在");
    }
}
