#include <stdio.h>
#define MAX_HISTORY 50
int orderHistory[MAX_HISTORY];
int top = -1;
void pushOrder(int orderId) {
    if (top == MAX_HISTORY - 1) {
        printf("Order history full!\n");
        return;
    }
    orderHistory[++top] = orderId;
}
void popOrder() {
    if (top == -1) {
        printf("No history available!\n");
        return;
    }
    printf("Order %d removed from history.\n", orderHistory[top--]);
}
void displayOrderHistory() {
    if (top == -1) {
        printf("No completed orders in history.\n");
        return;
    }
    printf("\n--- Order History ---\n");
    for (int i = top; i >= 0; i--) {
        printf("Order ID: %d\n", orderHistory[i]);
    }
}