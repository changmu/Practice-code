// 编写程序，实现字符串的大小写转换并倒序输出
public class p235_6 {
    public static void main(String[] args) {
        String str = "HelloWorld";
        StringBuffer strb = new StringBuffer();

        for (int i = str.length() - 1; i >= 0; --i) {
            if (Character.isUpperCase(str.charAt(i)))
                strb.append(Character.toLowerCase(str.charAt(i)));
            else 
                strb.append(Character.toUpperCase(str.charAt(i)));
        }

        System.out.println(strb);
    }
}
