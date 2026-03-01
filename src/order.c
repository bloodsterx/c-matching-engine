#include "../lib/order.h"

#include <stdio.h>

Order* create_order(
    long id, 
    char* user_name, 
    int price, 
    int volume, 
    Side side,
    time_t timestamp
) {
    Order* order = (Order*) malloc(sizeof(Order));
    if (!order) return NULL;

    order->id = id;
    order->user_name = user_name;
    order->price = price;
    order->volume = volume;
    order->side = side;
    order->timestamp = timestamp;
    return order;
}


OrderNode* init_ordernode() {
    OrderNode* order_node = (OrderNode*) malloc(sizeof(OrderNode));
    if (!order_node) return NULL;
    order_node->next = NULL;
    order_node->prev = NULL;
    order_node->order = NULL;
    return order_node;
}

OrderList* init_orderlist() {
    OrderList* order_list = (OrderList*) malloc(sizeof(OrderList));
    if (!order_list) return NULL;

    OrderNode* tail_ptr = init_ordernode();
    if (!tail_ptr) {
        free(order_list);
        return NULL;
    }

    OrderNode* head_ptr = init_ordernode();
    if (!head_ptr) {
        free(tail_ptr);
        free(order_list);
        return NULL;
    }

    head_ptr->prev = tail_ptr;
    tail_ptr->next = head_ptr;

    order_list->tail = tail_ptr;
    order_list->head = head_ptr;
    order_list->size = 0;
    return order_list;
}


/*
    Adds order to the order DLL at a given price
    Caller will handle adding this order to the global hashmap

*/
int add_order(OrderList *order_list, Order *order) {
    if (!order) return -1;
    if (!order_list) {
        printf("err: order_list is NULL\n");
        return -1;
    }
    OrderNode* new_node = init_ordernode();
    if (!new_node) return -1;

    new_node->order = order;

    // tail <-> head (size = 0)
    // tail <-> new_node <-> head (size = 1)
    OrderNode* old_tail = order_list->tail->next;

    old_tail->prev = new_node;
    new_node->next = old_tail;

    order_list->tail->next = new_node;
    new_node->prev = order_list->tail;

    order_list->size++;
    
    return 0;
}

/*
    Handles order removal operation in the order's order_list. 
    Responsible for decrementing and changing order_list state.
    **NOT RESPONSIBLE** for:
    - changing hashmap state (caller)
    - changing heap state

*/
int remove_order(OrderList *order_list, OrderNode *order_node) {
    if (!order_list || !order_node) return -1;

    if (!order_node->order) return -1;

    Order* order = order_node->order;

    destroy_order(order);
    order_node->order = NULL;

    order_node->prev->next = order_node->next;
    order_node->next->prev = order_node->prev;
    
    int res = destroy_ordernode(order_node);
    if (res != 0) return res; // extremely unlikely

    order_list->size--;
    return 0;
}

/*
    frees the order_node. Not responsible for freeing the internal Order struct.
    If order exists, will return error (-1)

    E.g. order of ops:
    1. cancel order called -> pop ordernode from orderid hashmap 
    2. get price and retrieve orderlist from price hashmap
    3. Remove order in ordernode
    4. Remove ordernode from orderList and decrement size
    then remove ordernode and reattach links

*/
int destroy_ordernode(OrderNode *order_node) {
    if (!order_node) return -1;

    if (order_node->order) {
        printf("err: Attempted to remove order node while order struct still exists\n");
        return -1;
    }

    free(order_node);
    return 0;
}


void destroy_orderlist(OrderList* order_list) {
    if (!order_list) return;
    if (order_list->size > 0) {
        printf("err: tried to remove order_list with %zu active order(s)\n", order_list->size);
        return;
    }

    // free the default pointers
    free(order_list->head);
    free(order_list->tail);
    free(order_list);
}

void print_orderlist(const OrderList *order_list) {
    if (!order_list) {
        printf("NULL\n");
        return;
    }
    printf("TAIL");
    OrderNode *cur = order_list->tail->next;
    while (cur != order_list->head) {
        printf(" <-> [id=%ld vol=%d]", cur->order->id, cur->order->volume);
        cur = cur->next;
    }
    printf(" <-> HEAD  (size=%zu)\n", order_list->size);
}

void destroy_order(Order* order) {
    if (!order) return;
    if (order->user_name) free(order->user_name);
    free(order);
}

