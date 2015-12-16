# coding: utf-8
# 将输入的年月日转换成人性化的表示
# Author：长木
# compiler version：2.7.9
# 时间：2015.1.3

month = ['January', 'February', 'March', 'April', \
		 'May', 'June', 'July', 'August', \
		 'September', 'October', 'November', 'December']
dayEnding = ['st', 'ed', 'rd'] + 17 * ['th'] + \
			['st', 'ed', 'rd'] + 7 * ['th'] + \
			['st']

y = raw_input(unicode('年份: ', 'utf-8').encode('gbk'))
m = raw_input("month: ")
d = raw_input("day: ")

print y + ' ' + month[int(m)-1] + ' ' + d + dayEnding[int(d)-1]

raw_input()