/* File        	: row.h */
/* Dibuat oleh 	: SWIFT GROUP */
/* Kelas		: 1B */

#ifndef column_H
#define column_H
#include <stdio.h>
#include <stdlib.h>
#define Nil NULL
#define Info(P) (P)->info
#define NextColumn(P) (P)->next

typedef char columnInfotype;
typedef struct tElmtColumn *address_column;

typedef struct tElmtColumn
{
    columnInfotype info;
    address_column next;
} ElmtColumn;

typedef struct erow
{
    int size;
    int rsize;
    address_column chars;
    address_column render;
} erow;

/*********** PROTOTYPE ****************/
/**** Predikat untuk test keadaan LIST  ****/
bool ColumnEmpty(address_column First);
/* Mengirim true jika List Kosong */

/**** Konstruktor/Kreator List Kosong ****/
void CreateColumn(address_column *First);
/* IS : L sembarang */
/* FS : Terbentuk List Kosong */

/**** Manajemen Memory ****/
address_column AlokasiChar(columnInfotype X);
/* Mengirimkan address_column hasil alokasi sebuah elemen */
/* Jika alokasi berhasil, maka address_column != Nil, 	   */
/*	dan misalnya menghasilkan P, maka Info(P) = X, Next(P) = Nil */
/* Jika alokasi gagal, mengirimkan Nil */

void DeAlokasiChar(address_column *P);
/* IS : P terdefinisi */
/* FS : P dikembalikan ke sistem */
/* Melakukan dealokasi / pengembalian address_column P ke system */

/**** PRIMITIF BERDASARKAN NILAI ****/
/**** Penambahan Elemen ****/
void InsVFirstChar(address_column *First, columnInfotype X);
/* IS : L mungkin Kosong */
/* FS : melakukan alokasi sebuah elemen dan */
/* menambahkan elemen pertama dengan nilai X jika alokasi berhasil */

void InsVLastChar(address_column *First, columnInfotype X);
/* IS : L mungkin Kosong */
/* FS : melakukan alokasi sebuah elemen dan */
/* menambahkan elemen list di akhir (elemen terakhir adalah yang baru) */
/* bernilai X jika alokasi berhasil. Jika alokasi gagal IS = FS */

/**** Penghapusan Elemen ****/
void DelVFirstChar(address_column *First, columnInfotype *X);
/* IS : L TIDAK Kosong */
/* FS : Elemen pertama List dihapus, nilai info disimpan ke X */
/* 	dan alamat elemen pertama di dealokasi */

void DelVLastChar(address_column *First, columnInfotype *X);
/* IS : L TIDAK Kosong */
/* FS : Elemen terakhir list dihapus : nilai info disimpan pada X */
/* 	dan alamat elemen terakhir di dealokasi */

/**** PRIMITIF BERDASARKAN ALAMAT ****/
/**** Penambahan elemen berdasarkan alamat ****/
void InsertFirstChar(address_column *First, address_column P);
/* IS : L sembarang, P sudah dialokasi */
/* FS : menambahkan elemen ber-address_column P sebagai elemen pertama */

void InsertAfterChar(address_column *First, address_column P, address_column Prec);
/* IS : Prec pastilah elemen List dan bukan elemen terakhir */
/*	P sudah dialokasi */
/* FS : Insert P sebagai elemen sesudah elemen beralamat Prec */

void InsertLastChar(address_column *First, address_column P);
/* IS : L sembarang, P sudah dialokasi */
/* FS : P ditambahkan sebagai elemen terakhir yang baru */

/**** Penghapusan sebuah elemen ****/
void DelFirstChar(address_column *First, address_column *P);
/* IS : L TIDAK kosong */
/* FS : P adalah alamat elemen pertama list sebelum penghapusan */
/*	elemen list berkurang satu (mungkin menjadi kosong) */
/* First elemen yang baru adalah suksessor elemen pertama yang lama */

void DelLastChar(address_column *First, address_column *P);
/* IS : L TIDAK kosong */
/* FS : P adalah alamat elemen terakhir list sebelum penghapusan */
/*	Elemen list berkurang satu (mungkin menjadi kosong) */
/* Last elemen baru adalah predesessor elemen terakhir yang lama, jika ada */

void DelAfterChar(address_column *First, address_column *Pdel, address_column Prec);
/* IS : L TIDAK Kosong, Prec adalah anggota List */
/* FS : menghapus Next(Prec): Pdel adalah alamat elemen list yang dihapus */

/***************************************/
/*******  PROSES TERHADAP LIST  ********/
/***************************************/

void DelAllChar(address_column *First);
/* Delete semua elemen list dan alamat elemen di dealokasi */

address_column SearchCharByIndex(address_column First, int index);
/* mencari elemen berdasarkan index keberapa elemen tersebut */
#endif
