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

int push(Heap *h, OrderList *order_list) {
    return 0;
}
