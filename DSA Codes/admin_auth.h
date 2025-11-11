#ifndef ADMIN_AUTH_H
#define ADMIN_AUTH_H
#define ADMIN_FILE "admins.txt"
#define ADMIN_USERNAME_LEN 50
#define ADMIN_PASSWORD_LEN 50

typedef struct {
    char username[ADMIN_USERNAME_LEN];
    int is_logged_in;
} AdminSession;
int admin_signup();
int admin_login(AdminSession *session);
void admin_logout(AdminSession *session);
#endif