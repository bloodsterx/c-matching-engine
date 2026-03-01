#include "../../lib/order.h"
#include <stdio.h>
#include <string.h>

/* Walk the list forward from tail->next and check each node's order matches
   the expected array. Returns 0 on success. */
static int check_forward(OrderList *ol, Order **expected, int n) {
    OrderNode *cur = ol->tail->next;
    for (int i = 0; i < n; i++) {
        if (cur == ol->head) {
            printf("  forward walk ended early at index %d\n", i);
            return -1;
        }
        if (cur->order != expected[i]) {
            printf("  forward[%d]: got %p, want %p\n", i,
                   (void *)cur->order, (void *)expected[i]);
            return -1;
        }
        cur = cur->next;
    }
    if (cur != ol->head) {
        printf("  forward walk did not reach sentinel head after %d nodes\n", n);
        return -1;
    }
    return 0;
}

/* Walk backward from head->prev and check each node's order matches the
   expected array (which should be in reverse insertion order). */
static int check_backward(OrderList *ol, Order **expected, int n) {
    OrderNode *cur = ol->head->prev;
    for (int i = n - 1; i >= 0; i--) {
        if (cur == ol->tail) {
            printf("  backward walk ended early at index %d\n", i);
            return -1;
        }
        if (cur->order != expected[i]) {
            printf("  backward[%d]: got %p, want %p\n", i,
                   (void *)cur->order, (void *)expected[i]);
            return -1;
        }
        cur = cur->prev;
    }
    if (cur != ol->tail) {
        printf("  backward walk did not reach sentinel tail\n");
        return -1;
    }
    return 0;
}

/* Null the order, relink neighbors, free the node, and decrement size.
   Mirrors remove_order's workflow without going through the public API. */
static int remove_node(OrderList *ol, OrderNode *node, Order *order) {
    node->order = NULL;
    destroy_order(order);
    node->prev->next = node->next;
    node->next->prev = node->prev;
    if (destroy_ordernode(node) != 0) return -1;
    ol->size--;
    return 0;
}

/* ------------------------------------------------------------------ */

/* Add 3 orders and check size + forward/backward traversal. */
static int test_add_three() {
    OrderList *ol = init_orderlist();
    if (!ol) return -1;

    Order *o1 = create_order(1, strdup("alice"), 100, 10, BUY,  0);
    Order *o2 = create_order(2, strdup("bob"),   200, 20, SELL, 1);
    Order *o3 = create_order(3, strdup("carol"), 300, 30, BUY,  2);
    if (!o1 || !o2 || !o3) return -1;

    if (add_order(ol, o1) != 0) return -1;
    if (add_order(ol, o2) != 0) return -1;
    if (add_order(ol, o3) != 0) return -1;

    if (ol->size != 3) {
        printf("  size: got %zu, want 3\n", ol->size);
        return -1;
    }

    /* o3 is closest to tail (newest), o1 is closest to head (oldest). */
    Order *expected[] = {o3, o2, o1};
    if (check_forward(ol,  expected, 3) != 0) return -1;
    if (check_backward(ol, expected, 3) != 0) return -1;

    /* Clean up */
    OrderNode *n1 = ol->tail->next;
    OrderNode *n2 = n1->next;
    OrderNode *n3 = n2->next;
    if (remove_node(ol, n1, o3) != 0) return -1;
    if (remove_node(ol, n2, o2) != 0) return -1;
    if (remove_node(ol, n3, o1) != 0) return -1;
    destroy_orderlist(ol);
    return 0;
}

/* Remove from the tail end (newest insertion = o3) and check links repair. */
static int test_remove_front() {
    OrderList *ol = init_orderlist();
    if (!ol) return -1;

    Order *o1 = create_order(1, strdup("alice"), 100, 10, BUY,  0);
    Order *o2 = create_order(2, strdup("bob"),   200, 20, SELL, 1);
    Order *o3 = create_order(3, strdup("carol"), 300, 30, BUY,  2);
    if (!o1 || !o2 || !o3) return -1;

    add_order(ol, o1);
    add_order(ol, o2);
    add_order(ol, o3);

    OrderNode *n1 = ol->tail->next;
    OrderNode *n2 = n1->next;
    OrderNode *n3 = n2->next;

    if (remove_node(ol, n1, o3) != 0) return -1;

    if (ol->size != 2) return -1;
    Order *expected[] = {o2, o1};
    if (check_forward(ol,  expected, 2) != 0) return -1;
    if (check_backward(ol, expected, 2) != 0) return -1;

    if (remove_node(ol, n2, o2) != 0) return -1;
    if (remove_node(ol, n3, o1) != 0) return -1;
    destroy_orderlist(ol);
    return 0;
}

