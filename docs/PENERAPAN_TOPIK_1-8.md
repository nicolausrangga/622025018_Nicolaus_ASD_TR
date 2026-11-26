# Pemetaan Penerapan Materi ASD Topik 1–8

| Topik | Penerapan dalam sistem | Berkas |
|---|---|---|
| 1. Refreshing C | Menu login dan menu pengguna, fungsi terpisah, `if/switch`, `for/while`, dan validasi input. | `src/main.c`, `include/topik1.h`, `src/topik1.c` |
| 2. Array, Pointer, File | Paket dan transaksi menggunakan `struct` serta array dinamis; pointer sebagai parameter; `malloc`, `realloc`, `free`; penyimpanan akun, paket, transaksi, graph pada `.txt`. | `include/topik1.h`, `include/topik2.h`, `src/topik2.c`, `data/*.txt` |
| 3. Sorting | Bubble Sort dan Selection Sort dengan pilihan ascending/descending untuk mengurutkan paket. | `include/topik3.h`, `src/topik3.c` |
| 4. Stack & Queue | Queue FIFO menyimpan permintaan pembelian; Stack LIFO menyimpan transaksi untuk fitur undo. | `include/topik4.h`, `src/topik4.c`, `src/topik2.c` |
| 5. Searching | Sequential Search mencari transaksi berdasarkan ID; Binary Search mencari ID paket setelah data salinan diurutkan. | `include/topik5.h`, `src/topik5.c` |
| 6. Hash Table | Fungsi hash buatan sendiri memetakan username ke indeks; collision diatasi dengan chaining linked list. | `include/topik6.h`, `src/topik6.c` |
| 7. Tree | BST untuk transaksi dengan operasi insert, search, delete, dan traversal preorder, inorder, postorder. | `include/topik7.h`, `src/topik7.c`, `src/topik2.c` |
| 8. Graph | Matriks adjacency memodelkan urutan relasi presale Reguler dan VIP; DFS dan BFS menelusuri tahap selanjutnya. | `include/topik8.h`, `src/topik8.c`, `data/zona.txt` |

## Arti relasi graph

Vertex mengikuti urutan baris data paket pada `data/paket.txt`. Relasi graph adalah Reguler P1 → Reguler P2 → Reguler P3, serta VIP P1 → VIP P2 → VIP P3. Graph ini menggambarkan relasi tahap presale, bukan rute lokasi konser.

## Catatan pengujian

Untuk pengujian awal, gunakan data akun dan transaksi kosong. Setelah program dijalankan, buat akun melalui menu Register. File paket menyimpan enam paket awal dengan kuota terjual nol.
