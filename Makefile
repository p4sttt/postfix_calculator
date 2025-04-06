CC = gcc
CFLAGS = -Wall -Wextra -std=c99 -pedantic -I./include -g

SRC_DIR = src
BUILD_DIR = build
INCLUDE_DIR = include

COMMON_SRCS = $(filter-out $(SRC_DIR)/eval.c $(SRC_DIR)/postfix.c,$(wildcard $(SRC_DIR)/*.c))
COMMON_OBJS = $(patsubst $(SRC_DIR)/%.c,$(BUILD_DIR)/%.o,$(COMMON_SRCS))

EVAL_BIN = eval
POSTFIX_BIN = postfix

all: $(EVAL_BIN) $(POSTFIX_BIN)

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c | $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

$(EVAL_BIN): $(BUILD_DIR)/eval.o $(COMMON_OBJS)
	$(CC) $(CFLAGS) $^ -o $@

$(BUILD_DIR)/eval.o: $(SRC_DIR)/eval.c | $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(POSTFIX_BIN): $(BUILD_DIR)/postfix.o $(COMMON_OBJS)
	$(CC) $(CFLAGS) $^ -o $@

$(BUILD_DIR)/postfix.o: $(SRC_DIR)/postfix.c | $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -rf $(BUILD_DIR) $(EVAL_BIN) $(POSTFIX_BIN)

.PHONY: all clean
