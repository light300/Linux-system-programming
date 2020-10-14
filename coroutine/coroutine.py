import time

def consume():
	while True:
		number = yield
		print('begin consume', number)

c = consume()

next(c)
for num in range(0, 100):
	print("begin produce", num)
	time.sleep(1)
	c.send(num)
