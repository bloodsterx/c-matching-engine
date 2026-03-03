#ifndef HEAP_H
#define HEAP_H

#include "order.h"
#include <stdlib.h>

typedef enum {
    MIN, MAX
} HeapType;

typedef struct {
OrderList** data; // array of OrderList* - each is a price level (price prio) with its time-priority queue
    int capacity;
    int size;
    HeapType type;
} Heap;

Heap* init_heap(int capacity, HeapType type);

int heapify_up(Heap* h);

int heapify_down(Heap* h);

/*
    Look at root of heap without popping
*/
OrderList* peek(Heap* h);

int push(Heap* h, OrderList* order_list);

OrderList* pop(Heap* h);

/*
    Destroys each OrderList (container only; OrderNodes remain in caller's hashmap),
    then frees the data array and the heap. Caller is responsible for freeing
    OrderNodes via the global order-id hashmap.

    Returns -1 if h is NULL or h->data is NULL.
*/
int destroy_heap(Heap* h);



#endif