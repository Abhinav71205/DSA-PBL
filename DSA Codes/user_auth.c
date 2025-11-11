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
    printf("Enter username: ");
    scanf("%s", username);
    printf("Enter password: ");
    scanf("%s", password);

    FILE *f = fopen(USER_FILE, "a");
    if (!f) {
        printf("Error opening file.\n");
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
        printf("No user file found.\n");
        return 0;
    }

    char file_user[USERNAME_LEN], file_pass[PASSWORD_LEN];
    int found = 0;

    while (fscanf(f, "%s %s", file_user, file_pass) == 2) {
        clean(file_user);
        clean(file_pass);

        if (strcmp(file_user, username) == 0 && strcmp(file_pass, password) == 0) {
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
