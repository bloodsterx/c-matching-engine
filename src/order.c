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
    if (!tail_ptr) return NULL;

    OrderNode* head_ptr = init_ordernode();
    if (!head_ptr) return NULL;

    head_ptr->prev = tail_ptr;
    tail_ptr->next = head_ptr;

    order_list->tail = tail_ptr;
    order_list->head = head_ptr;
    order_list->size = 0;
    return order_list;
}

int add_order(OrderList *order_list, Order *order) {
    if (!order) return -1;
    if (!order_list) {
        printf("err: order_list is NULL\n");
        return -1;
    }
    OrderNode* order_node = init_ordernode(order);

    if (!order_node) return -1;
    if (!order_list->head && !order_list->tail) {
        // order list initialised but no orders (e.g. all liquidity at price level previously depleted)
        // decision point: keep in memory if a price level has all volume consumed? Or delete list and create new
        order_list->head = order_node;
        order_list->tail = order_list->head;
    } 
    else if (order_list->head && order_list->tail) {
        // expected (head <-> tail)
        OrderNode* prev = order_list->head->prev;
        if (!prev)
        prev->next = order_node;
        order_node->prev = prev
    }
}



void destroy_orderlist(OrderList* order_list) {
    if (!order_list) return;
    if (order_list->head || order_list->tail) {
        printf("err: tried to remove order_list with at least one active order\n");
        return;
    }
    free(order_list);
}

void destroy_order(Order* order) {
    if (!order) return;
    if (order->user_name) free(order->user_name);
    free(order);
}

// /*
//  * Frees the order node ONLY. Caller is responsible freeing/reattaching any links. 
//  */
// void destroy_ordernode(OrderNode* order_node) {
//     if (!order_node) return;
//     if (order_node->order) {
//         printf("err: tried to free order node with an active order\n");
//         return;
//     }
//     if (order_node->next || order_node->prev) {
//         printf("err: tried to free order node with at least one attached prev/next links\n");
//         return;
//     }
//     free(order_node);
// }