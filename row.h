/* File        	: row.h */
/* Dibuat oleh 	: SWIFT GROUP */
/* Kelas		: 1B */

#ifndef row_H
#define row_H
#include <stdio.h>
#include <stdlib.h>
#define Nil NULL
// #define Info(P) (&P)->info
#define Next(P) (P)->next
#define MAX_COLUMN 10
typedef struct erow
{
    int size;
    int rsize;
    char chars[MAX_COLUMN];
    char render[MAX_COLUMN];
} erow;

typedef erow infotype;
typedef struct tElmtRow *address_row;

typedef struct tElmtRow
{
    infotype info;
    address_row next;
} ElmtRow;

/*********** PROTOTYPE ****************/
/**** Predikat untuk test keadaan LIST  ****/
bool ListEmpty(address_row First);
/* Mengirim true jika List Kosong */

/**** Konstruktor/Kreator List Kosong ****/
void CreateRow(address_row *First);
/* IS : L sembarang */
/* FS : Terbentuk List Kosong */

/**** Manajemen Memory ****/
address_row Alokasi(infotype X);
/* Mengirimkan address_row hasil alokasi sebuah elemen */
/* Jika alokasi berhasil, maka address_row != Nil, 	   */
/*	dan misalnya menghasilkan P, maka Info(P) = X, Next(P) = Nil */
/* Jika alokasi gagal, mengirimkan Nil */

void DeAlokasi(address_row *P);
/* IS : P terdefinisi */
/* FS : P dikembalikan ke sistem */
/* Melakukan dealokasi / pengembalian address_row P ke system */

/**** PRIMITIF BERDASARKAN NILAI ****/
/**** Penambahan Elemen ****/
void InsVFirst(address_row *First, infotype X);
/* IS : L mungkin Kosong */
/* FS : melakukan alokasi sebuah elemen dan */
/* menambahkan elemen pertama dengan nilai X jika alokasi berhasil */

void InsVLast(address_row *First, infotype X);
/* IS : L mungkin Kosong */
/* FS : melakukan alokasi sebuah elemen dan */
/* menambahkan elemen list di akhir (elemen terakhir adalah yang baru) */
/* bernilai X jika alokasi berhasil. Jika alokasi gagal IS = FS */

/**** Penghapusan Elemen ****/
void DelVFirst(address_row *First, infotype *X);
/* IS : L TIDAK Kosong */
/* FS : Elemen pertama List dihapus, nilai info disimpan ke X */
/* 	dan alamat elemen pertama di dealokasi */

void DelVLast(address_row *First, infotype *X);
/* IS : L TIDAK Kosong */
/* FS : Elemen terakhir list dihapus : nilai info disimpan pada X */
/* 	dan alamat elemen terakhir di dealokasi */

/**** PRIMITIF BERDASARKAN ALAMAT ****/
/**** Penambahan elemen berdasarkan alamat ****/
void InsertFirst(address_row *First, address_row P);
/* IS : L sembarang, P sudah dialokasi */
/* FS : menambahkan elemen ber-address_row P sebagai elemen pertama */

void InsertAfter(address_row *First, address_row P, address_row Prec);
/* IS : Prec pastilah elemen List dan bukan elemen terakhir */
/*	P sudah dialokasi */
/* FS : Insert P sebagai elemen sesudah elemen beralamat Prec */

void InsertLast(address_row *First, address_row P);
/* IS : L sembarang, P sudah dialokasi */
/* FS : P ditambahkan sebagai elemen terakhir yang baru */

/**** Penghapusan sebuah elemen ****/
void DelFirst(address_row *First, address_row *P);
/* IS : L TIDAK kosong */
/* FS : P adalah alamat elemen pertama list sebelum penghapusan */
/*	elemen list berkurang satu (mungkin menjadi kosong) */
/* First elemen yang baru adalah suksessor elemen pertama yang lama */

void DelLast(address_row *First, address_row *P);
/* IS : L TIDAK kosong */
/* FS : P adalah alamat elemen terakhir list sebelum penghapusan */
/*	Elemen list berkurang satu (mungkin menjadi kosong) */
/* Last elemen baru adalah predesessor elemen terakhir yang lama, jika ada */

void DelAfter(address_row *First, address_row *Pdel, address_row Prec);
/* IS : L TIDAK Kosong, Prec adalah anggota List */
/* FS : menghapus Next(Prec): Pdel adalah alamat elemen list yang dihapus */

/***************************************/
/*******  PROSES TERHADAP LIST  ********/
/***************************************/

void DelAll(address_row *First);
/* Delete semua elemen list dan alamat elemen di dealokasi */

address_row searchByIndex(address_row First, int index);
/* mencari elemen berdasarkan index keberapa elemen tersebut */
#endif
