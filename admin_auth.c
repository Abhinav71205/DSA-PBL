#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "admin_auth.h"

int admin_signup() {
    char username[ADMIN_USERNAME_LEN], password[ADMIN_PASSWORD_LEN];
    printf("\n--- ADMIN SIGN UP ---\n");
    printf("Enter new admin username: ");
    scanf("%s", username);
    printf("Enter password: ");
    scanf("%s", password);
    FILE *f = fopen(ADMIN_FILE, "r");
    if (f) {
        char u[ADMIN_USERNAME_LEN], p[ADMIN_PASSWORD_LEN];
        while (fscanf(f, "%s %s", u, p) == 2) {
            if (strcmp(u, username) == 0) {
                printf("Username already exists. Try another.\n");
                fclose(f);
                return 0;
            }
        }
        fclose(f);
    }
    f = fopen(ADMIN_FILE, "a");
    if (!f) {
        printf("Error opening admin file.\n");
        return 0;
    }
    fprintf(f, "%s %s\n", username, password);
    fclose(f);
    printf("Signup successful! You can now login.\n");
    return 1;
}
int admin_login(AdminSession *session) {
    char username[ADMIN_USERNAME_LEN], password[ADMIN_PASSWORD_LEN];
    printf("\n--- ADMIN LOGIN ---\n");
    printf("Username: ");
    scanf("%s", username);
    printf("Password: ");
    scanf("%s", password);
    FILE *f = fopen(ADMIN_FILE, "r");
    if (!f) {
        printf("No admin file found. Please sign up first.\n");
        return 0;
    }
    char u[ADMIN_USERNAME_LEN], p[ADMIN_PASSWORD_LEN];
    int found = 0;
    while (fscanf(f, "%s %s", u, p) == 2) {
        if (strcmp(u, username) == 0 && strcmp(p, password) == 0) {
            found = 1;
            break;
        }
    }
    fclose(f);
    if (found) {
        strcpy(session->username, username);
        session->is_logged_in = 1;
        printf("Login successful! Welcome, %s.\n", username);
        return 1;
    } else {
        printf("Invalid username or password.\n");
        return 0;
    }
}
void admin_logout(AdminSession *session) {
    session->is_logged_in = 0;
    strcpy(session->username, "");
    printf("Admin logged out successfully.\n");
}
