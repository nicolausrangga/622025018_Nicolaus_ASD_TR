# Sistem Penjualan Tiket Konser dengan Manajemen Harga dan Kuota

## Identitas Mahasiswa
- **Nama:** Nicolaus Rangga S.
- **NIM:** 622025018

## Deskripsi Aplikasi
Aplikasi konsol berbasis Bahasa C untuk mengelola penjualan tiket konser dengan kategori Reguler dan VIP. Setiap kategori memiliki Presale 1, Presale 2, dan Presale 3. Semakin tinggi tahap presale, harga tiket semakin meningkat dan kuota tersedia semakin besar. Data akun, paket, dan transaksi disimpan dalam berkas teks.

## Daftar Fitur
- Register, login, dan logout akun.
- Menampilkan paket tiket berdasarkan kategori dan tahap presale.
- Pengelolaan harga dan kuota tiket.
- Antrean pembelian dan pencatatan transaksi.
- Riwayat transaksi serta fitur undo.
- Sorting ascending/descending dengan dua algoritma.
- Sequential Search dan Binary Search.
- Hash Table untuk pencarian akun.
- Binary Search Tree (BST) dengan insert, search, delete, dan traversal.
- Graph dengan traversal DFS dan BFS.

## Struktur folder

```text
Sistem_Penjualan_Tiket_Konser/
├── include/
│   ├── topik1.h
│   ├── topik2.h
│   ├── topik3.h
│   ├── topik4.h
│   ├── topik5.h
│   ├── topik6.h
│   ├── topik7.h
│   └── topik8.h
├── src/
│   ├── main.c
│   ├── topik1.c
│   ├── topik2.c
│   ├── topik3.c
│   ├── topik4.c
│   ├── topik5.c
│   ├── topik6.c
│   ├── topik7.c
│   └── topik8.c
├── data/
│   ├── akun.txt
│   ├── paket.txt
│   ├── transaksi.txt
│   └── zona.txt
├── docs/
├── .gitignore
└── README.md
```

`transaksi.txt` ditambahkan untuk menyimpan riwayat transaksi secara permanen, sedangkan tiga file `akun.txt`, `paket.txt`, dan `zona.txt` mengikuti penamaan pada contoh struktur.

## Penerapan Topik 1–8

| Topik | Penerapan | Berkas utama |
|---|---|---|
| 1. Refreshing C | Menu, fungsi, input tervalidasi, percabangan dan perulangan; definisi tipe data bersama | `topik1.h`, `topik1.c`, `main.c` |
| 2. Array, Pointer, File | `struct`, array dinamis, pointer, `malloc`/`realloc`/`free`, baca-tulis file, serta alur penjualan | `topik2.h`, `topik2.c` |
| 3. Sorting | Bubble Sort dan Selection Sort; ascending dan descending | `topik3.h`, `topik3.c` |
| 4. Stack & Queue | Stack LIFO untuk undo; Queue FIFO untuk antrean pembelian | `topik4.h`, `topik4.c` |
| 5. Searching | Sequential Search transaksi dan Binary Search paket | `topik5.h`, `topik5.c` |
| 6. Hash Table | Hash username dengan collision chaining untuk register/login | `topik6.h`, `topik6.c` |
| 7. Tree | BST insert, search, delete, preorder, inorder, postorder | `topik7.h`, `topik7.c` |
| 8. Graph | Graph relasi tahap presale dengan DFS dan BFS | `topik8.h`, `topik8.c`, `data/zona.txt` |

## Paket awal

| ID | Kategori | Presale | Harga | Kuota |
|---:|---|---:|---:|---:|
| 101 | Reguler | 1 | Rp150.000 | 50 |
| 102 | Reguler | 2 | Rp175.000 | 100 |
| 103 | Reguler | 3 | Rp200.000 | 150 |
| 201 | VIP | 1 | Rp300.000 | 20 |
| 202 | VIP | 2 | Rp350.000 | 40 |
| 203 | VIP | 3 | Rp400.000 | 60 |

## Kompilasi dan menjalankan program

Jalankan perintah berikut dari folder utama proyek dengan GCC:

```bash
gcc -std=c11 -Wall -Wextra -pedantic src/*.c -Iinclude -o app
```

Windows: `app.exe`  
Linux/macOS: `./app`

Jalankan program dari folder utama agar lokasi `data/*.txt` ditemukan. Jangan mengubah susunan kolom atau pemisah `|` dalam file database.

## Catatan

- Program menyediakan register, login, logout, antrean pembelian, proses transaksi, undo, riwayat, sorting, searching, BST, serta DFS/BFS.
- Antrean dan stack undo berlaku selama satu sesi. Paket dan transaksi tersimpan di file teks.
- Password disimpan sebagai teks biasa untuk kebutuhan praktikum; gunakan password contoh yang tidak dipakai pada layanan lain.
