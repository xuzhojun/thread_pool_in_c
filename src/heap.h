#ifndef _HEAP_H_
#define _HEAP_H_


struct heap {
	int (*compare)(const void *v1, const void *v2);
	int capacity;
	int size;
	void **array;
};

/**
 * 创建一个堆，默认小顶堆
 * 
 * @param init_capacity 堆的初始容量大小，最小容量7，如果参数小于7，则容量设置为7
 * @param compare 比较器，比较v1和v2的大小。大于0表示v1大于v2；等于0，两变量相等；小于0，v1小于v2
 * @return 成功则返回堆，否则返回NULL
 */
struct heap *create_heap(int init_capacity, int (*compare)(const void *v1, const void *v2));

/**
 * 销毁堆，销毁之前调用者确保堆里元素已经释放
 * 
 * @param heap 堆
 */
void destroy_heap(struct heap *heap);

/**
 * 向堆里添加一个元素
 * 
 * @param heap 堆
 * @param item 需要插入的元素
 * @return 0 插入成功，否则插入失败
 */ 
int insert_heap(struct heap *heap, void *item);

/**
 * 移除并返回堆顶元素
 * 
 * @param heap 堆
 * @return 堆不为空返回堆顶元素，否则返回NULL
 */
void *remove_heap(struct heap *heap);

/**
 * 返回堆顶元素，不移除
 * 
 * @param heap 堆
 * @return 堆不为空返回堆顶元素，否则返回NULL
 */
void *peek_heap(struct heap *heap);

/**
 * 堆大小
 * 
 * @param heap 堆
 * @return 堆大小
 */
int heap_size(struct heap *heap);

#endif
