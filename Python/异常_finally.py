x = None
try:
	x = 1 / 1
finally:
	print 'Clean up...'
	raw_input()
	del x

raw_input()
