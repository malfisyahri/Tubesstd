#include "MLL.h"

string toLowerStr(string s) {
    for (size_t i = 0; i < s.length(); i++) s[i] = tolower((unsigned char)s[i]);
    return s;
}

string toUpperStr(string s) {
    for (size_t i = 0; i < s.length(); i++) s[i] = toupper((unsigned char)s[i]);
    return s;
}

string inputWajib(string prompt) {
    string s;
    while (true) {
        cout << prompt;
        if (!getline(cin, s)) return "";
        size_t a = s.find_first_not_of(" \t\r"), b = s.find_last_not_of(" \t\r");
        if (a != string::npos) return s.substr(a, b - a + 1);
        cout << "Input tidak boleh kosong." << endl;
    }
}

int inputInt(string prompt) {
    while (true) {
        string s = inputWajib(prompt);
        if (!cin) return -1;
        try {
            size_t idx = 0;
            int val = stoi(s, &idx);
            if (idx == s.length()) return val;
        } catch (...) {}
        cout << "Input harus berupa angka bulat." << endl;
    }
}

bool konfirmasi(string prompt) {
    return toUpperStr(inputWajib(prompt + " (y/n): ")) == "Y";
}

char inputTim() {
    while (true) {
        string s = toUpperStr(inputWajib("Tim (H = home / A = away): "));
        if (!cin) return 'H';
        if (s == "H" || s == "A") return s[0];
        cout << "Pilih H atau A." << endl;
    }
}

void createList(ListPertandingan &L) { L.first = Nil; }
bool isEmpty(ListPertandingan L) { return L.first == Nil; }
string namaTim(adrPertandingan P, char tim) { return (tim == 'H') ? P->info.timHome : P->info.timAway; }

int hitungJumlahParent(ListPertandingan L) {
    int n = 0;
    for (adrPertandingan P = L.first; P != Nil; P = P->next) n++;
    return n;
}

int hitungJumlahPemainTim(adrPertandingan P, char tim) {
    int n = 0;
    for (adrPemain C = (P ? P->lPemain.first : Nil); C != Nil; C = C->next)
        if (C->info.tim == tim) n++;
    return n;
}

int hitungJumlahPemain(adrPertandingan P) {
    return P ? hitungJumlahPemainTim(P, 'H') + hitungJumlahPemainTim(P, 'A') : 0;
}

int hitungJenisKejadian(adrPemain C, string jenis) {
    int n = 0;
    for (adrKejadian K = (C ? C->lKejadian.first : Nil); K != Nil; K = K->next)
        if (K->info.jenis == jenis) n++;
    return n;
}

void hitungSkor(adrPertandingan P, int &skorHome, int &skorAway) {
    skorHome = skorAway = 0;
    if (P == Nil) return;
    for (adrPemain C = P->lPemain.first; C != Nil; C = C->next) {
        int gol = hitungJenisKejadian(C, GOL), bd = hitungJenisKejadian(C, GOL_BUNUH_DIRI);
        if (C->info.tim == 'H') { skorHome += gol; skorAway += bd; }
        else { skorAway += gol; skorHome += bd; }
    }
}

void printSkorPertandingan(adrPertandingan P, string label) {
    int h, a;
    hitungSkor(P, h, a);
    cout << label << P->info.timHome << " " << h << " - " << a << " " << P->info.timAway << endl;
}

int menitKartuMerah(adrPemain C) {
    for (adrKejadian K = (C ? C->lKejadian.first : Nil); K != Nil; K = K->next)
        if (K->info.jenis == KARTU_MERAH) return K->info.menit;
    return -1;
}

int menitKeluar(adrPemain C) {
    if (C == Nil) return -1;
    int merah = menitKartuMerah(C), kuning = 0, k2 = -1;
    for (adrKejadian K = C->lKejadian.first; K != Nil; K = K->next) {
        if (K->info.jenis == KARTU_KUNING && ++kuning == 2) k2 = K->info.menit;
    }
    if (merah == -1) return k2;
    return (k2 == -1 || merah <= k2) ? merah : k2;
}

adrPertandingan createParent(infoPertandingan data) {
    adrPertandingan P = new elmPertandingan;
    P->info = data;
    P->lPemain.first = Nil;
    P->next = Nil;
    return P;
}

