try:
	x = input('Enter the first number: ')
	y = input("Enter the second number: ")
	print x / y
except (ZeroDivisionError, TypeError), e:
	# print "Error happened."
	print e 		# print exception

raw_input()
