#include "MLL.h"

int main() {
    ListPertandingan L;
    createList(L);
    isiDataDefault(L);

    int pilihan = -1;
    while (pilihan != 0) {
        tampilMenu();
        pilihan = inputInt("Pilih menu: ");
        if (!cin) break;
        cout << endl;

        switch (pilihan) {
            case 1:  menuTambahPertandingan(L); break;
            case 2:  printParent(L); break;
            case 3:  menuCariPertandingan(L); break;
            case 4:  menuHapusPertandingan(L); break;
            case 5:  menuTambahPemain(L); break;
            case 6:  printChild(pilihPertandingan(L)); break;
            case 7:  menuCariPemain(L); break;
            case 8:  menuHapusPemain(L); break;
            case 9:  menuTambahKejadian(L); break;
            case 10: menuHapusKejadian(L); break;
            case 11: menuRelasiParentChild(L); break;
            case 12: printAll(L); break;
            case 13: printLaporanPertandingan(pilihPertandingan(L)); break;
            case 14: printTimeline(pilihPertandingan(L)); break;
            case 15: printTopScorer(pilihPertandingan(L)); break;
            case 16: menuRekapPemain(L); break;
            case 0:  cout << "Terima kasih!" << endl; break;
            default: cout << "Menu tidak tersedia." << endl;
        }
    }

    hapusSemua(L);
    return 0;
}
