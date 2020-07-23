#include "heap.h"
#include <stdio.h>
#include <stdlib.h>

int compar(const void *a, const void *b) {
	int ia = *(int *)a;
	int ib = *(int *)b;

	return ia > ib ? 1 : ia == ib ? 0 : -1;
}

int main() {
	struct heap *heap = create_heap(32, compar);

	int *a;	

	a = malloc(sizeof(int));
	*a = 4;
	insert_heap(heap, a);

	int i = 0;
	for (i = 0; i < heap->size; i++) {
		a = heap->array[i];
		printf("%d ", *a);
	}
	printf("\n");
	a = malloc(sizeof(int));
	*a = 1;
	insert_heap(heap, a);

	for (i = 0; i < heap->size; i++) {
		a = heap->array[i];
		printf("%d ", *a);
	}
	printf("\n");
	a = malloc(sizeof(int));
	*a = 7;
	insert_heap(heap, a);

	for (i = 0; i < heap->size; i++) {
		a = heap->array[i];
		printf("%d ", *a);
	}
	printf("\n");
	a = malloc(sizeof(int));
	*a = 2;
	insert_heap(heap, a);

	for (i = 0; i < heap->size; i++) {
		a = heap->array[i];
		printf("%d ", *a);
	}
	printf("\n");
	a = malloc(sizeof(int));
	*a = 8;
	insert_heap(heap, a);

	for (i = 0; i < heap->size; i++) {
		a = heap->array[i];
		printf("%d ", *a);
	}
	printf("\n");
	a = malloc(sizeof(int));
	*a = 9;
	insert_heap(heap, a);

	for (i = 0; i < heap->size; i++) {
		a = heap->array[i];
		printf("%d ", *a);
	}
	printf("\n");

	a = remove_heap(heap);	
	printf("%d\n", *a);	
	free(a);
	a = remove_heap(heap);	
	printf("%d\n", *a);	
	free(a);
	a = remove_heap(heap);	
	printf("%d\n", *a);	
	free(a);
	a = remove_heap(heap);	
	printf("%d\n", *a);	
	free(a);
	a = remove_heap(heap);	
	printf("%d\n", *a);	
	free(a);
	a = remove_heap(heap);	
	printf("%d\n", *a);	
	free(a);

	return 0;
}
