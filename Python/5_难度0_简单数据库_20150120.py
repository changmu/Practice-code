# coding: utf-8
# �����ݿ�
# ʹ��������Ϊ�����ֵ䡣ÿ��������һ���ֵ��ʾ�����'phone'��'addr'�ֱ��ʾ���ǵĵ绰����͵�ַ��
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

# ��Ե绰����͵�ַʹ�õ������Ա�ǩ�����ڴ�ӡ�����ʱ���õ�
labels = {
	'phone': 'phone number',
	'addr': 'address'
}

name = raw_input('Name: ')

# ���ҵ绰���뻹�ǵ�ַ��ʹ����ȷ�ļ���
request = raw_input('Phone number(p) or address(a)? ')
if request == 'p': key = 'phone'
else: key = 'addr'

if name in people:
	print '%s\'s %s is %s.' % (name, labels[key], people[name][key])

raw_input()
