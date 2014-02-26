CFLAGS=-g
test_bt: test_bt.o btrace.o
	$(CC) test_bt.o btrace.o -o $@
