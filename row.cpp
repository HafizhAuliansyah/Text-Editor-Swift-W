/* File        	: row.h */
/* Dibuat oleh 	: SWIFT GROUP */
/* Kelas		: 1B */

#include "row.h"

/*********** PROTOTYPE ****************/
/**** Predikat untuk test keadaan First  ****/
bool ListEmpty(address_row First)
{
    return (First == Nil);
}

/**** Konstruktor/Kreator address_row Kosong ****/
void CreateList(address_row *First)
{
    *First = Nil;
}

/**** Manajemen Memory ****/
address_row Alokasi(infotype X)
/* Mengirimkan address_row hasil alokasi sebuah elemen */
/* Jika alokasi berhasil, maka address_row != Nil, 	   */
/*	dan misalnya menghasilkan P, maka P->info = X, Next(P) = Nil */
/* Jika alokasi gagal, mengirimkan Nil */
{
    address_row P;
    P = (address_row)malloc(sizeof(ElmtRow));
    if (P != Nil)
    {
        P->info = X;
        Next(P) = Nil;
    }
    return P;
}

void DeAlokasi(address_row *P)
/* IS : P terdefinisi */
/* FS : P dikembalikan ke sistem */
/* Melakukan dealokasi / pengembalian address_row P ke system */
{
    Next(*P) = Nil;
    free(*P);
}

/**** Pencarian sebuah elemen List ****/
address_row Search(address_row First, infotype X)
/* Mencari apakah ada elemen list dengan P->info = X */
/* Jika ada, mengirimkan address_row elemen tsb. */
/* Jika tidak ada, mengirimkan Nil */
{
    // address_row P;
    // P = First;
    // bool found = false;
    // while (P != Nil && !found)
    // {
    //     if (P->info == X)
    //     {
    //         found = true;
    //     }
    //     else
    //     {
    //         P = Next(P);
    //     }
    // }
    // return P;
}

bool FSearch(address_row First, address_row P)
/* Mencari apakah ada elemen list yang beralamat P */
/* Mengirimkan true jika ada, false jika tidak ada */
{
    address_row Q = First;
    bool found = false;
    while (Q != Nil && !found)
    {
        if (Q == P)
        {
            found = true;
        }
        else
        {
            Q = Next(Q);
        }
    }
    return found;
}

// address_row SearchPrec(address_row First, infotype X)
/* Mengirimkan address_row elemen sebelum elemen yang nilainya = X */
/* Mencari apakah ada elemen list dengan P->info = X */
/* Jika ada, mengirimkan address_row Prec, dengan Next(Prec) = P dan P->info = X */
/* Jika tidak ada, mengirimkan Nil */
/* Jika P adalah elemen pertama, maka Prec = Nil */
/* Search dengan spesifikasi seperti ini menghindari */
/* traversal ulang jika setelah Search akan dilakukan operasi lain */
//{
// address_row P, Prec;
// P = First;
// if (Next(P) = Nil)
// {
//     /* list hanya berisi 1 elemen*/
//     Prec = Nil;
// }
// else
// {
//     Prec = P;
//     P = Next(P);
//     while (P != Nil && P->info != X)
//     {
//         Prec = P;
//         P = Next(P);
//     }
//     if (P->info == X)
//     {
//         return Prec;
//     }
//     else
//     {
//         return Nil;
//     }
// }
// }
/**** PRIMITIF BERDASARKAN NILAI ****/
/**** Penambahan Elemen ****/
void InsVFirst(address_row *First, infotype X)
/* IS : L mungkin Kosong */
/* FS : melakukan alokasi sebuah elemen dan */
/* menambahkan elemen pertama dengan nilai X jika alokasi berhasil */
{
    address_row P = Alokasi(X);
    if (P != Nil)
    {
        InsertFirst(&(*First), P);
    }
}

void InsVLast(address_row *First, infotype X)
/* IS : L mungkin Kosong */
/* FS : melakukan alokasi sebuah elemen dan */
/* menambahkan elemen list di akhir (elemen terakhir adalah yang baru) */
/* bernilai X jika alokasi berhasil. Jika alokasi gagal IS = FS */
{
    address_row P = Alokasi(X);
    if (P != Nil)
    {
        InsertLast(&(*First), P);
    }
}
/**** Penghapusan Elemen ****/

void DelVFirst(address_row *First, infotype *X)
/* IS : L TIDAK Kosong */
/* FS : Elemen pertama List dihapus, nilai info disimpan ke X */
/* 	dan alamat elemen pertama di dealokasi */
{
    address_row P;
    DelFirst(&(*First), &P); /* Hapus elemem pertama, blm didealokasi */
    *X = P->info;            /* info dari First disimpan di X       */
    DeAlokasi(&P);
}
void DelVLast(address_row *First, infotype *X)
/* IS : L TIDAK Kosong */
/* FS : Elemen terakhir list dihapus : nilai info disimpan pada X */
/* 	dan alamat elemen terakhir di dealokasi */
{
    address_row P;
    DelLast(&(*First), &P); /* Hapus elemem terakhir, blm didealokasi */
    *X = P->info;           /* info dari terakhir disimpan di X       */
    DeAlokasi(&P);
}
/**** PRIMITIF BERDASARKAN ALAMAT ****/
/**** Penambahan elemen berdasarkan alamat ****/
void InsertFirst(address_row *First, address_row P)
/* IS : L sembarang, P sudah dialokasi */
/* FS : menambahkan elemen ber-address_row P sebagai elemen pertama */
{
    Next(P) = *First;
    *First = P;
}

