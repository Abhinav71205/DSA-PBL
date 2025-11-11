#ifndef QUEUE_H
#define QUEUE_H

typedef struct QNode {
    int value;              
    struct QNode *next;     
} QNode;


typedef struct Queue {
    QNode *front;           
    QNode *rear;
    int size;               
} Queue;

void init_queue(Queue *q);

int enqueue(Queue *q, int v);
int dequeue(Queue *q, int *out);


int is_queue_empty(Queue *q);


void free_queue(Queue *q);

#endif
