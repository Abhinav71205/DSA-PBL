#ifndef USER_AUTH_H
#define USER_AUTH_H

#define USER_FILE "C:\\Users\\Abhinav\\Desktop\\users.txt"  
#define USERNAME_LEN 50
#define PASSWORD_LEN 50

typedef struct {
    char username[USERNAME_LEN];
    int is_logged_in;
} UserSession;

int login_user(UserSession *session);
void logout_user(UserSession *session);

#endif
