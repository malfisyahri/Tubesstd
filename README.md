# Football Match Center

Tugas Besar Struktur Data, kelompok ONE. Program C++ untuk mencatat pertandingan sepak bola dengan Multi Linked List model List to List.

## Anggota

| Nama | NIM |
| --- | --- |
| Muhamad Alfi Syahrin | 103032400057 |
| Muhammad Faiz Ghiffari | 103032400002 |
| Fahrio Hasnal Mubarik | 103032400038 |

## Struktur Data

Tiga tingkat, semuanya Singly Linked List. Setiap list hanya punya `first` dan setiap elemen hanya punya `next`.

| Tingkat | Entitas | Atribut |
| --- | --- | --- |
| Induk | Pertandingan | idMatch, timHome, timAway, stadion |
| Anak | Pemain | noPunggung, nama, posisi, tim (H/A) |
| Cucu | Kejadian | menit, jenis (gol, assist, kartu kuning, kartu merah, gol bunuh diri) |

Setiap pertandingan menyimpan list pemainnya, dan setiap pemain menyimpan list kejadiannya yang terurut menit. Skor tidak disimpan, tapi dihitung lewat traversal: gol menambah skor tim pemain, gol bunuh diri menambah skor tim lawan.

## File

| File | Isi |
| --- | --- |
| `MLL.h` | Struktur data dan prototipe |
| `MLL.cpp` | Realisasi operasi ADT |
| `main.cpp` | Menu program dan input |

## Cara Build

Dengan g++:

```
g++ -std=c++11 -o tubes main.cpp MLL.cpp
./tubes
```

Dengan Code Blocks: buat project Console application C++ kosong, lalu tambahkan ketiga file di atas ke project dan build. File `.cbp` tidak disertakan di repo.

## Menu

| No | Fitur |
| --- | --- |
| 1 | Tambah pertandingan |
| 2 | Tampilkan daftar pertandingan |
| 3 | Cari pertandingan (ID atau nama tim) |
| 4 | Hapus pertandingan beserta seluruh pemain dan kejadiannya |
| 5 | Tambah pemain |
| 6 | Tampilkan pemain pada pertandingan |
| 7 | Cari pemain (tim dan nomor punggung, atau nama) |
| 8 | Hapus pemain |
| 9 | Tambah kejadian |
| 10 | Hapus kejadian (berdasarkan nomor urut) |
| 11 | Show relasi Parent - Child (pertandingan dan pemainnya) |
| 12 | Tampilkan seluruh data (printAll) |
| 13 | Laporan pertandingan |
| 14 | Timeline pertandingan |
| 15 | Top scorer pertandingan |
| 16 | Rekap pemain di semua pertandingan |

Saat program dijalankan, 3 pertandingan awal (`M01`: Manchester United vs Tottenham Hotspur, `M02`: Manchester City vs Liverpool, `M03`: Arsenal vs Chelsea) beserta 12 pemain (2 per tim) otomatis dimuat tanpa kejadian.

## Aturan Validasi

- ID pertandingan harus unik, tim home dan away tidak boleh sama.
- Nomor punggung 1 sampai 99 dan unik per tim dalam satu pertandingan. Posisi hanya GK, DF, MF, FW.
- Menit kejadian 1 sampai 120. Kejadian yang sama pada menit yang sama untuk pemain yang sama ditolak.
- Assist hanya diterima kalau ada gol rekan satu tim di menit yang sama, dan setiap gol maksimal memiliki satu assist.
- Pemain maksimal 1 kartu merah dan 2 kartu kuning. Kartu kuning kedua otomatis membuat pemain keluar, setara kartu merah.
- Pemain yang sudah keluar tidak boleh punya kejadian setelah menit keluar. Kartu yang membuat pemain keluar sebelum kejadian yang sudah tercatat juga ditolak.
- Menghapus gol atau pemain otomatis menghapus assist yang tidak lagi didukung gol.
- Rekap lintas pertandingan mengenali pemain dari nama, nama tim, dan nomor punggung agar pemain bernama sama di tim yang sama tidak tergabung.
