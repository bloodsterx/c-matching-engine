#include "../../lib/heap.h"
#include "../../lib/order.h"
#include <assert.h>
#include <stdio.h>
#include <string.h>

static OrderList* make_price_level(int price) {
    OrderList* ol = init_orderlist();
    if (!ol) return NULL;
    ol->price = price;
    return ol;
}

static int clear_orders(OrderList* ol) {
    if (!ol) return -1;
    while (ol->size > 0) {
        if (remove_order(ol, ol->tail->next) == -1) return -1;
    }
    return 0;
}

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
    OrderList* ol = make_price_level(100);
    if (!ol) return -1;
    Order* order = init_order(1, strdup("alice"), 100, 10, BUY, 0);
    if (!order) return -1;
    if (add_order(ol, order) == -1) return -1;
    
    push(min_heap, ol);
    OrderList* peeked = peek(min_heap);
    assert(ol == peeked);
    print_orderlist(peeked);

    remove_order(ol, peeked->tail->next);

    destroy_heap(min_heap);
    return 0;

}

int test3() {
    Heap* min_heap = init_heap(0, MIN);
    if (peek(min_heap) != NULL) {
        destroy_heap(min_heap);
        return -1;
    }
    OrderList* ol = make_price_level(100);
    if (!ol) return -1;
    Order* order1 = init_order(1, strdup("alice"), 100, 10, BUY, 0);
    Order* order2 = init_order(2, strdup("bob"), 100, 20, SELL, 1);
    if (!order1 || !order2) return -1;
    if (add_order(ol, order1) == -1) return -1;
    push(min_heap, ol);

    if (add_order(ol, order2) == -1) return -1;
    OrderList* peeked = peek(min_heap);
    assert(ol == peeked);
    print_orderlist(peeked);

    remove_order(ol, peeked->head->prev);
    remove_order(ol, peeked->tail->next);

    destroy_heap(min_heap);
    return 0;
}

/*
    Min-heap ordering by price, pop all levels, and ensure resizing works.
*/
int test4_min_heap_ordering() {
    Heap* h = init_heap(0, MIN);
    if (!h) return -1;

    int prices[] = { 500, 100, 300, 200, 400 };
    int n = (int)(sizeof(prices) / sizeof(prices[0]));
    for (int i = 0; i < n; i++) {
        OrderList* ol = make_price_level(prices[i]);
        if (!ol) return -1;
        if (push(h, ol) == -1) return -1;
    }

    OrderList* top = peek(h);
    assert(top != NULL);
    assert(top->price == 100);
    assert(h->size == n);
    assert(h->capacity >= h->size);

    int expected[] = { 100, 200, 300, 400, 500 };
    for (int i = 0; i < n; i++) {
        OrderList* popped = pop(h);
        assert(popped != NULL);
        assert(popped->price == expected[i]);
        destroy_orderlist(popped);
    }
    assert(peek(h) == NULL);
    assert(pop(h) == NULL);

    if (destroy_heap(h) == -1) return -1;
    return 0;
}

/*
    Max-heap ordering by price, pop all levels.
*/
int test5_max_heap_ordering() {
    Heap* h = init_heap(0, MAX);
    if (!h) return -1;

    int prices[] = { 500, 100, 300, 200, 400 };
    int n = (int)(sizeof(prices) / sizeof(prices[0]));
    for (int i = 0; i < n; i++) {
        OrderList* ol = make_price_level(prices[i]);
        if (!ol) return -1;
        if (push(h, ol) == -1) return -1;
    }

    OrderList* top = peek(h);
    assert(top != NULL);
    assert(top->price == 500);

    int expected[] = { 500, 400, 300, 200, 100 };
    for (int i = 0; i < n; i++) {
        OrderList* popped = pop(h);
        assert(popped != NULL);
        assert(popped->price == expected[i]);
        destroy_orderlist(popped);
    }

    if (destroy_heap(h) == -1) return -1;
    return 0;
}

/*
    Interleaved push/pop and cleanup of orders in levels.
*/
int test6_interleaved_push_pop() {
    Heap* h = init_heap(1, MIN);
    if (!h) return -1;

    OrderList* l100 = make_price_level(100);
    OrderList* l200 = make_price_level(200);
    OrderList* l150 = make_price_level(150);
    if (!l100 || !l200 || !l150) return -1;

    if (push(h, l200) == -1) return -1;
    if (push(h, l100) == -1) return -1;
    assert(peek(h)->price == 100);

    OrderList* popped = pop(h);
    assert(popped == l100);

    // Add a couple of orders to l200, then ensure we can clean them up.
    Order* o1 = init_order(1, strdup("alice"), 200, 10, BUY, 0);
    Order* o2 = init_order(2, strdup("bob"), 200, 20, SELL, 1);
    if (!o1 || !o2) return -1;
    if (add_order(l200, o1) == -1) return -1;
    if (add_order(l200, o2) == -1) return -1;
    print_orderlist(l200);

    if (push(h, l150) == -1) return -1;
    assert(peek(h)->price == 150);

    // Cleanup popped level (it has no orders) and destroy it (not owned by heap anymore).
    destroy_orderlist(popped);

    // Cleanup orders in l200 (still owned by heap).
    if (clear_orders(l200) == -1) return -1;

    if (destroy_heap(h) == -1) return -1;
    return 0;
}

/*
    API edge cases: NULL/empty handling and push argument validation.
*/
int test7_api_edges() {
    assert(peek(NULL) == NULL);
    assert(pop(NULL) == NULL);
    assert(destroy_heap(NULL) == -1);

    Heap* h = init_heap(0, MIN);
    if (!h) return -1;
    assert(peek(h) == NULL);
    assert(pop(h) == NULL);

    OrderList* ol = make_price_level(123);
    if (!ol) return -1;
    assert(push(NULL, ol) == -1);
    assert(push(h, NULL) == -1);

    // Not pushed, so we own it.
    destroy_orderlist(ol);

    if (destroy_heap(h) == -1) return -1;
    return 0;
}

typedef int (*heap_test_fn)();

static int run_heap_test(int index, const char* name, heap_test_fn fn) {
    printf("  [HEAP TEST %d] %s ...\n", index, name);
    int rc = fn();
    if (rc == 0) {
        printf("  [HEAP TEST %d] %s PASSED\n\n", index, name);
    } else {
        printf("  [HEAP TEST %d] %s FAILED (rc=%d)\n\n", index, name, rc);
    }
    return rc;
}

int main() {
    int failed = 0;
    int idx = 1;

    printf("========== HEAP TESTS BEGIN ==========\n");

    failed |= run_heap_test(idx++, "test1_init_destroy",              test1) != 0;
    failed |= run_heap_test(idx++, "test2_single_push_peek_destroy",  test2) != 0;
    failed |= run_heap_test(idx++, "test3_multi_order_same_price",    test3) != 0;
    failed |= run_heap_test(idx++, "test4_min_heap_ordering",         test4_min_heap_ordering) != 0;
    failed |= run_heap_test(idx++, "test5_max_heap_ordering",         test5_max_heap_ordering) != 0;
    failed |= run_heap_test(idx++, "test6_interleaved_push_pop",      test6_interleaved_push_pop) != 0;
    failed |= run_heap_test(idx++, "test7_api_edges",                 test7_api_edges) != 0;

    printf("=========== HEAP TESTS END ===========\n");
    if (failed) {
        printf("One or more HEAP tests FAILED.\n");
        return -1;
    }
    printf("All HEAP tests PASSED.\n");
    return 0;
}
