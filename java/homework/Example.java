/*
 * 这段代码是课本第五章的样例代码，已调试通过，注释内的代码对应不同的小节
 * */
public class Example {
    public static void main(String[] args) throws Exception {
        /*
           Storage st = new Storage();
           Input input = new Input(st);
           Output output = new Output(st);

           new Thread(input).start();
           new Thread(output).start();
           */

        /*
           MyThread myThread = new MyThread();
           myThread.start();

           MyThread2 myThread2 = new MyThread2();
           Thread thread = new Thread(myThread2);
           thread.start();

           new TicketWindow().start();
           new TicketWindow().start();
           new TicketWindow().start();
           new TicketWindow().start();

           TicketWindow2 ticketWindow2 = new TicketWindow2();
           Thread th = new Thread(ticketWindow2 , "窗口1");
           th.setDaemon(true);
           th.start();

           Thread th2 = new Thread(ticketWindow2 , "窗口2");
           th2.setDaemon(true);
           th2.start();

           new Thread(ticketWindow2 , "窗口1").start();
           new Thread(ticketWindow2, "窗口2").start();
           new Thread(ticketWindow2, "窗口3").start();
           new Thread(ticketWindow2, "窗口4").start();

           Thread th = new Thread(new MinPriority(), "低优先级线程");
           Thread th2 = new Thread(new MaxPriority(), "高优先级线程");

           th.setPriority(Thread.MIN_PRIORITY);
           th2.setPriority(10);

           th.start();
           th2.start();

           new Thread(new SleepThread()).start();
           for (int i = 0; i < 10; ++i) {
           if (i == 5) Thread.sleep(2000);
           System.out.println(Thread.currentThread().getName() + " is outputing " + i);
           Thread.sleep(500);
           }

           Thread tha = new YieldThread("Thread A");
           Thread thb = new YieldThread("Thread B");

           tha.start();
           thb.start();

           Thread t = new Thread(new EmergencyThread(), "Thread child");
           t.start();

           for (int i = 0; i < 6; ++i) {
           System.out.println(Thread.currentThread().getName() + " is outputing " + i);
           if (i == 2) t.join();
           Thread.sleep(500);
           }

           Ticket2 sal = new Ticket2();
        // 以下四句的执行结果始终为Thread 1
        // 跟书上的差别略大，大概是跟操作系统的调度有关
        new Thread(sal, "Thread 1").start();
        new Thread(sal, "Thread 2").start();
        new Thread(sal, "Thread 3").start();
        new Thread(sal, "Thread 4").start();

        // 等待后台线程执行完
        Thread.sleep(1000); // 1s

        DeadLockThread chinese = new DeadLockThread(true);
        DeadLockThread american = new DeadLockThread(false);

        new Thread(chinese, "chinese").start();
        new Thread(american, "american").start();
        */
    }
}

class MyThread extends Thread {
    public void run() {
        while (true) {
            System.out.println("Child");
        }
    }
}

class MyThread2 implements Runnable {
    public void run() {
        while (true)
            System.out.println("Child2");
    }
}

class TicketWindow extends Thread {
    private static int tickets = 1000;
    public void run() {
        while (true) {
            if (tickets > 0) {
                Thread th = Thread.currentThread(); // 绑定当前进程
                String th_name = th.getName();
                System.out.println(th_name + "正在发售" + tickets-- + "张票");
            }
        }
    }
}

class TicketWindow2 implements Runnable {
    private int tickets = 100;

    public void run() {
        while (true) {
            if (tickets > 0) {
                Thread th = Thread.currentThread();
                String th_name = th.getName();
                System.out.println(th_name + "正在发售" + tickets-- + "张票");
            }
        }
    }
}

class MaxPriority implements Runnable {
    public void run() {
        for (int i = 0; i < 10; ++i) {
            System.out.println(Thread.currentThread().getName() + "is outputing " + i);
        }
    }
}

class MinPriority implements Runnable {
    public void run() {
        for (int i = 0; i < 10; ++i) {
            System.out.println(Thread.currentThread().getName() + "is outputing " + i);
        }
    }
}

/*
 * 线程休眠
 * */
class SleepThread implements Runnable {
    public void run() {
        for (int i = 0; i < 10; ++i) {
            if (i == 3) {
                try {
                    Thread.sleep(2000);
                } catch (InterruptedException e) {
                    e.printStackTrace();
                }
            }
            System.out.println(Thread.currentThread().getName() + " is outputing " + i);
            try {
                Thread.sleep(500);
            } catch (InterruptedException e) {
                e.printStackTrace();
            }
        }
    }
}

