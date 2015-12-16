# coding: utf-8
# 简单数据库
# 使用人名作为键的字典。每个人用另一个字典表示，其键'phone'和'addr'分别表示他们的电话号码和地址。
# author: changmu

people = {
	'Alice': {
		'phone': '2341',
		'addr': 'Foo drive 23'
	},

	'Beth': {
		'phone': '9102',
		'addr': 'Bar street 42'
	},

	'Cecil': {
		'phone': '3158',
		'addr': 'Baz avenue 90'
	}
}

# 针对电话号码和地址使用的描述性标签，会在打印输出的时候用到
labels = {
	'phone': 'phone number',
	'addr': 'address'
}

name = raw_input('Name: ')

# 查找电话号码还是地址？使用正确的键：
request = raw_input('Phone number(p) or address(a)? ')
if request == 'p': key = 'phone'
else: key = 'addr'

if name in people:
	print '%s\'s %s is %s.' % (name, labels[key], people[name][key])

raw_input()