/* Remove from the head end (oldest insertion = o1) and check links repair. */
static int test_remove_back() {
    OrderList *ol = init_orderlist();
    if (!ol) return -1;

    Order *o1 = create_order(1, strdup("alice"), 100, 10, BUY,  0);
    Order *o2 = create_order(2, strdup("bob"),   200, 20, SELL, 1);
    Order *o3 = create_order(3, strdup("carol"), 300, 30, BUY,  2);
    if (!o1 || !o2 || !o3) return -1;

    add_order(ol, o1);
    add_order(ol, o2);
    add_order(ol, o3);

    OrderNode *n1 = ol->tail->next;
    OrderNode *n2 = n1->next;
    OrderNode *n3 = n2->next;

    if (remove_node(ol, n3, o1) != 0) return -1;

    if (ol->size != 2) return -1;
    Order *expected[] = {o3, o2};
    if (check_forward(ol,  expected, 2) != 0) return -1;
    if (check_backward(ol, expected, 2) != 0) return -1;

    if (remove_node(ol, n1, o3) != 0) return -1;
    if (remove_node(ol, n2, o2) != 0) return -1;
    destroy_orderlist(ol);
    return 0;
}

/* Remove from the middle and check links repair. */
static int test_remove_middle() {
    OrderList *ol = init_orderlist();
    if (!ol) return -1;

    Order *o1 = create_order(1, strdup("alice"), 100, 10, BUY,  0);
    Order *o2 = create_order(2, strdup("bob"),   200, 20, SELL, 1);
    Order *o3 = create_order(3, strdup("carol"), 300, 30, BUY,  2);
    if (!o1 || !o2 || !o3) return -1;

    add_order(ol, o1);
    add_order(ol, o2);
    add_order(ol, o3);

    OrderNode *n1 = ol->tail->next;
    OrderNode *n2 = n1->next;
    OrderNode *n3 = n2->next;

    if (remove_node(ol, n2, o2) != 0) return -1;

    if (ol->size != 2) return -1;
    Order *expected[] = {o3, o1};
    if (check_forward(ol,  expected, 2) != 0) return -1;
    if (check_backward(ol, expected, 2) != 0) return -1;

    if (remove_node(ol, n1, o3) != 0) return -1;
    if (remove_node(ol, n3, o1) != 0) return -1;
    destroy_orderlist(ol);
    return 0;
}

/* Remove all nodes one by one and confirm sentinels link directly at the end. */
static int test_remove_all() {
    OrderList *ol = init_orderlist();
    if (!ol) return -1;

    Order *o1 = create_order(1, strdup("alice"), 100, 10, BUY,  0);
    Order *o2 = create_order(2, strdup("bob"),   200, 20, SELL, 1);
    if (!o1 || !o2) return -1;

    add_order(ol, o1);
    add_order(ol, o2);

    OrderNode *n1 = ol->tail->next;
    OrderNode *n2 = n1->next;

    if (remove_node(ol, n1, o1) != 0) return -1;
    if (remove_node(ol, n2, o2) != 0) return -1;

    if (ol->size != 0) return -1;
    if (ol->tail->next != ol->head) {
        printf("  tail->next should be head after emptying list\n");
        return -1;
    }
    if (ol->head->prev != ol->tail) {
        printf("  head->prev should be tail after emptying list\n");
        return -1;
    }

    destroy_orderlist(ol);
    return 0;
}

/* ------------------------------------------------------------------ */

#define RUN(fn) \
    do { \
        if ((fn)() != 0) { printf("FAIL: " #fn "\n"); return -1; } \
    } while (0)

int main() {
    RUN(test_add_three);
    RUN(test_remove_front);
    RUN(test_remove_back);
    RUN(test_remove_middle);
    RUN(test_remove_all);
    return 0;
}
