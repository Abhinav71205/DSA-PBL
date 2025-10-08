#include <stdio.h>
#define MAX_ORDERS 50
int orders[MAX_ORDERS];
int front = -1, rear = -1;
void enqueueOrder(int orderId) {
    if (rear == MAX_ORDERS - 1) {
        printf("Order Queue Full!\n");
        return;
    }
    if (front == -1) front = 0;
    orders[++rear] = orderId;
    printf("Order %d added to queue!\n", orderId);
}
void dequeueOrder() {
    if (front == -1 || front > rear) {
        printf("No pending orders!\n");
        return;
    }
    printf("Order %d processed and removed from queue.\n", orders[front++]);
}
void displayOrders() {
    if (front == -1 || front > rear) {
        printf("No orders in queue.\n");
        return;
    }
    printf("\n--- Pending Orders ---\n");
    for (int i = front; i <= rear; i++) {
        printf("Order ID: %d\n", orders[i]);
    }
}