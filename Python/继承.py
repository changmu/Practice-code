class A:
	def init(self):
		self.blocked = []
	def filter(self, sequence):
		return [x for x in sequence if x not in self.blocked]

class B(A):
	def init(self):
		self.blocked = ['SPAM']

f = A()
f.init();
print f.filter([1, 2, 3])

s = B()
s.init()
print s.filter(['SPAM', 'a', 'b'])

print issubclass(B, A)
print B.__bases__
print A.__bases__

print f.__class__
print s.__class__

# raw_input()
