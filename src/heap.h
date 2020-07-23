#ifndef _HEAP_H_
#define _HEAP_H_


struct heap {
	int (*compare)(const void *v1, const void *v2);
	int capacity;
	int size;
	void **array;
};


struct heap *create_heap(int capacity, int (*compare)(const void *v1, const void *v2));
void destroy_heap(struct heap *heap);
int insert_heap(struct heap *heap, void *item);
void *remove_heap(struct heap *heap);
void *peek_heap(struct heap *heap);
int heap_size(struct heap *heap);

#endif
