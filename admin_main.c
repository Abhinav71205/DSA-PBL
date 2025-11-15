#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "inventory.h"
#include "admin_auth.h"
void admin_menu(Inventory *inv, const char *filename) {
    int ch;
    while (1) {
        printf("\n--- ADMIN MENU ---\n1.List products\n2.Add product\n3.Update qty\n4.Remove product\n5.Save & Exit\nChoose: ");
        if (scanf("%d", &ch) != 1) break;
        if (ch == 1) list_products(inv);
        else if (ch == 2) {
            int id, qty; char name[NAME_LEN]; float price;
            printf("Enter id name price qty: ");
            if (scanf("%d %49s %f %d", &id, name, &price, &qty) != 4) {
                printf("Invalid input.\n"); continue;
            }
            Product *p = create_product(id, name, price, qty);
            add_product(inv, p);
            printf("Product added successfully.\n");
        } else if (ch == 3) {
            int id, qty;
            printf("Enter product ID and new qty: ");
            if (scanf("%d %d", &id, &qty) != 2) continue;
            if (update_product_qty(inv, id, qty))
                printf("Quantity updated.\n");
            else
                printf("Product not found.\n");
        } else if (ch == 4) {
            int id;
            printf("Enter product ID to remove: ");
            if (scanf("%d", &id) != 1) continue;
            if (remove_product(inv, id))
                printf("Product removed.\n");
            else
                printf("Product not found.\n");
        } else if (ch == 5) {
            save_inventory_to_file(inv, filename);
            break;
        }
    }
}
int main(int argc, char **argv) {
    AdminSession session = {"", 0};
    int choice;
    while (1) {
        printf("\n--- ADMIN ACCESS ---\n1.Sign Up\n2.Login\n3.Exit\nChoose: ");
        scanf("%d", &choice);

        if (choice == 1)
            admin_signup();
        else if (choice == 2) {
            if (admin_login(&session))
                break;
        } else if (choice == 3)
            return 0;
    }
    Inventory inv;
    init_inventory(&inv);
    const char *file = "products.txt";
    if (argc > 1) file = argv[1];
    load_inventory_from_file(&inv, file);
    printf("\nWelcome, %s! You can now manage the inventory.\n", session.username);
    admin_menu(&inv, file);
    free_inventory(&inv);
    admin_logout(&session);
    printf("Goodbye, %s!\n", session.username);
    return 0;
}
