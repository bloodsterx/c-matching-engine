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

Heap* resize_heap(Heap* h);

int heapify_up(Heap* h);

int heapify_down(Heap* h);

/*
    Look at root of heap without popping
*/
OrderList* peek(Heap* h);

int push(Heap* h, OrderList* order_list);

OrderList* pop(Heap* h);

/*
    Destroy order lists first, then free the data array, then self

    Throws error if:
    - there is at least one active order (at least one OrderList has size > 0)
    - h is NULL
    - h->data is NULL
*/
int destroy_heap(Heap* h);



#endif