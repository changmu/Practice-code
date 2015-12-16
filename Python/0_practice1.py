def init(data):
	data['first'] = {}
	data['middle'] = {}
	data['last'] = {}

storage = {}
init(storage)
print storage

def lookup(data, label, name):
	return data[label].get(name)

print lookup(storage, 'middle', 'Lie')

def store(data, full_name):
	names = full_name.split();
	if len(names) == 2: names.insert(1, '')
	labels = 'first', 'middle', 'last'
	print '...', labels
	for label, name in zip(labels, names):
		people = lookup(data, label, name)
		if people:
			people.append(full_name)
		else:
			data[label][name] = [full_name]
			
MyNames = {}
init(MyNames)
store(MyNames, 'Magnus Lie Hetland')
print lookup(MyNames, 'middle', 'Lie')

store(MyNames, 'Robin Hood')
store(MyNames, 'Robin Locksley')
print lookup(MyNames, 'first', 'Robin')
store(MyNames, 'Mr. Gumby')
print lookup(MyNames, 'middle', '')
