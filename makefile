CC     = gcc
BUILD_DIR ?= build

CFLAGS_COMMON = -Wall -Wextra -I lib -g
SANITIZERS   = -fsanitize=address,undefined

# Default builds use sanitizers (fast feedback).
CFLAGS ?= $(CFLAGS_COMMON) $(SANITIZERS)
SRC    = src/order.c src/heap.c

TEST_BINS = order_creation_destruction orderlist_dll test_heap
TESTS = $(addprefix $(BUILD_DIR)/,$(TEST_BINS))

$(BUILD_DIR)/%: test/%/test.c $(SRC)
	@mkdir -p $(BUILD_DIR)
	$(CC) $(CFLAGS) $^ -o $@

test: $(TESTS)
	@i=1; fail=0; \
	for t in $(TESTS); do \
		echo "=== RUN TEST $$i: $$t ==="; \
		$$t; rc=$$?; \
		if [ $$rc -eq 0 ]; then \
			echo "TEST $$i PASSED"; \
		else \
			echo "TEST $$i FAILED (exit $$rc)"; \
			fail=1; \
		fi; \
		echo ""; \
		i=$$((i+1)); \
	done; \
	if [ $$fail -eq 0 ]; then \
		echo "ALL TESTS PASSED"; \
	else \
		echo "SOME TESTS FAILED"; \
		exit 1; \
	fi

# Run tests under Valgrind (Linux). Use: make test-valgrind
# Override with: make test-valgrind VALGRIND=/path/to/valgrind ...
VALGRIND ?= /usr/bin/valgrind
VALGRIND_OPTS = --leak-check=full --show-leak-kinds=all --error-exitcode=1
VG_BUILD_DIR ?= build-valgrind
VG_CFLAGS = $(CFLAGS_COMMON) -O0 -fno-omit-frame-pointer
VG_TESTS = $(addprefix $(VG_BUILD_DIR)/,$(TEST_BINS))

$(VG_BUILD_DIR)/%: test/%/test.c $(SRC)
	@mkdir -p $(VG_BUILD_DIR)
	$(CC) $(VG_CFLAGS) $^ -o $@

test-valgrind: $(VG_TESTS)
	@command -v $(VALGRIND) >/dev/null 2>&1 || { echo "Valgrind not found. Install with: sudo apt install valgrind"; exit 1; }; \
	i=1; fail=0; \
	for t in $(VG_TESTS); do \
		echo "=== RUN TEST $$i (Valgrind): $$t ==="; \
		$(VALGRIND) $(VALGRIND_OPTS) $$t; rc=$$?; \
		if [ $$rc -eq 0 ]; then \
			echo "TEST $$i PASSED"; \
		else \
			echo "TEST $$i FAILED (exit $$rc)"; \
			fail=1; \
		fi; \
		echo ""; \
		i=$$((i+1)); \
	done; \
	if [ $$fail -eq 0 ]; then \
		echo "ALL VALGRIND TESTS PASSED"; \
	else \
		echo "SOME VALGRIND TESTS FAILED"; \
		exit 1; \
	fi

clean:
	rm -rf $(BUILD_DIR) $(VG_BUILD_DIR)
