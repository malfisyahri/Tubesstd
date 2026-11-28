#ifndef MLL_H
#define MLL_H

#include <iostream>
#include <string>
#include <cctype>
using namespace std;

#define Nil NULL

const string GOL = "GOL", ASSIST = "ASSIST", KARTU_KUNING = "KARTU KUNING",
             KARTU_MERAH = "KARTU MERAH", GOL_BUNUH_DIRI = "GOL BUNUH DIRI";

struct infoKejadian { int menit; string jenis; };
typedef struct elmKejadian *adrKejadian;
struct elmKejadian { infoKejadian info; adrKejadian next; };
struct ListKejadian { adrKejadian first; };

struct infoPemain { int noPunggung; string nama, posisi; char tim; };
typedef struct elmPemain *adrPemain;
struct elmPemain { infoPemain info; ListKejadian lKejadian; adrPemain next; };
struct ListPemain { adrPemain first; };

struct infoPertandingan { string idMatch, timHome, timAway, stadion; };
typedef struct elmPertandingan *adrPertandingan;
struct elmPertandingan { infoPertandingan info; ListPemain lPemain; adrPertandingan next; };
struct ListPertandingan { adrPertandingan first; };

void createList(ListPertandingan &L);
bool isEmpty(ListPertandingan L);
adrPertandingan createParent(infoPertandingan data);
bool insertParent(ListPertandingan &L, adrPertandingan P);
void deleteParent(ListPertandingan &L, adrPertandingan P);
adrPertandingan findParent(ListPertandingan L, string idMatch);
void printSatuParent(adrPertandingan P);
void printParent(ListPertandingan L);
void printParentTim(ListPertandingan L, string tim);

adrPemain createChild(infoPemain data);
bool insertChild(adrPertandingan P, adrPemain C);
int deleteChild(adrPertandingan P, adrPemain C);
adrPemain findChild(adrPertandingan P, char tim, int noPunggung);
adrPemain findChildNama(adrPertandingan P, string nama);
void printChild(adrPertandingan P);

adrKejadian createKejadian(infoKejadian data);
void insertKejadian(adrPemain C, adrKejadian K);
int deleteKejadian(adrPertandingan P, adrPemain C, adrKejadian K);
adrKejadian findKejadian(adrPemain C, int menit, string jenis);
adrKejadian findKejadianKe(adrPemain C, int urutan);
void printKejadian(adrPemain C);

void printAll(ListPertandingan L);
void printRelasiParentChild(ListPertandingan L);
void printLaporanPertandingan(adrPertandingan P);
void printTopScorer(adrPertandingan P);
void printTimeline(adrPertandingan P);
void cariPemainSemuaPertandingan(ListPertandingan L, string nama, string tim, int noPunggung);
void hapusSemua(ListPertandingan &L);

string toLowerStr(string s);
string toUpperStr(string s);
string inputWajib(string prompt);
int inputInt(string prompt);
adrPertandingan pilihPertandingan(ListPertandingan L);
void isiDataDefault(ListPertandingan &L);
void tampilMenu();

void menuTambahPertandingan(ListPertandingan &L);
void menuCariPertandingan(ListPertandingan L);
void menuHapusPertandingan(ListPertandingan &L);
void menuTambahPemain(ListPertandingan &L);
void menuCariPemain(ListPertandingan L);
void menuHapusPemain(ListPertandingan &L);
void menuTambahKejadian(ListPertandingan &L);
void menuHapusKejadian(ListPertandingan &L);
void menuRelasiParentChild(ListPertandingan L);
void menuRekapPemain(ListPertandingan L);

#endif
