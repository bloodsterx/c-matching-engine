#include "../../lib/order.h"
#include <stdio.h>

int create_destroy_orderlist() {
    OrderList* ol = init_orderlist();
    if (ol->size != 0) {
        return -1;
    }
    destroy_orderlist(ol);
    return 0;
}

int main() {
    int failed = 0;

    printf("==== ORDER CREATION/DESTRUCTION TESTS BEGIN ====\n");
    printf("  [ORDER TEST 1] create_destroy_orderlist ...\n");
    int rc = create_destroy_orderlist();
    if (rc == 0) {
        printf("  [ORDER TEST 1] create_destroy_orderlist PASSED\n\n");
    } else {
        printf("  [ORDER TEST 1] create_destroy_orderlist FAILED (rc=%d)\n\n", rc);
        failed = 1;
    }
    printf("===== ORDER CREATION/DESTRUCTION TESTS END =====\n");

    if (failed) {
        printf("One or more ORDER tests FAILED.\n");
        return -1;
    }
    printf("All ORDER tests PASSED.\n");
    return 0;
}