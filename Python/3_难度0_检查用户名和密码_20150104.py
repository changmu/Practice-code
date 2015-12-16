# coding: utf-8
# 检查用户名和密码
# Author：长木
# compiler version：2.7.9
# 时间：2015.1.4

database = [
	['jack', 123],
	['tom', 234],
	['harry', 159]
]
id = raw_input('Input id: ')
passward = int(raw_input('Input passward: '))

if [id, passward] in database:
	print 'Yes'
else:
	print 'No'

raw_input()