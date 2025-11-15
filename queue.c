#include <stdio.h>
#include <stdlib.h>
#include "queue.h"
void init_queue(Queue *q) {
    q->front = q->rear = NULL;
    q->size = 0;
}

int enqueue(Queue *q, int v) {
    QNode *n = (QNode*)malloc(sizeof(QNode));
    if (!n) return 0;

    n->value = v;
    n->next = NULL;

    if (!q->rear) {
        
        q->front = q->rear = n;
    } else {
        q->rear->next = n;
        q->rear = n;
    }

    q->size++;
    return 1;
}


int dequeue(Queue *q, int *out) {
    if (!q->front) return 0; 

    QNode *n = q->front;
    *out = n->value;

    q->front = n->next;
    if (!q->front)
        q->rear = NULL; 

    free(n);
    q->size--;
    return 1;
}


int is_queue_empty(Queue *q) {
    return (q->front == NULL);
}


void free_queue(Queue *q) {
    int tmp;
    while (dequeue(q, &tmp));
}