all: thread_pool.o blocking_queue.o

thread_pool.o : thread_pool.c thread_pool.h
	gcc -g -c thread_pool.c

blocking_queue.o : blocking_queue.c blocking_queue.h
	gcc -g -c blocking_queue.c

clean:
	rm -f *.o

.PHONY:
	clean
