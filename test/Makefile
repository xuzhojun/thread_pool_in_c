all: pre test_thread_pool

pre:
	make -C ../src

test_thread_pool: test_thread_pool.o ../src/thread_pool.o ../src/blocking_queue.o
	gcc -g -o test_thread_pool test_thread_pool.o ../src/thread_pool.o ../src/blocking_queue.o -lpthread

test_thread_pool.o: test_thread_pool.c
	gcc -g -c -I../src test_thread_pool.c


clean:
	make -C ../src clean
	rm -f *.o

.PHONY:
	clean
