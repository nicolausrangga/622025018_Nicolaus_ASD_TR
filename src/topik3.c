#include "topik3.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static int compare_package(const TicketPackage *left, const TicketPackage *right, int criterion) {
    if (criterion == 1) {
        if (left->price < right->price) return -1;
        if (left->price > right->price) return 1;
    } else if (criterion == 2) {
        if (left->quota < right->quota) return -1;
        if (left->quota > right->quota) return 1;
    } else {
        if (left->id < right->id) return -1;
        if (left->id > right->id) return 1;
    }
    return 0;
}

static int needs_swap(const TicketPackage *left, const TicketPackage *right, int criterion, int descending) {
    int comparison = compare_package(left, right, criterion);
    return descending ? comparison < 0 : comparison > 0;
}

void bubble_sort_packages(TicketPackage *items, size_t count, int criterion, int descending) {
    size_t i;
    size_t j;
    int swapped;
    TicketPackage temporary;

    for (i = 0; i + 1 < count; i++) {
        swapped = 0;
        for (j = 0; j + 1 < count - i; j++) {
            if (needs_swap(&items[j], &items[j + 1], criterion, descending)) {
                temporary = items[j];
                items[j] = items[j + 1];
                items[j + 1] = temporary;
                swapped = 1;
            }
        }
        if (!swapped) {
            break;
        }
    }
}

void selection_sort_packages(TicketPackage *items, size_t count, int criterion, int descending) {
    size_t i;
    size_t j;
    size_t selected_index;
    TicketPackage temporary;

    for (i = 0; i < count; i++) {
        selected_index = i;
        for (j = i + 1; j < count; j++) {
            if (needs_swap(&items[selected_index], &items[j], criterion, descending)) {
                selected_index = j;
            }
        }
        if (selected_index != i) {
            temporary = items[i];
            items[i] = items[selected_index];
            items[selected_index] = temporary;
        }
    }
}

static void print_sorted_packages(const TicketPackage *items, size_t count) {
    size_t i;
    printf("\nHasil pengurutan:\n");
    printf("ID    | Kategori | Tahap    | Harga          | Kuota      | Stok\n");
    printf("---------------------------------------------------------------\n");
    for (i = 0; i < count; i++) {
        print_package(&items[i]);
    }
}

void sorting_menu(const AppState *app) {
    TicketPackage *copy;
    int algorithm;
    int criterion;
    int direction;

    if (app->package_count == 0) {
        printf("Belum ada data paket untuk diurutkan.\n");
        return;
    }
    copy = (TicketPackage *)malloc(app->package_count * sizeof(TicketPackage));
    if (copy == NULL) {
        printf("Memori tidak cukup untuk proses sorting.\n");
        return;
    }
    memcpy(copy, app->packages, app->package_count * sizeof(TicketPackage));

    printf("\n=== SORTING PAKET TIKET ===\n");
    printf("1. Bubble Sort\n2. Selection Sort\n");
    algorithm = read_int_range("Pilih algoritma: ", 1, 2);
    printf("1. Berdasarkan harga\n2. Berdasarkan kuota\n3. Berdasarkan ID paket\n");
    criterion = read_int_range("Pilih kriteria: ", 1, 3);
    printf("1. Ascending (naik)\n2. Descending (turun)\n");
    direction = read_int_range("Pilih arah: ", 1, 2);

    if (algorithm == 1) {
        bubble_sort_packages(copy, app->package_count, criterion, direction == 2);
    } else {
        selection_sort_packages(copy, app->package_count, criterion, direction == 2);
    }
    print_sorted_packages(copy, app->package_count);
    free(copy);
}
