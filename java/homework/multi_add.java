public class multi_add extends Thread {
    int startNum;
    static int sum;

    public multi_add(int startNum) {
        this.startNum = startNum;
    }

    synchronized void add(int num) {
        sum += num;
    }

    public void run() {
        int sum = 0;

        for (int i = 0; i < 10; ++i) {
            sum += startNum + i;
        }

        add(sum);
    }

    public static void main(String[] args) throws Exception {
        multi_add[] t = new multi_add[10];
        for (int i = 0; i < 10; ++i) {
            t[i] = new multi_add(i * 10 + 1);
            t[i].start();
        }
        for (int i = 0; i < 10; ++i)
            t[i].join();
        System.out.println("sum is " + sum);
    }
}
