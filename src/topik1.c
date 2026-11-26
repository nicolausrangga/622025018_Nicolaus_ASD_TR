#include "topik1.h"

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void read_line(const char *prompt, char *buffer, size_t buffer_size) {
    size_t length;
    int ch;

    if (buffer == NULL || buffer_size == 0) {
        return;
    }

    printf("%s", prompt);
    if (fgets(buffer, (int)buffer_size, stdin) == NULL) {
        buffer[0] = '\0';
        return;
    }

    length = strlen(buffer);
    if (length > 0 && buffer[length - 1] == '\n') {
        buffer[length - 1] = '\0';
    } else {
        while ((ch = getchar()) != '\n' && ch != EOF) {
            /* Buang sisa input yang melebihi kapasitas buffer. */
        }
    }

    length = strlen(buffer);
    while (length > 0 && (buffer[length - 1] == '\r' || isspace((unsigned char)buffer[length - 1]))) {
        buffer[--length] = '\0';
    }
    while (buffer[0] != '\0' && isspace((unsigned char)buffer[0])) {
        memmove(buffer, buffer + 1, strlen(buffer));
    }
}

int read_int_range(const char *prompt, int min_value, int max_value) {
    char input[MAX_INPUT];
    char *end_pointer;
    long value;

    while (1) {
        read_line(prompt, input, sizeof(input));
        if (input[0] == '\0') {
            printf("Input tidak boleh kosong. Silakan coba lagi.\n");
            continue;
        }

        value = strtol(input, &end_pointer, 10);
        if (*end_pointer != '\0' || end_pointer == input || value < min_value || value > max_value) {
            printf("Input tidak valid. Masukkan angka %d sampai %d.\n", min_value, max_value);
            continue;
        }
        return (int)value;
    }
}

void format_currency(long amount, char *buffer, size_t buffer_size) {
    char raw[64];
    size_t raw_length;
    size_t output_index = 0;
    size_t i;

    snprintf(raw, sizeof(raw), "%ld", amount);
    raw_length = strlen(raw);

    for (i = 0; i < raw_length && output_index + 1 < buffer_size; i++) {
        buffer[output_index++] = raw[i];
        if (i + 1 < raw_length && (raw_length - i - 1) % 3 == 0 && output_index + 1 < buffer_size) {
            buffer[output_index++] = '.';
        }
    }
    buffer[output_index] = '\0';
}

int find_package_index_linear(const AppState *app, int package_id) {
    size_t i;
    for (i = 0; i < app->package_count; i++) {
        if (app->packages[i].id == package_id) {
            return (int)i;
        }
    }
    return -1;
}

int find_transaction_index_linear(const AppState *app, int transaction_id) {
    size_t i;
    for (i = 0; i < app->transaction_count; i++) {
        if (app->transactions[i].id == transaction_id) {
            return (int)i;
        }
    }
    return -1;
}

void print_package(const TicketPackage *package) {
    char price_text[64];
    int remaining;

    format_currency(package->price, price_text, sizeof(price_text));
    remaining = package->quota - package->sold;
    printf("%-5d | %-8s | Presale %-1d | Rp %-12s | Kuota: %-4d | Tersisa: %d\n",
           package->id, package->category, package->presale, price_text,
           package->quota, remaining);
}

void print_transaction(const Transaction *transaction) {
    char total_text[64];
    format_currency(transaction->total, total_text, sizeof(total_text));
    printf("ID: %d | Akun: %s | Paket: %d | Jumlah: %d | Total: Rp %s | Waktu: %s\n",
           transaction->id, transaction->username, transaction->package_id,
           transaction->quantity, total_text, transaction->created_at);
}
