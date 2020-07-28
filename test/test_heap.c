#include "heap.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdlib.h>
#include <unistd.h>

int random_int(int max) {
	srandom(time(NULL));
	return random() % max;
}

int compar(const void *a, const void *b) {
	int ia = *(int *)a;
	int ib = *(int *)b;

	return ia > ib ? 1 : ia == ib ? 0 : -1;
}

int main() {
	struct heap *heap = create_heap(3, compar);

	int *a;	

	int i;
	int j;
	int ret;
	for (i = 0; i < 23; i++) {
		sleep(1);
		a = malloc(sizeof(int));
		*a = random_int(50);
		ret = insert_heap(heap, a);
		printf("insert ret = %d a = %d\n", ret, *a);
		for (j = 0; j < heap->size; j++) {
			a = heap->array[j];
			printf("%d ", *a);
		}
		printf("\n");
	}
	printf("-------");
	int size = heap_size(heap);
	for (i = 0; i < size; i++) {
		a = remove_heap(heap);	
		printf("%d \n", *a);	
		free(a);
	}
	destroy_heap(heap);
	return 0;
}
