#ifndef TOPIK3_H
#define TOPIK3_H

#include "topik1.h"

void bubble_sort_packages(TicketPackage *items, size_t count, int criterion, int descending);
void selection_sort_packages(TicketPackage *items, size_t count, int criterion, int descending);
void sorting_menu(const AppState *app);

#endif