bool insertParent(ListPertandingan &L, adrPertandingan P) {
    if (findParent(L, P->info.idMatch) != Nil) {
        cout << "Gagal: ID " << P->info.idMatch << " sudah digunakan." << endl;
        return false;
    }
    if (toLowerStr(P->info.timHome) == toLowerStr(P->info.timAway)) {
        cout << "Gagal: tim home dan tim away tidak boleh sama." << endl;
        return false;
    }
    if (isEmpty(L)) L.first = P;
    else {
        adrPertandingan Q = L.first;
        while (Q->next != Nil) Q = Q->next;
        Q->next = P;
    }
    return true;
}

void dealokasiPemain(adrPemain C) {
    while (C->lKejadian.first != Nil) {
        adrKejadian h = C->lKejadian.first;
        C->lKejadian.first = h->next;
        delete h;
    }
    delete C;
}

void deleteParent(ListPertandingan &L, adrPertandingan P) {
    if (isEmpty(L) || P == Nil) return;
    if (P == L.first) L.first = P->next;
    else {
        adrPertandingan Prec = L.first;
        while (Prec != Nil && Prec->next != P) Prec = Prec->next;
        if (Prec == Nil) return;
        Prec->next = P->next;
    }
    while (P->lPemain.first != Nil) {
        adrPemain h = P->lPemain.first;
        P->lPemain.first = h->next;
        dealokasiPemain(h);
    }
    delete P;
}

adrPertandingan findParent(ListPertandingan L, string idMatch) {
    string key = toLowerStr(idMatch);
    for (adrPertandingan P = L.first; P != Nil; P = P->next)
        if (toLowerStr(P->info.idMatch) == key) return P;
    return Nil;
}

void printSatuParent(adrPertandingan P) {
    if (P == Nil) return;
    int h, a;
    hitungSkor(P, h, a);
    cout << "[" << P->info.idMatch << "] " << P->info.timHome << " " << h << " - " << a << " " << P->info.timAway
         << " | Stadion: " << P->info.stadion << " | Pemain: " << hitungJumlahPemain(P) << endl;
}

void printParent(ListPertandingan L) {
    if (isEmpty(L)) { cout << "Belum ada data pertandingan." << endl; return; }
    cout << "=== DAFTAR PERTANDINGAN (" << hitungJumlahParent(L) << ") ===" << endl;
    int no = 1;
    for (adrPertandingan P = L.first; P != Nil; P = P->next) {
        cout << no++ << ". "; printSatuParent(P);
    }
}

void printParentTim(ListPertandingan L, string tim) {
    string t = toLowerStr(tim);
    int no = 0;
    for (adrPertandingan P = L.first; P != Nil; P = P->next) {
        if (toLowerStr(P->info.timHome) == t || toLowerStr(P->info.timAway) == t) {
            cout << ++no << ". "; printSatuParent(P);
        }
    }
    if (no == 0) cout << "Tim " << tim << " tidak ditemukan di pertandingan manapun." << endl;
}

adrPemain createChild(infoPemain data) {
    adrPemain C = new elmPemain;
    C->info = data;
    C->lKejadian.first = Nil;
    C->next = Nil;
    return C;
}

bool insertChild(adrPertandingan P, adrPemain C) {
    if (P == Nil || findChild(P, C->info.tim, C->info.noPunggung) != Nil) {
        cout << "Gagal: nomor #" << C->info.noPunggung << " di tim tersebut sudah terdaftar." << endl;
        return false;
    }
    if (P->lPemain.first == Nil) P->lPemain.first = C;
    else {
        adrPemain Q = P->lPemain.first;
        while (Q->next != Nil) Q = Q->next;
        Q->next = C;
    }
    return true;
}

int hitungKejadianTimMenit(adrPertandingan P, char tim, int menit, string jenis) {
    int n = 0;
    for (adrPemain Q = P->lPemain.first; Q != Nil; Q = Q->next)
        if (Q->info.tim == tim)
            for (adrKejadian K = Q->lKejadian.first; K != Nil; K = K->next)
                if (K->info.jenis == jenis && K->info.menit == menit) n++;
    return n;
}

bool adaGolRekan(adrPertandingan P, adrPemain C, int menit) {
    for (adrPemain Q = P->lPemain.first; Q != Nil; Q = Q->next)
        if (Q != C && Q->info.tim == C->info.tim)
            for (adrKejadian K = Q->lKejadian.first; K != Nil; K = K->next)
                if (K->info.jenis == GOL && K->info.menit == menit) return true;
    return false;
}

