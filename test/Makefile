all: pre test_thread_pool test_blocking_queue bq_performance_test

pre:
	make -C ../src

test_thread_pool: test_thread_pool.o ../src/thread_pool.o ../src/blocking_queue.o
	gcc -g -o test_thread_pool test_thread_pool.o ../src/thread_pool.o ../src/blocking_queue.o -lpthread

test_thread_pool.o: test_thread_pool.c
	gcc -g -c -I../src test_thread_pool.c


test_blocking_queue: test_blocking_queue.o ../src/blocking_queue.o
	make -C ../src blocking_queue.o
	gcc -g -o test_blocking_queue test_blocking_queue.o ../src/blocking_queue.o

test_blocking_queue.o: test_blocking_queue.c
	gcc -g -c -I../src test_blocking_queue.c

bq_performance_test: bq_performance_test.o ../src/blocking_queue.o
	make -C ../src blocking_queue.o
	gcc -g -o bq_performance_test bq_performance_test.o ../src/blocking_queue.o -lpthread

bq_performance_test.o: bq_performance_test.c
	gcc -g -c -I../src bq_performance_test.c


clean:
	make -C ../src clean
	rm -f *.o

.PHONY:
	clean
