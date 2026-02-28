#ifndef ORDER_H
#define ORDER_H

#include <stdlib.h>
#include <time.h>

typedef enum {
    BUY, SELL
} Side;

typedef struct {
    long id;
    char *user_name;
    int price;
    int volume;
    Side side;
    time_t timestamp;
} Order;

typedef struct OrderNode {
    Order *order;
    struct OrderNode *prev;
    struct OrderNode *next;
} OrderNode;


typedef struct {
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

void destroy_orderlist(OrderList* order_list);

int add_order(OrderList *order_list, Order *order);

int remove_order(OrderNode* order_node);



/*
insert (append latest timestamp to head)
remove (O(1) lookup from hashmap to get node)


*/




#endif