void lepasKejadian(adrPemain C, adrKejadian K) {
    if (C->lKejadian.first == K) C->lKejadian.first = K->next;
    else {
        adrKejadian Prec = C->lKejadian.first;
        while (Prec != Nil && Prec->next != K) Prec = Prec->next;
        if (Prec != Nil) Prec->next = K->next;
    }
    K->next = Nil;
}

int rapikanAssist(adrPertandingan P) {
    int n = 0;
    for (adrPemain C = P->lPemain.first; C != Nil; C = C->next) {
        adrKejadian K = C->lKejadian.first;
        while (K != Nil) {
            adrKejadian nextK = K->next;
            if (K->info.jenis == ASSIST && !adaGolRekan(P, C, K->info.menit)) {
                lepasKejadian(C, K);
                delete K;
                n++;
            }
            K = nextK;
        }
    }
    for (adrPemain C = P->lPemain.first; C != Nil; C = C->next) {
        adrKejadian K = C->lKejadian.first;
        while (K != Nil) {
            adrKejadian nextK = K->next;
            if (K->info.jenis == ASSIST &&
                hitungKejadianTimMenit(P, C->info.tim, K->info.menit, ASSIST) >
                hitungKejadianTimMenit(P, C->info.tim, K->info.menit, GOL)) {
                lepasKejadian(C, K);
                delete K;
                n++;
            }
            K = nextK;
        }
    }
    return n;
}

int deleteChild(adrPertandingan P, adrPemain C) {
    if (P == Nil || C == Nil || P->lPemain.first == Nil) return -1;
    if (P->lPemain.first == C) P->lPemain.first = C->next;
    else {
        adrPemain Prec = P->lPemain.first;
        while (Prec != Nil && Prec->next != C) Prec = Prec->next;
        if (Prec == Nil) return -1;
        Prec->next = C->next;
    }
    dealokasiPemain(C);
    return rapikanAssist(P);
}

adrPemain findChild(adrPertandingan P, char tim, int noPunggung) {
    for (adrPemain C = (P ? P->lPemain.first : Nil); C != Nil; C = C->next)
        if (C->info.tim == tim && C->info.noPunggung == noPunggung) return C;
    return Nil;
}

adrPemain findChildNama(adrPertandingan P, string nama) {
    string n = toLowerStr(nama);
    for (adrPemain C = (P ? P->lPemain.first : Nil); C != Nil; C = C->next)
        if (toLowerStr(C->info.nama) == n) return C;
    return Nil;
}

void printChild(adrPertandingan P) {
    if (P == Nil) return;
    cout << "Pemain pada [" << P->info.idMatch << "] " << P->info.timHome << " vs " << P->info.timAway << ":" << endl;
    if (P->lPemain.first == Nil) { cout << "  (belum ada pemain)" << endl; return; }
    int no = 1;
    for (adrPemain C = P->lPemain.first; C != Nil; C = C->next) {
        cout << "  " << no++ << ". [" << C->info.tim << "] #" << C->info.noPunggung << " "
             << C->info.nama << " (" << C->info.posisi << ") - " << namaTim(P, C->info.tim)
             << " | G:" << hitungJenisKejadian(C, GOL) << " A:" << hitungJenisKejadian(C, ASSIST)
             << " K:" << hitungJenisKejadian(C, KARTU_KUNING) << " M:" << hitungJenisKejadian(C, KARTU_MERAH);
        if (menitKeluar(C) != -1) cout << " [KELUAR " << menitKeluar(C) << "']";
        cout << endl;
    }
}

adrKejadian createKejadian(infoKejadian data) {
    adrKejadian K = new elmKejadian;
    K->info = data;
    K->next = Nil;
    return K;
}

void insertKejadian(adrPemain C, adrKejadian K) {
    if (C == Nil || K == Nil) return;
    if (C->lKejadian.first == Nil || K->info.menit < C->lKejadian.first->info.menit) {
        K->next = C->lKejadian.first;
        C->lKejadian.first = K;
    } else {
        adrKejadian Prec = C->lKejadian.first;
        while (Prec->next != Nil && Prec->next->info.menit <= K->info.menit) Prec = Prec->next;
        K->next = Prec->next;
        Prec->next = K;
    }
}

int deleteKejadian(adrPertandingan P, adrPemain C, adrKejadian K) {
    if (P == Nil || C == Nil || K == Nil) return -1;
    lepasKejadian(C, K);
    delete K;
    return rapikanAssist(P);
}

