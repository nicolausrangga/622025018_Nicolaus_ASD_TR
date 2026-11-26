#ifndef TOPIK5_H
#define TOPIK5_H

#include "topik1.h"

int sequential_search_transaction(const AppState *app, int transaction_id);
int binary_search_package_id(const TicketPackage *items, size_t count, int package_id);
void searching_menu(const AppState *app);

#endif
