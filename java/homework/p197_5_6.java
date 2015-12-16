/*
 * 5.1 在Java中创建线程有几种方式？
 *  有2种方式：1>通过继承Thread类
 *             2>通过实现Runnable接口来构造Thread类
 * 5.2 sleep()和wait()有什么区别？
 *  sleep()让当前线程处于睡眠状态，指定时间后自动被唤醒
 *  wait()阻塞当前线程，除非接收到其余线程的notify(), 否则不会被唤醒
 * */

/*
 * 6
 * */
public class p197_5_6 {
    public static void main(String args[]) throws Exception {
        // 1
        Student stu1 = new Student("Thread 1");
        Student stu2 = new Student("Thread 2");

        stu1.start();
        stu2.start();

        // 2
        new Thread(new Student2()).start();
        for (int i = 0; i < 50; ++i)
            System.out.println("main: " + i);

        // 3
        new Thread(new Teacher(), "Teacher 1").start();
        new Thread(new Teacher(), "Teacher 2").start();
        new Thread(new Teacher(), "Teacher 3").start();
        new Thread(new Teacher(), "Teacher 4").start();

        // 4
        AddNum addNum = new AddNum();
        for (int i = 1; i <= 10; ++i) {
            new Thread(addNum, "Thread " + i).start();
        }
        // 等待线程完成
        Thread.sleep(500);
    }
}

class Student extends Thread {
    Student(String str) {
        super(str);
    }

    public void run() {
        System.out.println(Thread.currentThread().getName());
    }
}

class Student2 implements Runnable {
    public void run() {
        for (int i = 0; i < 50; ++i)
            System.out.println("new: " + i);
    }
}

class Teacher implements Runnable {
    private int studyNotes = 80;

    public void run() {
        while (studyNotes > 0)
            distribute();
    }

    private synchronized void distribute() {
        if (studyNotes > 0) {
            System.out.println(Thread.currentThread().getName() + " 正在分发第 " + studyNotes-- + " 份笔记");
        }
    }
}


class AddNum implements Runnable {
    private int beg, end;
    private int globalSum;
    private int count;

    private synchronized void add() {
        int localSum = 0;
        end = ++count * 10;
        beg = end - 9;

        for (int i = beg; i <= end; ++i)
            localSum += i;
        System.out.println(Thread.currentThread().getName() + "'s result is " + localSum);
        globalSum += localSum;

        if (end == 100)
            System.out.println("The total Sum is " + globalSum);
    }

    public void run() {
        add();
    }
}