adrKejadian findKejadian(adrPemain C, int menit, string jenis) {
    for (adrKejadian K = (C ? C->lKejadian.first : Nil); K != Nil; K = K->next)
        if (K->info.menit == menit && K->info.jenis == jenis) return K;
    return Nil;
}

adrKejadian findKejadianKe(adrPemain C, int urutan) {
    if (C == Nil || urutan < 1) return Nil;
    adrKejadian K = C->lKejadian.first;
    for (int i = 1; K != Nil && i < urutan; i++) K = K->next;
    return K;
}

void printKejadian(adrPemain C) {
    if (C == Nil || C->lKejadian.first == Nil) { cout << "     (belum ada kejadian)" << endl; return; }
    int no = 1, kuning = 0;
    for (adrKejadian K = C->lKejadian.first; K != Nil; K = K->next) {
        cout << "     " << no++ << ". " << K->info.menit << "' " << K->info.jenis;
        if (K->info.jenis == KARTU_KUNING && ++kuning == 2) cout << " (kedua, keluar)";
        cout << endl;
    }
}

bool tambahKejadian(adrPertandingan P, adrPemain C, infoKejadian d) {
    if (d.menit < 1 || d.menit > 120) { cout << "Ditolak: menit harus 1..120." << endl; return false; }
    if (findKejadian(C, d.menit, d.jenis) != Nil) { cout << "Ditolak: kejadian kembar di menit sama." << endl; return false; }
    int keluar = menitKeluar(C);
    if (keluar != -1 && d.menit > keluar) { cout << "Ditolak: pemain sudah keluar menit " << keluar << "'." << endl; return false; }
    if (d.jenis == KARTU_MERAH && hitungJenisKejadian(C, KARTU_MERAH) >= 1) { cout << "Ditolak: sudah kartu merah." << endl; return false; }
    if (d.jenis == KARTU_KUNING && hitungJenisKejadian(C, KARTU_KUNING) >= 2) { cout << "Ditolak: sudah 2 kartu kuning." << endl; return false; }
    if (d.jenis == ASSIST) {
        if (!adaGolRekan(P, C, d.menit)) { cout << "Ditolak: butuh gol rekan setim di menit yang sama." << endl; return false; }
        if (hitungKejadianTimMenit(P, C->info.tim, d.menit, ASSIST) >= hitungKejadianTimMenit(P, C->info.tim, d.menit, GOL)) {
            cout << "Ditolak: gol pada menit ini sudah memiliki assist." << endl; return false;
        }
    }
    adrKejadian baru = createKejadian(d);
    insertKejadian(C, baru);
    keluar = menitKeluar(C);
    if (keluar != -1) {
        for (adrKejadian K = C->lKejadian.first; K != Nil; K = K->next) {
            if (K->info.menit > keluar) {
                lepasKejadian(C, baru); delete baru;
                cout << "Ditolak: kartu ini bentrok dengan kejadian setelah menit keluar." << endl;
                return false;
            }
        }
    }
    cout << "Kejadian " << d.jenis << " " << d.menit << "' ditambahkan." << endl;
    printSkorPertandingan(P, "Skor saat ini: ");
    return true;
}

void printAll(ListPertandingan L) {
    if (isEmpty(L)) { cout << "Multi Linked List kosong." << endl; return; }
    for (adrPertandingan P = L.first; P != Nil; P = P->next) {
        cout << "==================================================" << endl;
        printSatuParent(P);
        if (P->lPemain.first == Nil) cout << "  (belum ada pemain)" << endl;
        for (adrPemain C = P->lPemain.first; C != Nil; C = C->next) {
            cout << "  [" << C->info.tim << "] #" << C->info.noPunggung << " " << C->info.nama
                 << " (" << C->info.posisi << ") - " << namaTim(P, C->info.tim) << endl;
            printKejadian(C);
        }
    }
    cout << "==================================================" << endl;
}

void printRelasiParentChild(ListPertandingan L) {
    if (isEmpty(L)) { cout << "Belum ada data pertandingan." << endl; return; }
    cout << "=== RELASI PARENT - CHILD (" << hitungJumlahParent(L) << " Pertandingan) ===" << endl;
    for (adrPertandingan P = L.first; P != Nil; P = P->next) {
        cout << "--------------------------------------------------" << endl;
        printSatuParent(P);
        printChild(P);
    }
}

