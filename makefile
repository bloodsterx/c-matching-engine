CC     = gcc
CFLAGS = -Wall -Wextra -I lib -fsanitize=address,undefined -g
SRC    = src/order.c

TESTS = \
	build/order_creation_destruction \
	build/orderlist_dll

build/%: test/%/test.c $(SRC)
	@mkdir -p build
	$(CC) $(CFLAGS) $^ -o $@

test: $(TESTS)
	@for t in $(TESTS); do $$t && echo "PASS: $$t" || echo "FAIL: $$t"; done

clean:
	rm -rf build
