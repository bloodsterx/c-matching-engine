#include "../lib/heap.h"

#include <stdio.h>
#include <stdlib.h>

Heap* init_heap(int capacity, HeapType type) {
    Heap* h = (Heap*) malloc(sizeof(Heap));
    if (!h) return NULL;
    h->capacity = capacity;
    h->size = 0;
    h->type = type;
    OrderList** lists = (OrderList**) malloc(sizeof(OrderList*) * capacity);
    if (!lists) {
        free(h);
        return NULL;
    }
    h->data = lists;
    return h;
}

static int has_priority(Heap* h, int i, int j) {
    if (h->type == MAX) {
        return h->data[i]->price > h->data[j]->price;
    } else {
        return h->data[i]->price < h->data[j]->price;
    }
}

/*
    CRITICAL: caller must ensure i, j are in range of ol[0:ol->size]
*/
static int swap(OrderList** i, OrderList** j) {
    OrderList* tmp = *i;
    *i = *j;
    *j = tmp;
    return 0;
}

int heapify_up(Heap* h) {
    if (!h) return -1;
    int i = h->size - 1;

    while (i > 0) {
        int parent = (i - 1) / 2;
        
        if (has_priority(h, i, parent)) {
            // if child smaller than parent in min heap (bigger than parent in max heap)
            swap(&h->data[parent], &h->data[i]);
            i = parent;
        } else {
            break;
        }
    }
    return 0;
}

int heapify_down(Heap* h) {
    if (!h) return -1;
    int i = 0; // root of any given subtree
    
    while (i < h->size) { // at each iteration (subtree), compare root 'i' with its children
        int best = i; // best is the min of a min heap, max of a max heap
        int l = 2 * i + 1;
        int r = 2 * i + 2;
        
        if (l < h->size && has_priority(h, l, best)) {
            best = l;
        }
        if (r < h->size && has_priority(h, r, best)) {
            best = r;
        }
        if (i == best) {
            // the root (after deletion) has priority over both its children, thus heap is sorted
            break;
        }
        swap(&h->data[i], &h->data[best]);
        i = best;
    }
    return 0;
}

/*
    Resizes heap when size == capacity. Caller must ensure size == capacity before resize_heap call
*/
static int resize_heap(Heap* h) {
    if (!h) return -1;

    OrderList** tmp = (OrderList**) realloc(h->data, sizeof(OrderList*)*h->capacity*2);
    if (!tmp) return -1;
    
    h->capacity *= 2; // double in size - standard resize op
    h->data = tmp;
    return 0;
}


int push(Heap *h, OrderList *order_list) {
    if (!h || !order_list) return -1;
    
    if (h->size == h->capacity) {
        if (resize_heap(h) == -1) return -1;
    }

    h->data[h->size] = order_list;
    h->size++;

    if (heapify_up(h) == -1) return -1;
    
    return 0;
}
