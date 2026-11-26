#include "topik4.h"

#include <stdio.h>

int queue_enqueue(AppState *app, const PurchaseRequest *request) {
    if (app->queue_count >= QUEUE_CAPACITY) {
        return 0;
    }
    app->purchase_queue[app->queue_rear] = *request;
    app->queue_rear = (app->queue_rear + 1) % QUEUE_CAPACITY;
    app->queue_count++;
    return 1;
}

int queue_dequeue(AppState *app, PurchaseRequest *request) {
    if (app->queue_count == 0) {
        return 0;
    }
    *request = app->purchase_queue[app->queue_front];
    app->queue_front = (app->queue_front + 1) % QUEUE_CAPACITY;
    app->queue_count--;
    return 1;
}

void queue_display(const AppState *app) {
    int i;
    int index;

    if (app->queue_count == 0) {
        printf("Antrean pembelian masih kosong.\n");
        return;
    }
    printf("\n--- ANTREAN PEMBELIAN (FIFO) ---\n");
    for (i = 0; i < app->queue_count; i++) {
        const PurchaseRequest *request;
        index = (app->queue_front + i) % QUEUE_CAPACITY;
        request = &app->purchase_queue[index];
        printf("%d. Akun: %-15s | Paket: %d | Jumlah: %d\n",
               i + 1, request->username, request->package_id, request->quantity);
    }
}

int stack_push(AppState *app, const SaleRecord *record) {
    if (app->stack_top >= STACK_CAPACITY - 1) {
        return 0;
    }
    app->undo_stack[++app->stack_top] = *record;
    return 1;
}

int stack_peek(const AppState *app, SaleRecord *record) {
    if (app->stack_top < 0) {
        return 0;
    }
    *record = app->undo_stack[app->stack_top];
    return 1;
}

int stack_pop(AppState *app, SaleRecord *record) {
    if (app->stack_top < 0) {
        return 0;
    }
    *record = app->undo_stack[app->stack_top--];
    return 1;
}
