#ifndef STACK_H
#define STACK_H

#include <stdbool.h>
#include <stddef.h>

#define INITIAL_CAPACITY 100

typedef struct {
    int *data;
    size_t capacity;
    size_t top;
} stack_t;

void stack_init (stack_t *s);
void stack_free (stack_t *s);

bool stack_empty (stack_t *s);
void stack_push (stack_t *s, int val);
int stack_pop (stack_t *s);
int stack_peek (stack_t *s);

#endif
