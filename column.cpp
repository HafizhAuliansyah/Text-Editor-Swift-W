#include "column.h"
/*********** PROTOTYPE ****************/
/**** Predikat untuk test keadaan LIST  ****/
bool ColumnEmpty(address_column First)
/* Mengirim true jika List Kosong */
{
    return (First == Nil);
}
/**** Konstruktor/Kreator List Kosong ****/
void CreateColumn(address_column *First)
/* IS : L sembarang */
/* FS : Terbentuk List Kosong */
{
    *First = Nil;
}
/**** Manajemen Memory ****/
address_column AlokasiChar(columnInfotype X)
/* Mengirimkan address_column hasil alokasi sebuah elemen */
/* Jika alokasi berhasil, maka address_column != Nil, 	   */
/*	dan misalnya menghasilkan P, maka Info(P) = X, Next(P) = Nil */
/* Jika alokasi gagal, mengirimkan Nil */
{
    address_column P;
    P = (address_column)malloc(sizeof(ElmtColumn));
    if (P != Nil)
    {
        Info(P) = X;
        NextColumn(P) = Nil;
    }
    return P;
}
void DeAlokasiChar(address_column *P)
/* IS : P terdefinisi */
/* FS : P dikembalikan ke sistem */
/* Melakukan dealokasi / pengembalian address_column P ke system */
{
    NextColumn(*P) = Nil;
    free(*P);
}
/**** PRIMITIF BERDASARKAN NILAI ****/
/**** Penambahan Elemen ****/
void InsVFirstChar(address_column *First, columnInfotype X)
/* IS : L mungkin Kosong */
/* FS : melakukan alokasi sebuah elemen dan */
/* menambahkan elemen pertama dengan nilai X jika alokasi berhasil */
{
    address_column P = AlokasiChar(X);
    if (P != Nil)
    {
        InsertFirstChar(&(*First), P);
    }
}
void InsVLastChar(address_column *First, columnInfotype X)
/* IS : L mungkin Kosong */
/* FS : melakukan alokasi sebuah elemen dan */
/* menambahkan elemen list di akhir (elemen terakhir adalah yang baru) */
/* bernilai X jika alokasi berhasil. Jika alokasi gagal IS = FS */
{
    address_column P = AlokasiChar(X);
    if (P != Nil)
    {
        InsertLastChar(&(*First), P);
    }
}
/**** Penghapusan Elemen ****/
void DelVFirstChar(address_column *First, columnInfotype *X)
/* IS : L TIDAK Kosong */
/* FS : Elemen pertama List dihapus, nilai info disimpan ke X */
/* 	dan alamat elemen pertama di dealokasi */
{
    address_column P;
    DelFirstChar(&(*First), &P);
    *X = P->info;
    DeAlokasiChar(&P);
}
void DelVLastChar(address_column *First, columnInfotype *X)
/* IS : L TIDAK Kosong */
/* FS : Elemen terakhir list dihapus : nilai info disimpan pada X */
/* 	dan alamat elemen terakhir di dealokasi */
{
    address_column P;
    DelLastChar(&(*First), &P); /* Hapus elemem terakhir, blm didealokasi */
    *X = P->info;               /* info dari terakhir disimpan di X       */
    DeAlokasiChar(&P);
}
/**** PRIMITIF BERDASARKAN ALAMAT ****/
/**** Penambahan elemen berdasarkan alamat ****/
void InsertFirstChar(address_column *First, address_column P)
/* IS : L sembarang, P sudah dialokasi */
/* FS : menambahkan elemen ber-address_column P sebagai elemen pertama */
{
    NextColumn(P) = *First;
    *First = P;
}
void InsertAfterChar(address_column *First, address_column P, address_column Prec)
/* IS : Prec pastilah elemen List dan bukan elemen terakhir */
/*	P sudah dialokasi */
/* FS : Insert P sebagai elemen sesudah elemen beralamat Prec */
{
    NextColumn(P) = NextColumn(Prec);
    NextColumn(Prec) = P;
}
void InsertLastChar(address_column *First, address_column P)
/* IS : L sembarang, P sudah dialokasi */
/* FS : P ditambahkan sebagai elemen terakhir yang baru */
{
    if (ColumnEmpty(*First))
    {
        InsertFirstChar(&(*First), P);
    }
    else
    {
        address_column Last = *First;
        while (NextColumn(Last) != Nil)
        {
            Last = NextColumn(Last);
        }
        NextColumn(Last) = P;
    }
}
/**** Penghapusan sebuah elemen ****/
void DelFirstChar(address_column *First, address_column *P)
/* IS : L TIDAK kosong */
/* FS : P adalah alamat elemen pertama list sebelum penghapusan */
/*	elemen list berkurang satu (mungkin menjadi kosong) */
/* First elemen yang baru adalah suksessor elemen pertama yang lama */
{
    *P = *First;
    *First = NextColumn(*P);
    NextColumn(*P) = Nil;
}
void DelLastChar(address_column *First, address_column *P)
/* IS : L TIDAK kosong */
/* FS : P adalah alamat elemen terakhir list sebelum penghapusan */
/*	Elemen list berkurang satu (mungkin menjadi kosong) */
/* Last elemen baru adalah predesessor elemen terakhir yang lama, jika ada */
{
    address_column Last, PrecLast;

    Last = *First;
    /* dari catatan di kelas */
    if (NextColumn(Last) == Nil)
    { /* hanya 1 elemen */
        DelFirstChar(&(*First), &(Last));
    }
    else
    { /* Lebih dr 1 elemen */
        PrecLast = Nil;
        while (NextColumn(Last) != Nil)
        { /* Maju sampai elemen terakhir */
            PrecLast = Last;
            Last = NextColumn(Last);
        } /* Next(Last)=Nil */
        *P = Last;
        NextColumn(PrecLast) = Nil;
    }
}
void DelAfterChar(address_column *First, address_column *Pdel, address_column Prec)
/* IS : L TIDAK Kosong, Prec adalah anggota List */
/* FS : menghapus Next(Prec): Pdel adalah alamat elemen list yang dihapus */
{
    *Pdel = NextColumn(Prec);
    NextColumn(Prec) = NextColumn(*Pdel);
    NextColumn(*Pdel) = Nil;
}
/***************************************/
/*******  PROSES TERHADAP LIST  ********/
/***************************************/

void DelAllChar(address_column *First)
/* Delete semua elemen list dan alamat elemen di dealokasi */
{
    columnInfotype X;
    while (!ColumnEmpty(*First))
    {
        DelVFirstChar(&(*First), &X);
    }
}
address_column SearchCharByIndex(address_column First, int index)
/* mencari elemen berdasarkan index keberapa elemen tersebut */
{
    address_column P = First;
    int i = 0;
    while (P != Nil && i < index)
    {
        P = NextColumn(P);
        i++;
    }
    return P;
}