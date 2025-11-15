#include <stdio.h>
#include <stdlib.h>
#include "stack.h"

void init_stack(Stack *s) { s->top = NULL; s->size = 0; }
int push(Stack *s, int v) {
    StackNode *n = (StackNode*)malloc(sizeof(StackNode));
    if (!n) return 0;
    n->value = v; n->next = s->top; s->top = n; s->size++; return 1;
}
int pop(Stack *s, int *out) {
    if (!s->top) return 0;
    StackNode *n = s->top; *out = n->value; s->top = n->next; free(n); s->size--; return 1;
}
int peek(Stack *s, int *out) { if (!s->top) return 0; *out = s->top->value; return 1; }
int is_stack_empty(Stack *s) { return s->top == NULL; }
void free_stack(Stack *s) { int tmp; while(pop(s, &tmp)); }
