#ifndef TOPIK2_H
#define TOPIK2_H

#include "topik1.h"

/* Pengelolaan data paket dan transaksi melalui file teks. */
int load_packages(AppState *app);
int save_packages(const AppState *app);
int load_transactions(AppState *app);
int save_transactions(const AppState *app);
int ensure_data_files(void);

/* Proses penjualan tiket dan riwayat transaksi pengguna. */
void show_packages(const AppState *app);
int submit_purchase_request(AppState *app);
void process_purchase_queue(AppState *app);
void undo_last_sale(AppState *app);
void show_user_transactions(const AppState *app);
int cancel_user_transaction(AppState *app, int transaction_id);

#endif
