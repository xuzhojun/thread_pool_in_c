#include "heap.h"
#include <stdlib.h>
#include <stdio.h>

#define PARENT(i) ((i - 1) / 2)
#define LEFT(i) (2 * i + 1)
#define RIGHT(i) (2 * i + 2)

#define DEFAULT_CAPACITY 16

struct heap *create_heap(int capacity,
                         int (*compare)(const void *v1, const void *v2)) {
	struct heap *heap = calloc(1, sizeof(struct heap));
	if (heap == NULL) {
		return NULL;
	}
	heap->capacity = capacity < DEFAULT_CAPACITY ? DEFAULT_CAPACITY : capacity;
	heap->size = 0;
	heap->array = calloc(capacity, sizeof(void *));
	if (heap->array == NULL) {
		free(heap);
		return NULL;
	}
	heap->compare = compare;

	return heap;
}

static void exchange_item(struct heap *heap, int index1, int index2) {
	void *tmp = heap->array[index1];
	heap->array[index1] = heap->array[index2];
	heap->array[index2]	= tmp;
}

static void shift_up(struct heap *heap, int curr) {
	while (curr) {
		if (heap->compare(heap->array[curr], heap->array[PARENT(curr)]) > 0) {
			exchange_item(heap, curr, PARENT(curr));
			curr = PARENT(curr);
			continue;
		}
		break;
	}
}

static void shift_down(struct heap *heap, int curr) {
	while (curr < heap->size - 1) {
		if (heap->array[LEFT(curr)] != NULL 
            && heap->compare(heap->array[curr], heap->array[LEFT(curr)]) < 0) {
			if (heap->array[RIGHT(curr)] != NULL 
                && heap->compare(heap->array[curr], heap->array[RIGHT(curr)]) < 0) {
				if (heap->compare(heap->array[LEFT(curr)], heap->array[RIGHT(curr)]) < 0) {
					exchange_item(heap, curr, RIGHT(curr));
					curr = RIGHT(curr);
					continue;
				}
			}
			exchange_item(heap, curr, LEFT(curr));
			curr = LEFT(curr);
		} else if (heap->array[RIGHT(curr)] != NULL 
                   && heap->compare(heap->array[curr], heap->array[RIGHT(curr)]) < 0) {
			exchange_item(heap, curr, RIGHT(curr));
			curr = RIGHT(curr);
		} else { 
			break;
		}
	}
}

static int resize_heap(struct heap *heap) {
	void *tmp = realloc(heap->array, heap->capacity >> 1);
	if (tmp == NULL) {
		return -1;
	}
	heap->array = tmp;
	heap->capacity >>= 1;
	return 0;
}

int insert_heap(struct heap *heap, void *item) {
	if (heap->size + 1 > heap->capacity) {
		if (resize_heap(heap) != 0) {
			return -1;
		}
	}
	heap->array[heap->size] = item;
	heap->size++;
	shift_up(heap, heap->size - 1);
	return 0;
}

void *remove_heap(struct heap *heap) {
	void *tmp = heap->array[0];
	heap->array[0] = heap->array[heap->size - 1];
	heap->array[heap->size - 1] = NULL;
	heap->size--;
	shift_down(heap, 0);
	return tmp;
}

void *peek_heap(struct heap *heap) {
	return heap->array[0];
}
int heap_size(struct heap *heap) {
	return heap->size;
}

void destroy_heap(struct heap *heap) {
	free(heap->array);
	free(heap);
}