void InsertAfter(address_row *First, address_row P, address_row Prec)
/* IS : Prec pastilah elemen List dan bukan elemen terakhir */
/*	P sudah dialokasi */
/* FS : Insert P sebagai elemen sesudah elemen beralamat Prec */
{
    Next(P) = Next(Prec);
    Next(Prec) = P;
}
void InsertLast(address_row *First, address_row P)
/* IS : L sembarang, P sudah dialokasi */
/* FS : P ditambahkan sebagai elemen terakhir yang baru */
{
    if (ListEmpty(*First))
    {
        InsertFirst(&(*First), P);
    }
    else
    {
        address_row Last = *First;
        while (Next(Last) != Nil)
        {
            Last = Next(Last);
        }
        Next(Last) = P;
    }
}
/**** Penghapusan sebuah elemen ****/

void DelFirst(address_row *First, address_row *P)
/* IS : L TIDAK kosong */
/* FS : P adalah alamat elemen pertama list sebelum penghapusan */
/*	elemen list berkurang satu (mungkin menjadi kosong) */
/* First elemen yang baru adalah suksessor elemen pertama yang lama */
{
    *P = *First;
    *First = Next(*P);
    Next(*P) = Nil;
}

void DelP(address_row *First, infotype X)
/* IS : L sembarang */
/* FS : Jika ada elemen list beraddress_row P, dengan P->info = X */
/* 	Maka P dihapus dari list dan di dealokasi */
/* Jika tidak ada elemen list dengan P->info = X, maka list tetap */
/* List mungkin menjadi kosong karena penghapusan */
{
    // address_row P, Prec;
    // infotype Y;
    // /* Algoritma*/
    // P = *First;
    // if (P->info.chars == X.chars)
    // {
    //     DelVFirst(&(*First), &Y);
    // }
    // else
    // {
    //     Prec = Nil;
    //     while (Next(P) != Nil && P->info.chars != X.chars)
    //     {
    //         Prec = P;
    //         P = Next(P);
    //     }
    //     if (P->info.chars == X.chars)
    //     {
    //         Next(Prec) = Next(P);
    //         Next(P) = Nil;
    //         DeAlokasi(&P);
    //     }
    // }
}
void DelLast(address_row *First, address_row *P)
/* IS : L TIDAK kosong */
/* FS : P adalah alamat elemen terakhir list sebelum penghapusan */
/*	Elemen list berkurang satu (mungkin menjadi kosong) */
/* Last elemen baru adalah predesessor elemen terakhir yang lama, jika ada */
{
    address_row Last, PrecLast;

    Last = *First;
    /* dari catatan di kelas */
    if (Next(Last) == Nil)
    { /* hanya 1 elemen */
        DelFirst(&(*First), &(Last));
    }
    else
    { /* Lebih dr 1 elemen */
        PrecLast = Nil;
        while (Next(Last) != Nil)
        { /* Maju sampai elemen terakhir */
            PrecLast = Last;
            Last = Next(Last);
        } /* Next(Last)=Nil */
        *P = Last;
        Next(PrecLast) = Nil;
    }
}
void DelAfter(address_row *First, address_row *Pdel, address_row Prec)
/* IS : L TIDAK Kosong, Prec adalah anggota List */
/* FS : menghapus Next(Prec): Pdel adalah alamat elemen list yang dihapus */
{
    *Pdel = Next(Prec);
    Next(Prec) = Next(*Pdel);
    Next(*Pdel) = Nil;
}
/**** PROSES SEMUA ELEMEN LIST  ****/
void PrintInfo(address_row First)
/* IS : L mungkin kosong */
/* FS : Jika List tidak kosong, semua info yang disimpan pada elemen list */
/*	diprint. Jika list kosong, hanya menuliskan "List Kosong" */
{
    address_row P = First;
    if (P == Nil)
    {
        printf("List Kosong");
    }
    else
    {
        do
        {
            printf("%s  ", P->info.chars);
            P = Next(P);
        } while (P != Nil);
    }
}

/***************************************/
/*******  PROSES TERHADAP LIST  ********/
/***************************************/

void DelAll(address_row *First)
/* Delete semua elemen list dan alamat elemen di dealokasi */
{
    infotype X;
    while (!ListEmpty(*First))
    {
        DelVFirst(&(*First), &X);
    } /* kosong */
}

int NbElmt(address_row First)
{ /* Mengirimkan banyaknya elemen list, mengirimkan Nol jika kosong */
    address_row P;
    int NbEl = 0;
    if (ListEmpty(First))
    {
        return 0;
    }
    else
    { /* Tidak kosong */
        P = First;
        do
        {
            NbEl++;
            P = Next(P);
        } while (P != Nil);
    }
    return NbEl;
}

address_row searchByIndex(address_row First, int index)
{
    address_row P = First;
    int i = 0;
    while (P != Nil && i < index)
    {
        P = Next(P);
        i++;
    }
    return P;
}
