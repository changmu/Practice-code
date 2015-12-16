class Person {
    public void finalize() {
        System.out.println("对象正在作为垃圾被回收");
    }
}

public class Main {
    public static void main(String[] args) {
        Person p1 = new Person();
        Person p2 = new Person();

        p1 = p2 = null;
        System.gc();
    }
}
