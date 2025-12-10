CC = gcc

CFLAGS = -Wall -Wextra -g -std=c99
INCLUDES = -I. -I../datast -I../utils

BUILD_DIR = build

TARGETS = sorting_test llist_test bitree_test

TEST_OBJS = $(BUILD_DIR)/test_sorting.o $(BUILD_DIR)/test_linkedlist.o $(BUILD_DIR)/test_binary_tree.o

LIB_OBJS_SORTING = $(BUILD_DIR)/sorting.o
LIB_OBJS_LLIST = $(BUILD_DIR)/linkedlist.o $(BUILD_DIR)/nodes.o
LIB_OBJS_BITREE = $(BUILD_DIR)/binary_tree.o $(BUILD_DIR)/nodes.o

all: $(TARGETS)

sorting_test: $(BUILD_DIR)/test_sorting.o $(LIB_OBJS_SORTING)
	$(CC) $(CFLAGS) $^ -o $@

llist_test: $(BUILD_DIR)/test_linkedlist.o $(LIB_OBJS_LLIST)
	$(CC) $(CFLAGS) $^ -o $@

bitree_test: $(BUILD_DIR)/test_binary_tree.o $(LIB_OBJS_BITREE)
	$(CC) $(CFLAGS) $^ -o $@

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

$(BUILD_DIR)/test_%.o: test_%.c | $(BUILD_DIR)
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

$(BUILD_DIR)/sorting.o: ../utils/sorting.c | $(BUILD_DIR)
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

$(BUILD_DIR)/%.o: ../datast/%.c | $(BUILD_DIR)
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

.PHONY: clean all

clean:
	rm -f $(TARGETS)
	rm -rf $(BUILD_DIR)