void printLaporanPertandingan(adrPertandingan P) {
    if (P == Nil) return;
    int h, a; hitungSkor(P, h, a);
    cout << "=== LAPORAN [" << P->info.idMatch << "] ===" << endl;
    cout << P->info.timHome << " " << h << " - " << a << " " << P->info.timAway << " (" << P->info.stadion << ")" << endl;
    cout << "Pemain: " << hitungJumlahPemainTim(P, 'H') << " Home, " << hitungJumlahPemainTim(P, 'A') << " Away" << endl;
    for (adrPemain C = P->lPemain.first; C != Nil; C = C->next) {
        if (C->lKejadian.first != Nil) {
            cout << "  - " << C->info.nama << " (" << namaTim(P, C->info.tim) << "):" << endl;
            printKejadian(C);
        }
    }
}

void printTopScorer(adrPertandingan P) {
    if (P == Nil) return;
    int maks = 0;
    for (adrPemain C = P->lPemain.first; C != Nil; C = C->next)
        if (hitungJenisKejadian(C, GOL) > maks) maks = hitungJenisKejadian(C, GOL);
    if (maks == 0) { cout << "Belum ada gol di pertandingan ini." << endl; return; }
    cout << "Top scorer (" << maks << " gol):" << endl;
    for (adrPemain C = P->lPemain.first; C != Nil; C = C->next)
        if (hitungJenisKejadian(C, GOL) == maks)
            cout << "  - " << C->info.nama << " (#" << C->info.noPunggung << ", " << namaTim(P, C->info.tim) << ")" << endl;
}

void printTimeline(adrPertandingan P) {
    if (P == Nil) return;
    bool ada = false;
    for (int m = 1; m <= 120; m++) {
        for (adrPemain C = P->lPemain.first; C != Nil; C = C->next) {
            for (adrKejadian K = C->lKejadian.first; K != Nil; K = K->next) {
                if (K->info.menit == m) {
                    cout << "  " << m << "' " << K->info.jenis << " - " << C->info.nama
                         << " (#" << C->info.noPunggung << ", " << namaTim(P, C->info.tim) << ")" << endl;
                    ada = true;
                }
            }
        }
    }
    if (!ada) cout << "Belum ada kejadian di pertandingan ini." << endl;
}

void cariPemainSemuaPertandingan(ListPertandingan L, string nama, string tim, int noPunggung) {
    if (isEmpty(L)) { cout << "Belum ada data pertandingan." << endl; return; }
    int main = 0, gol = 0, assist = 0, kuning = 0, merah = 0;
    string tLow = toLowerStr(tim), nLow = toLowerStr(nama);
    for (adrPertandingan P = L.first; P != Nil; P = P->next) {
        char kt = (toLowerStr(P->info.timHome) == tLow) ? 'H' : (toLowerStr(P->info.timAway) == tLow ? 'A' : 0);
        adrPemain C = kt ? findChild(P, kt, noPunggung) : Nil;
        if (C != Nil && toLowerStr(C->info.nama) == nLow) {
            int g = hitungJenisKejadian(C, GOL), a = hitungJenisKejadian(C, ASSIST),
                k = hitungJenisKejadian(C, KARTU_KUNING), m = hitungJenisKejadian(C, KARTU_MERAH);
            cout << "  [" << P->info.idMatch << "] " << P->info.timHome << " vs " << P->info.timAway
                 << " | G:" << g << " A:" << a << " K:" << k << " M:" << m << endl;
            main++; gol += g; assist += a; kuning += k; merah += m;
        }
    }
    if (main == 0) cout << "Pemain tidak ditemukan di tim tersebut." << endl;
    else cout << "TOTAL: " << main << " match, " << gol << " gol, " << assist << " assist, "
              << kuning << " kuning, " << merah << " merah." << endl;
}

void hapusSemua(ListPertandingan &L) {
    while (!isEmpty(L)) deleteParent(L, L.first);
}

adrPertandingan pilihPertandingan(ListPertandingan L) {
    if (isEmpty(L)) { cout << "Belum ada data pertandingan." << endl; return Nil; }
    string id = toUpperStr(inputWajib("ID pertandingan: "));
    adrPertandingan P = findParent(L, id);
    if (P == Nil) cout << "Pertandingan " << id << " tidak ditemukan." << endl;
    return P;
}

