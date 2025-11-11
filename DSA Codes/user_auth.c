#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "user_auth.h"

void clean(char *str) {
    for (int i = 0; str[i]; i++) {
        if (str[i] == '\n' || str[i] == '\r')
            str[i] = '\0';
    }
}
int signup_user() {
    char username[USERNAME_LEN], password[PASSWORD_LEN];
    printf("\n--- SIGN UP ---\n");
    printf("Enter new username: ");
    scanf("%s", username);
    printf("Enter password: ");
    scanf("%s", password);
    FILE *f = fopen(USER_FILE, "r");
    if (f) {
        char u[USERNAME_LEN], p[PASSWORD_LEN];
        while (fscanf(f, "%s %s", u, p) == 2) {
            if (strcmp(u, username) == 0) {
                printf("Username already exists. Try another one.\n");
                fclose(f);
                return 0;
            }
        }
        fclose(f);
    }
    f = fopen(USER_FILE, "a");
    if (!f) {
        printf("Error opening user file.\n");
        return 0;
    }
    fprintf(f, "%s %s\n", username, password);
    fclose(f);
    printf("Signup successful! You can now login.\n");
    return 1;
}
int login_user(UserSession *session) {
    char username[USERNAME_LEN], password[PASSWORD_LEN];
    printf("\n--- LOGIN ---\n");
    printf("Enter username: ");
    scanf("%s", username);
    printf("Enter password: ");
    scanf("%s", password);
    FILE *f = fopen(USER_FILE, "r");
    if (!f) {
        printf("No users found. Please sign up first.\n");
        return 0;
    }
    char u[USERNAME_LEN], p[PASSWORD_LEN];
    int found = 0;
    while (fscanf(f, "%s %s", u, p) == 2) {
        clean(u);
        clean(p);
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
void logout_user(UserSession *session) {
    session->is_logged_in = 0;
    strcpy(session->username, "");
    printf("You have been logged out.\n");
}
