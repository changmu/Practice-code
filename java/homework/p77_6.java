public class p77_6 {
    public static void main(String[] args) {
        /*
         * 1
         * */
        int sum = 0;

        for (int i = 1; i <= 99; ++i) {
            if ((i & 1) == 1)
                sum += i;
        }

        System.out.println(sum);

        System.out.println(f(-2));

        int arr[] = {25, 24, 12, 76, 101, 96, 28};
        f2(arr);
        System.out.println(arr);

        for (int i = 0; i < arr.length; ++i)
            System.out.print(arr[i] + " ");
        System.out.println();
    }
    /*
     * 2
     * */
    private static int f(int x) {
        int y = 0;

        if (x > 0) y = x + 3;
        else if (x == 0) y = 0;
        else y = x * x - 1;

        return y;
    }
    /*
     * 3
     * */
    private static void f2(int arr[]) {
        // i 存放当前最大值
        for (int i = arr.length - 1; i > 0; --i)
            for (int j = 0; j < i; ++j) {
                if (arr[j] > arr[j+1]) {
                    arr[j] ^= arr[j+1];
                    arr[j+1] = arr[j] ^ arr[j+1];
                    arr[j] ^= arr[j+1];
                }
            }
    }
}
