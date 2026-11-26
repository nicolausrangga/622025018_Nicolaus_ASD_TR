#include "topik6.h"
#include "topik2.h"

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

unsigned long auth_hash(const char *username) {
    unsigned long hash = 5381UL;
    int character;

    while ((character = (unsigned char)*username++) != 0) {
        hash = ((hash << 5) + hash) + (unsigned long)tolower(character);
    }
    return hash % HASH_TABLE_SIZE;
}

UserNode *auth_find_user(const AppState *app, const char *username) {
    unsigned long index = auth_hash(username);
    UserNode *current = app->user_table[index];

    while (current != NULL) {
        if (strcmp(current->username, username) == 0) {
            return current;
        }
        current = current->next;
    }
    return NULL;
}

static int valid_credential(const char *value, size_t max_length) {
    size_t i;
    size_t length = strlen(value);
    if (length == 0 || length > max_length) {
        return 0;
    }
    for (i = 0; i < length; i++) {
        if (value[i] == '|' || isspace((unsigned char)value[i])) {
            return 0;
        }
    }
    return 1;
}

static int insert_user_node(AppState *app, const char *username, const char *password) {
    unsigned long index;
    UserNode *node;

    if (auth_find_user(app, username) != NULL) {
        return 0;
    }
    node = (UserNode *)malloc(sizeof(UserNode));
    if (node == NULL) {
        return 0;
    }
    snprintf(node->username, sizeof(node->username), "%s", username);
    snprintf(node->password, sizeof(node->password), "%s", password);
    index = auth_hash(username);
    node->next = app->user_table[index];
    app->user_table[index] = node;
    return 1;
}

void auth_load_users(AppState *app) {
    FILE *file = fopen(USERS_FILE, "r");
    char line[128];
    char username[MAX_USERNAME + 1];
    char password[MAX_PASSWORD + 1];

    if (file == NULL) {
        return;
    }
    while (fgets(line, sizeof(line), file) != NULL) {
        if (sscanf(line, "%31[^|]|%31[^\r\n]", username, password) == 2) {
            (void)insert_user_node(app, username, password);
        }
    }
    fclose(file);
}

int auth_register(AppState *app) {
    char username[MAX_INPUT];
    char password[MAX_INPUT];
    FILE *file;

    printf("\n=== REGISTER AKUN ===\n");
    read_line("Buat username (tanpa spasi): ", username, sizeof(username));
    read_line("Buat password (tanpa spasi): ", password, sizeof(password));

    if (!valid_credential(username, MAX_USERNAME) || !valid_credential(password, MAX_PASSWORD)) {
        printf("Register gagal. Username/password wajib diisi, maksimal %d karakter, tanpa spasi atau karakter '|'.\n",
               MAX_USERNAME);
        return 0;
    }
    if (auth_find_user(app, username) != NULL) {
        printf("Register gagal. Username sudah digunakan.\n");
        return 0;
    }

    file = fopen(USERS_FILE, "a");
    if (file == NULL) {
        printf("Register gagal. File akun tidak dapat dibuka.\n");
        return 0;
    }
    {
        int write_ok = fprintf(file, "%s|%s\n", username, password) >= 0;
        int close_ok = fclose(file) == 0;
        if (!write_ok || !close_ok) {
            printf("Register gagal. Data akun tidak dapat disimpan.\n");
            return 0;
        }
    }

    if (!insert_user_node(app, username, password)) {
        printf("Akun tersimpan, tetapi gagal dimuat ke memori. Silakan mulai ulang program.\n");
        return 0;
    }
    printf("Register berhasil. Silakan login menggunakan akun baru.\n");
    return 1;
}

int auth_login(AppState *app) {
    char username[MAX_INPUT];
    char password[MAX_INPUT];
    UserNode *user;

    printf("\n=== LOGIN ===\n");
    read_line("Username: ", username, sizeof(username));
    read_line("Password: ", password, sizeof(password));

    if (username[0] == '\0' || password[0] == '\0') {
        printf("Login gagal. Username dan password tidak boleh kosong.\n");
        return 0;
    }

    user = auth_find_user(app, username);
    if (user == NULL || strcmp(user->password, password) != 0) {
        printf("Login gagal. Username atau password salah.\n");
        return 0;
    }

    snprintf(app->current_username, sizeof(app->current_username), "%s", user->username);
    printf("Login berhasil. Selamat datang, %s!\n", app->current_username);
    return 1;
}

void auth_free_users(AppState *app) {
    size_t i;
    for (i = 0; i < HASH_TABLE_SIZE; i++) {
        UserNode *current = app->user_table[i];
        while (current != NULL) {
            UserNode *next = current->next;
            free(current);
            current = next;
        }
        app->user_table[i] = NULL;
    }
}
