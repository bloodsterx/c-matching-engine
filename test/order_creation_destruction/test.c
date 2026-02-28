#include "../../lib/order.h"

int create_destroy_orderlist() {
    OrderList* ol = init_orderlist();
    if (ol->size != 0) {
        return -1;
    }
    destroy_orderlist(ol);
    return 0;
}

int main() {
    int res = create_destroy_orderlist();
    if (res != 0) {
        return -1;
    }
    return 0;
}