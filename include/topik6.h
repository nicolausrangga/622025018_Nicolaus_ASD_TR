#ifndef TOPIK6_H
#define TOPIK6_H

#include "topik1.h"

typedef struct UserNode {
    char username[MAX_USERNAME + 1];
    char password[MAX_PASSWORD + 1];
    struct UserNode *next;
} UserNode;

unsigned long auth_hash(const char *username);
UserNode *auth_find_user(const AppState *app, const char *username);
void auth_load_users(AppState *app);
int auth_register(AppState *app);
int auth_login(AppState *app);
void auth_free_users(AppState *app);

#endif
