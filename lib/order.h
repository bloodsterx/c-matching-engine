#ifndef ORDER_H
#define ORDER_H

#include <stdlib.h>
#include <time.h>

typedef enum {
    BUY, SELL
} Side;

/*
args:
    - long id;
    - char *user_name;
    - int price;
    - int volume;
    - Side side;
    - time_t timestamp;
*/
typedef struct {
    long id;
    char *user_name;
    int price;
    int volume;
    Side side;
    time_t timestamp;
} Order;

/*
args:
    - Order *order;
    - struct OrderNode *prev;
    - struct OrderNode *next;
*/
typedef struct OrderNode {
    Order *order;
    struct OrderNode *prev;
    struct OrderNode *next;
} OrderNode;

/*
args:
    - int price; price level this list represents
    - OrderNode* head; dummy pointer always pointing to the head if list not empty, else points to tail
    - OrderNode* tail; likewise for head
    - size_t size;
*/
typedef struct {
    int price;
    OrderNode *head;
    OrderNode *tail;
    size_t size;
} OrderList;

Order* create_order(
    long id, 
    char* user_name, 
    int price, 
    int volume, 
    Side side,
    time_t timestamp
);


OrderList* init_orderlist();

OrderNode* init_ordernode();

int add_order(OrderList *order_list, Order *order);

int remove_order(OrderList *order_list, OrderNode *order_node);

void destroy_orderlist(OrderList* order_list);

int destroy_ordernode(OrderNode* order_node);

void destroy_order(Order* order);

void print_orderlist(const OrderList *order_list);




/*
insert (append latest timestamp to head)
remove (O(1) lookup from hashmap to get node)


*/
#endif