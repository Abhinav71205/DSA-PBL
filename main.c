#include <stdio.h>
#include "product_management.c"
#include "cart_and_order.c"
#include "queue.c"
#include "stack.c"
#include "billing.c"
#include "inventory.c"
void menu() {
    int choice;
    while (1) {
        printf("\n--- E-Commerce Inventory & Cart System ---\n");
        printf("1. Add Product\n2. Display Products\n3. Sort Products by Price\n4. Add to Cart\n5. Show Cart\n");
        printf("6. Enqueue Order\n7. Dequeue Order\n8. Display Orders\n9. Push Order to History\n10. Display History\n");
        printf("11. Generate Bill\n12. Exit\n");
        printf("Enter choice: ");
        scanf("%d", &choice);

        if (choice == 1) addProduct();
        else if (choice == 2) displayProducts();
        else if (choice == 3) sortProductsByPrice();
        else if (choice == 4) {
            int id, qty;
            printf("Enter Product ID: ");
            scanf("%d", &id);
            printf("Enter Quantity: ");
            scanf("%d", &qty);
            addToCart(id, qty);
        } else if (choice == 5) displayCart();
        else if (choice == 6) {
            int id;
            printf("Enter Order ID: ");
            scanf("%d", &id);
            enqueueOrder(id);
        } else if (choice == 7) dequeueOrder();
        else if (choice == 8) displayOrders();
        else if (choice == 9) {
            int id;
            printf("Enter Completed Order ID: ");
            scanf("%d", &id);
            pushOrder(id);
        } else if (choice == 10) displayOrderHistory();
        else if (choice == 11) generateBill();
        else if (choice == 12) break;
        else printf("Invalid choice!\n");
    }
}
int main() {
    menu();
    return 0;
}