/*
 * 线程让步
 * */
class YieldThread extends Thread {
    public YieldThread(String name) {
        super(name);
    }

    public void run() {
        for (int i = 0; i != 5; ++i) {
            System.out.println(Thread.currentThread().getName() + " is outputing " + i);
            if (i == 3) {
                System.out.println("线程让步");
                Thread.yield();
            }
        }
    }
}

/*
 * 线程插队
 * */
class EmergencyThread implements Runnable {
    public void run() {
        for (int i = 0; i < 6; ++i) {
            System.out.println(Thread.currentThread().getName() + " is outputing " + i);
            try {
                Thread.sleep(500);
            } catch (InterruptedException e) {
                e.printStackTrace();
            }
        }
    }
}

/*
 * 线程安全
 * */
class SaleThread implements Runnable {
    private int tickets = 10;

    public void run() {
        while (tickets > 0) {
            try {
                Thread.sleep(10);
            } catch (InterruptedException e) {
                e.printStackTrace();
            }
            System.out.println(Thread.currentThread().getName() + " is outputing " + tickets--);
        }
    }
}

class Ticket1 implements Runnable {
    private int tickets = 10;
    Object lock = new Object();

    public void run() {
        while (true) {
            synchronized (lock) {
                try {
                    Thread.sleep(10);
                } catch (InterruptedException e) {
                    e.printStackTrace();
                }
                if (tickets > 0)
                    System.out.println(Thread.currentThread().getName() + " is saleing " + tickets--);
                else break;
            }
        }
    }
}

/*
 * 同步方法
 * */
class Ticket2 implements Runnable {
    private int tickets = 10;

    public void run() {
        while (tickets != 0) {
            saleTicket();
        }
    }

    private synchronized void saleTicket() {
        if (tickets != 0) {
            try {
                Thread.sleep(10);
            } catch (InterruptedException e) {
                e.printStackTrace();
            }

            System.out.println(Thread.currentThread().getName() + " is saleing " + tickets--);
        }
    }
}

/*
 * 死锁问题
 * */
class DeadLockThread implements Runnable {
    static Object chopsticks = new Object();
    static Object knifeAndFork = new Object();
    private boolean flag;

    DeadLockThread(boolean flag) {
        this.flag = flag;
    }

    public void run() {
        if (flag) {
            while (true) {
                synchronized (knifeAndFork) {
                    System.out.println(Thread.currentThread().getName() + " now has knifeAndFork, he is waiting for chopsticks");
                    synchronized (chopsticks) {
                        System.out.println(Thread.currentThread().getName() + " now gets chopsticks");
                        return;
                    }
                }
            }
        } else {
            while (true) {
                synchronized (chopsticks) {
                    System.out.println(Thread.currentThread().getName() + " now has chopsticks, he is waiting for knifeAndFork");
                    synchronized (knifeAndFork) {
                        System.out.println(Thread.currentThread().getName() + " now gets knifeAndFork");
                        return;
                    }
                }
            }
        }
    }
}

/*
 * 多线程通信
 * */
class Storage {
    private int[] cells = new int[10];
    private int inPos, outPos;
    private int count;

    public synchronized void put(int apple) {
        try {
            while (count == cells.length)
                this.wait();
            // 把苹果放入箱子
            cells[inPos] = apple;
            System.out.println("在第 " + inPos + "个位置放" + apple + "个苹果");
            if (++inPos == cells.length)
                inPos = 0;
            ++count;
            this.notify();
        } catch (Exception e) {
            e.printStackTrace();
        }
    }

    public synchronized  void get() {
        try {
            while (count == 0)
                this.wait();
            int data = cells[outPos];
            System.out.println("在第 " + outPos + "个位置取" + data + "个苹果");
            if (++outPos == cells.length)
                outPos = 0;
            --count;
            this.notify();
        } catch (Exception e) {
            e.printStackTrace();
        }
    }
}

class Input implements Runnable {
    private Storage st;
    private int num;

    Input(Storage st) {
        this.st = st;
    }

    public void run() {
        while (true)
            st.put(num++);
    }
}

class Output implements Runnable {
    private Storage st;

    Output(Storage st) {
        this.st = st;
    }

    public void run() {
        while (true)
            st.get();
    }
}
