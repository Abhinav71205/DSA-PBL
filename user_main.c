#include <stdio.h>
#include <stdlib.h>
#include "inventory.h"
#include "cart_order.h"
#include "billing.h"
#include "user_auth.h"
#include "order_history.h"

void user_menu(Inventory *inv, UserSession *session) {
    Cart cart;
    init_cart(&cart);
    int ch;

    while (1) {
        printf("\n--- USER MENU (%s) ---\n", session->username);
        printf("1. Browse products\n");
        printf("2. Add to cart\n");
        printf("3. View cart\n");
        printf("4. Checkout\n");
        printf("5. View Order History\n");
        printf("6. Logout\n");
        printf("Choose: ");

        if (scanf("%d", &ch) != 1) break;

        if (ch == 1)
            list_products(inv);
        else if (ch == 2) {
            int id, qty;
            printf("Enter Product ID and Quantity: ");
            if (scanf("%d %d", &id, &qty) != 2) continue;

            Product *p = find_product_by_id(inv, id);
            if (!p) {
                printf("Product not found.\n");
                continue;
            }
            if (p->qty < qty) {
                printf("Not enough stock.\n");
                continue;
            }

            add_to_cart(&cart, id, qty);
            printf("Added to cart successfully.\n");
        }
        else if (ch == 3)
            list_cart(&cart, inv);
        else if (ch == 4) {
            if (cart.head == NULL) {
                printf("Cart is empty! Add items before checkout.\n");
                continue;
            }

            print_bill(&cart, inv);
            save_order_to_file(session->username, &cart, inv);

            CartItem *cur = cart.head;
            while (cur) {
                Product *p = find_product_by_id(inv, cur->product_id);
                if (p) p->qty -= cur->qty;
                cur = cur->next;
            }

            clear_cart(&cart);
            printf("Thank you for your purchase!\n");
        }
        else if (ch == 5)
            show_order_history(session->username);
        else if (ch == 6) {
            clear_cart(&cart);
            logout_user(session);
            break;
        }
        else
            printf("Invalid choice. Try again.\n");
    }
    clear_cart(&cart);
}

int main(int argc, char **argv) {
    UserSession session = {"", 0};
    int choice;

    while (1) {
        printf("\n--- ACCOUNT ---\n");
        printf("1. Sign Up\n");
        printf("2. Login\n");
        printf("3. Exit\n");
        printf("Choose: ");

        if (scanf("%d", &choice) != 1) break;

        if (choice == 1)
            signup_user();
        else if (choice == 2) {
            if (login_user(&session))
                break;
        }
        else if (choice == 3)
            return 0;
        else
            printf("Invalid choice.\n");
    }

    Inventory inv;
    init_inventory(&inv);

    const char *file = "products.txt";
    if (argc > 1) file = argv[1];

    load_inventory_from_file(&inv, file);
    user_menu(&inv, &session);
    save_inventory_to_file(&inv, file);
    free_inventory(&inv);

    return 0;
}
