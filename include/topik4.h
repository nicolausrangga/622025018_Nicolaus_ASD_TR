#ifndef TOPIK4_H
#define TOPIK4_H

#include "topik1.h"

int queue_enqueue(AppState *app, const PurchaseRequest *request);
int queue_dequeue(AppState *app, PurchaseRequest *request);
void queue_display(const AppState *app);
int stack_push(AppState *app, const SaleRecord *record);
int stack_peek(const AppState *app, SaleRecord *record);
int stack_pop(AppState *app, SaleRecord *record);

#endif
