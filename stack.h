#ifndef STACK_H
#define STACK_H

typedef struct StackNode {
    int value;
    struct StackNode *next;
} StackNode;

typedef struct Stack {
    StackNode *top;
    int size;
} Stack;

void init_stack(Stack *s);
int push(Stack *s, int v);
int pop(Stack *s, int *out);
int peek(Stack *s, int *out);
int is_stack_empty(Stack *s);
void free_stack(Stack *s);

#endif 