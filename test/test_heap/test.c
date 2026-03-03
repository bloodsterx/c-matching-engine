#include "../../lib/heap.h"
#include "../../lib/order.h"
#include <assert.h>
#include <string.h>

/*
    Initialise and destroy
*/
int test1() {
    Heap* min_heap = init_heap(0, MIN);
    if (destroy_heap(min_heap) == -1) return -1;
    return 0;
}

/*
    Initialise, push one node, peek, and destroy
*/
int test2() {
    Heap* min_heap = init_heap(0, MIN);
    if (peek(min_heap) != NULL) {
        destroy_heap(min_heap);
        return -1;
    }
    OrderList* ol = init_orderlist();
    Order* order = init_order(1, strdup("alice"), 100, 10, BUY, 0);
    if (add_order(ol, order) == -1) return -1;
    
    push(min_heap, ol);
    OrderList* peeked = peek(min_heap);
    assert(ol == peeked);
    print_orderlist(peeked);

    remove_order(ol, peeked->tail->next);

    destroy_heap(min_heap);
    return 0;

}

int main() {
    if (test1() == -1) return -1;
    if (test2() == -1) return -1;

    return 0;
    

}
