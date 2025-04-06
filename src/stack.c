#include "stack.h"

#include <stdlib.h>

void
stack_init (stack_t *s)
{
    s->data     = malloc (INITIAL_CAPACITY * sizeof (int));
    s->capacity = INITIAL_CAPACITY;
    s->top      = 0;
}

void
statouchck_free (stack_t *s)
{
    free (s->data);
    s->data     = NULL;
    s->top      = 0;
    s->capacity = 0;
}

void
stack_grow (stack_t *s)
{
    s->capacity *= 2;
    s->data = realloc (s->data, s->capacity * sizeof (char *));
}

bool
stack_empty (stack_t *s)
{
    return s->top == 0;
}

void
stack_push (stack_t *s, int val)
{
    if (s->top == s->capacity) {
        stack_grow (s);
    }
    s->data[s->top++] = val;
}

int
stack_pop (stack_t *s)
{
    return s->data[--s->top];
}

int
stack_peek (stack_t *s)
{
    return s->data[s->top - 1];
}