adrPemain pilihPemain(adrPertandingan P, bool tampilkan) {
    if (P->lPemain.first == Nil) { cout << "Belum ada pemain." << endl; return Nil; }
    if (tampilkan) printChild(P);
    char tim = inputTim();
    int no = inputInt("No punggung: ");
    adrPemain C = findChild(P, tim, no);
    if (C == Nil) cout << "Pemain tidak ditemukan." << endl;
    return C;
}

void isiDataDefault(ListPertandingan &L) {
    struct DataM { const char *id, *h, *a, *s; } m[3] = {
        {"M01", "Manchester United", "Tottenham Hotspur", "Old Trafford"},
        {"M02", "Manchester City", "Liverpool", "Etihad Stadium"},
        {"M03", "Arsenal", "Chelsea", "Emirates Stadium"}
    };
    struct DataP { int idx; char tim; int no; const char *nama, *pos; } p[12] = {
        {0,'H',1,"Senne Lammens","GK"}, {0,'H',8,"Bruno Fernandes","MF"}, {0,'A',1,"Guglielmo Vicario","GK"}, {0,'A',19,"Dominic Solanke","FW"},
        {1,'H',9,"Erling Haaland","FW"}, {1,'H',16,"Rodri","MF"}, {1,'A',1,"Alisson Becker","GK"}, {1,'A',11,"Mohamed Salah","FW"},
        {2,'H',7,"Bukayo Saka","FW"}, {2,'H',41,"Declan Rice","MF"}, {2,'A',10,"Cole Palmer","MF"}, {2,'A',24,"Reece James","DF"}
    };
    adrPertandingan ptr[3];
    for (int i = 0; i < 3; i++) {
        ptr[i] = createParent({m[i].id, m[i].h, m[i].a, m[i].s});
        insertParent(L, ptr[i]);
    }
    for (int i = 0; i < 12; i++) insertChild(ptr[p[i].idx], createChild({p[i].no, p[i].nama, p[i].pos, p[i].tim}));
}

void menuTambahPertandingan(ListPertandingan &L) {
    infoPertandingan d;
    d.idMatch = toUpperStr(inputWajib("ID pertandingan: "));
    d.timHome = inputWajib("Tim home: ");
    d.timAway = inputWajib("Tim away: ");
    d.stadion = inputWajib("Stadion: ");
    adrPertandingan P = createParent(d);
    if (insertParent(L, P)) cout << "Pertandingan " << d.idMatch << " ditambahkan." << endl;
    else delete P;
}

void menuCariPertandingan(ListPertandingan L) {
    if (isEmpty(L)) { cout << "Belum ada data." << endl; return; }
    int p = inputInt("Cari by (1. ID / 2. Nama tim): ");
    if (p == 1) { adrPertandingan P = pilihPertandingan(L); if (P) printSatuParent(P); }
    else if (p == 2) printParentTim(L, inputWajib("Nama tim: "));
    else cout << "Pilihan tidak valid." << endl;
}

void menuHapusPertandingan(ListPertandingan &L) {
    adrPertandingan P = pilihPertandingan(L);
    if (!P) return;
    printSatuParent(P);
    if (konfirmasi("Hapus pertandingan ini beserta seluruh datanya?")) {
        deleteParent(L, P); cout << "Pertandingan dihapus." << endl;
    }
}

void menuTambahPemain(ListPertandingan &L) {
    adrPertandingan P = pilihPertandingan(L);
    if (!P) return;
    infoPemain d;
    d.tim = inputTim();
    d.noPunggung = inputInt("No punggung (1-99): ");
    if (d.noPunggung < 1 || d.noPunggung > 99) { cout << "Nomor harus 1..99." << endl; return; }
    d.nama = inputWajib("Nama pemain: ");
    d.posisi = toUpperStr(inputWajib("Posisi (GK/DF/MF/FW): "));
    if (d.posisi != "GK" && d.posisi != "DF" && d.posisi != "MF" && d.posisi != "FW") {
        cout << "Posisi tidak valid." << endl; return;
    }
    adrPemain C = createChild(d);
    if (insertChild(P, C)) cout << d.nama << " ditambahkan ke " << namaTim(P, d.tim) << "." << endl;
    else delete C;
}

