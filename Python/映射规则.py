# coding: utf-8
def checkIndex(key):
	'''
		��Ӧ����һ���Ǹ�����
	'''
	if not isinstance(key, (int, long)): raise TypeError
	if key < 0: raise IndexError

class ArithmeticSequence:
	def __init__(self, start = 0, step = 1):
		'''
			��ʼ����������
		'''
		self.start = start
		self.step = step
		self.changed = {}
	def __getitem__(self, key):		# �±�����
		'''
			Get an item from the arithmetic sequence.
		'''
		checkIndex(key)

		try: return self.changed[key]
		except KeyError:
			return self.start + key * self.step
	def __setitem__(self, key, value):		# �±��޸�
		'''
			�޸����������е�һ����
		'''
		checkIndex(key)
		self.changed[key] = value;
	def __len__(self):
		return 1024

# Test
s = ArithmeticSequence(1, 2)
print s[4]
s[4] = 2
print s[4]
print s[50000000000]
print len(s)