void menuCariPemain(ListPertandingan L) {
    adrPertandingan P = pilihPertandingan(L);
    if (!P) return;
    if (P->lPemain.first == Nil) { cout << "Belum ada pemain." << endl; return; }
    int p = inputInt("Cari by (1. Tim & No / 2. Nama): ");
    adrPemain C = Nil;
    if (p == 1) {
        C = pilihPemain(P, false);
        if (!C) return;
    } else if (p == 2) {
        C = findChildNama(P, inputWajib("Nama: "));
        if (!C) { cout << "Pemain tidak ditemukan." << endl; return; }
    } else {
        cout << "Pilihan tidak valid." << endl;
        return;
    }
    cout << "[" << C->info.tim << "] #" << C->info.noPunggung << " " << C->info.nama << " (" << C->info.posisi << ")" << endl;
    printKejadian(C);
}

void menuHapusPemain(ListPertandingan &L) {
    adrPertandingan P = pilihPertandingan(L);
    if (!P) return;
    adrPemain C = pilihPemain(P, true);
    if (!C || !konfirmasi("Hapus " + C->info.nama + "?")) return;
    int ass = deleteChild(P, C);
    cout << "Pemain dihapus.";
    if (ass > 0) cout << " (" << ass << " assist ikut terhapus)";
    cout << endl;
    printSkorPertandingan(P, "Skor terbaru: ");
}

void menuTambahKejadian(ListPertandingan &L) {
    adrPertandingan P = pilihPertandingan(L);
    if (!P) return;
    adrPemain C = pilihPemain(P, true);
    if (!C) return;
    int menit = inputInt("Menit (1-120): ");
    cout << "1.Gol  2.Assist  3.Kartu Kuning  4.Kartu Merah  5.Gol Bunuh Diri" << endl;
    int j = inputInt("Jenis (1-5): ");
    string jenis[] = {GOL, ASSIST, KARTU_KUNING, KARTU_MERAH, GOL_BUNUH_DIRI};
    if (j < 1 || j > 5) { cout << "Pilihan tidak valid." << endl; return; }
    tambahKejadian(P, C, {menit, jenis[j - 1]});
}

void menuHapusKejadian(ListPertandingan &L) {
    adrPertandingan P = pilihPertandingan(L);
    if (!P) return;
    adrPemain C = pilihPemain(P, true);
    if (!C) return;
    printKejadian(C);
    if (C->lKejadian.first == Nil) return;
    adrKejadian K = findKejadianKe(C, inputInt("Nomor urut kejadian: "));
    if (!K) { cout << "Nomor urut tidak valid." << endl; return; }
    int ass = deleteKejadian(P, C, K);
    cout << "Kejadian dihapus.";
    if (ass > 0) cout << " (" << ass << " assist ikut terhapus)";
    cout << endl;
    printSkorPertandingan(P, "Skor terbaru: ");
}

void menuRelasiParentChild(ListPertandingan L) {
    int p = inputInt("Relasi Parent-Child (1. Semua / 2. Satu ID): ");
    if (p == 1) printRelasiParentChild(L);
    else if (p == 2) { adrPertandingan P = pilihPertandingan(L); if (P) { printSatuParent(P); printChild(P); } }
    else cout << "Pilihan tidak valid." << endl;
}

void menuRekapPemain(ListPertandingan L) {
    string nama = inputWajib("Nama pemain: "), tim = inputWajib("Nama tim: ");
    int no = inputInt("No punggung (1-99): ");
    if (no >= 1 && no <= 99) cariPemainSemuaPertandingan(L, nama, tim, no);
    else cout << "Nomor punggung tidak valid." << endl;
}

void tampilMenu() {
    cout << "\n+=== FOOTBALL MATCH CENTER (MLL) ===+"
         << "\n| 1. Tambah pertandingan            |"
         << "\n| 2. Daftar pertandingan (Parent)   |"
         << "\n| 3. Cari pertandingan              |"
         << "\n| 4. Hapus pertandingan             |"
         << "\n| 5. Tambah pemain (Child)          |"
         << "\n| 6. Daftar pemain pada match       |"
         << "\n| 7. Cari pemain                    |"
         << "\n| 8. Hapus pemain                   |"
         << "\n| 9. Tambah kejadian (Grandchild)   |"
         << "\n|10. Hapus kejadian                 |"
         << "\n|11. Show relasi Parent - Child     |"
         << "\n|12. Tampilkan semua (printAll)     |"
         << "\n|13. Laporan pertandingan           |"
         << "\n|14. Timeline pertandingan          |"
         << "\n|15. Top scorer pertandingan        |"
         << "\n|16. Rekap pemain semua match       |"
         << "\n| 0. Keluar                         |"
         << "\n+===================================+\n";
}
