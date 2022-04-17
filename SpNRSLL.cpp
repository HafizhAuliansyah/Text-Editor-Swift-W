/* File        	: main.cpp */
/* Dibuat oleh 	: Muhammad Rizki Halomoan (211511049) */
/*				  Fiqli Trias Ramadhan (211511036) */
/* Kelas		: 1B */
/* Tanggal     	: 6/3/2022 */

#include <stdio.h>
#include <stdlib.h>
#include "SpNRSLL.h"

/*********** PROTOTYPE ****************/
/**** Predikat untuk test keadaan LIST  ****/
bool ListEmpty (List L){
	return (First(L)==Nil);	
}

/**** Konstruktor/Kreator List Kosong ****/
void CreateList (List * L){
	First(*L) = Nil;
}


/**** Manajemen Memory ****/
address Alokasi (infotype X)
/* Mengirimkan address hasil alokasi sebuah elemen */
/* Jika alokasi berhasil, maka address != Nil, 	   */
/*	dan misalnya menghasilkan P, maka Info(P) = X, Next(P) = Nil */
/* Jika alokasi gagal, mengirimkan Nil */
{
	address P;
	P = (address)malloc(sizeof(ElmtList));
	if(P != Nil){
		Info(P) = X;
		Next(P) = Nil;
	}
	return P;
}

void DeAlokasi (address *P)
/* IS : P terdefinisi */
/* FS : P dikembalikan ke sistem */
/* Melakukan dealokasi / pengembalian address P ke system */
{
	Next(*P)= Nil;
	free(*P);
}
/**** Pencarian sebuah elemen List ****/
address Search (List L, infotype X)
/* Mencari apakah ada elemen list dengan Info(P) = X */
/* Jika ada, mengirimkan address elemen tsb. */
/* Jika tidak ada, mengirimkan Nil */
{
	address P;
	P = First(L);
	bool found = false;
	while(P != Nil && !found){
		if(Info(P) == X){
			found = true;	
		}else{
			P = Next(P);
		}
	}
	return P;
}

bool FSearch (List L, address P)
/* Mencari apakah ada elemen list yang beralamat P */
/* Mengirimkan true jika ada, false jika tidak ada */
{
	address Q = First(L);
	bool found = false;
	while(Q != Nil && !found){
		if(Q == P){
			found = true;	
		}else{
			Q = Next(Q);
		}
	}
	return found;
}

address SearchPrec (List L, infotype X)
/* Mengirimkan address elemen sebelum elemen yang nilainya = X */
/* Mencari apakah ada elemen list dengan Info(P) = X */
/* Jika ada, mengirimkan address Prec, dengan Next(Prec) = P dan Info(P) = X */
/* Jika tidak ada, mengirimkan Nil */
/* Jika P adalah elemen pertama, maka Prec = Nil */
/* Search dengan spesifikasi seperti ini menghindari */
/* traversal ulang jika setelah Search akan dilakukan operasi lain */
{
	address P, Prec;
	P = First(L);
	if(Next(P) = Nil){
		/* list hanya berisi 1 elemen*/
		Prec = Nil;
	}else{
		Prec = P;
		P = Next(P);
		while(P != Nil && Info(P) != X){
			Prec = P;
			P = Next(P);
		}
		if(Info(P) == X){
			return Prec;
		}else{
			return Nil;
		}
	}
}
/**** PRIMITIF BERDASARKAN NILAI ****/
/**** Penambahan Elemen ****/
void InsVFirst (List * L, infotype X)
/* IS : L mungkin Kosong */
/* FS : melakukan alokasi sebuah elemen dan */
/* menambahkan elemen pertama dengan nilai X jika alokasi berhasil */
{
	address P = Alokasi(X);
	if(P != Nil){
		InsertFirst(&(*L),P);
	}
}

void InsVLast (List * L, infotype X)
/* IS : L mungkin Kosong */
/* FS : melakukan alokasi sebuah elemen dan */
/* menambahkan elemen list di akhir (elemen terakhir adalah yang baru) */
/* bernilai X jika alokasi berhasil. Jika alokasi gagal IS = FS */
{
	address P = Alokasi(X);
	if(P != Nil){
		InsertLast(&(*L),P);
	}
}
/**** Penghapusan Elemen ****/
void DelVFirst (List * L, infotype * X)
/* IS : L TIDAK Kosong */
/* FS : Elemen pertama List dihapus, nilai info disimpan ke X */
/* 	dan alamat elemen pertama di dealokasi */
{
	address P;
	DelFirst(&(*L), &P); /* Hapus elemem pertama, blm didealokasi */
	*X = Info(P);		 /* info dari First disimpan di X       */
	DeAlokasi(&P);
}
void DelVLast (List * L, infotype * X)
/* IS : L TIDAK Kosong */
/* FS : Elemen terakhir list dihapus : nilai info disimpan pada X */
/* 	dan alamat elemen terakhir di dealokasi */
{
	address P;
	DelLast(&(*L), &P); /* Hapus elemem terakhir, blm didealokasi */
	*X = Info(P);		 /* info dari terakhir disimpan di X       */
	DeAlokasi(&P);
}
/**** PRIMITIF BERDASARKAN ALAMAT ****/
/**** Penambahan elemen berdasarkan alamat ****/
void InsertFirst (List * L, address P)
/* IS : L sembarang, P sudah dialokasi */
/* FS : menambahkan elemen ber-address P sebagai elemen pertama */
{
	Next(P) = First(*L);
	First(*L) = P;
}

void InsertAfter (List * L, address P, address Prec)
/* IS : Prec pastilah elemen List dan bukan elemen terakhir */
/*	P sudah dialokasi */
/* FS : Insert P sebagai elemen sesudah elemen beralamat Prec */
{
	Next(P) = Next(Prec);
	Next(Prec) = P;
	
}
void InsertLast (List * L, address P)
/* IS : L sembarang, P sudah dialokasi */
/* FS : P ditambahkan sebagai elemen terakhir yang baru */
{
	if(ListEmpty(*L)){
		InsertFirst(&(*L),P);
	}else{
		address Last = First(*L);
		while(Next(Last) != Nil){
			Last=Next(Last);
		}
		Next(Last) = P;
	}
}
/**** Penghapusan sebuah elemen ****/
void DelFirst (List * L, address * P)
/* IS : L TIDAK kosong */
/* FS : P adalah alamat elemen pertama list sebelum penghapusan */
/*	elemen list berkurang satu (mungkin menjadi kosong) */
/* First elemen yang baru adalah suksessor elemen pertama yang lama */
{
	*P = First(*L);
	First(*L) = Next(*P);
	Next(*P) = Nil;
}

void DelP (List * L, infotype X)
/* IS : L sembarang */
/* FS : Jika ada elemen list beraddress P, dengan Info(P) = X */
/* 	Maka P dihapus dari list dan di dealokasi */
/* Jika tidak ada elemen list dengan Info(P) = X, maka list tetap */
/* List mungkin menjadi kosong karena penghapusan */
{
	address P, Prec;
  	infotype Y;
  	/* Algoritma*/
  	P = First(*L);
  	if (Info(P) == X) {
    	DelVFirst(L,&Y);
  	} else {
    	Prec = Nil;
    	while (Next(P)!= Nil && Info(P) != X) {
        	Prec = P;
        	P = Next(P);
    	}
    	if (Info(P) == X){
        	Next(Prec) = Next(P);
        	Next(P) = Nil;
        	DeAlokasi(&P);
      	}
  	}
}
void DelLast (List * L, address * P)
/* IS : L TIDAK kosong */
/* FS : P adalah alamat elemen terakhir list sebelum penghapusan */
/*	Elemen list berkurang satu (mungkin menjadi kosong) */
/* Last elemen baru adalah predesessor elemen terakhir yang lama, jika ada */
{
	address Last, PrecLast;

	Last = First(*L);
	/* dari catatan di kelas */
	if (Next(Last) == Nil)
	{ /* hanya 1 elemen */
		DelFirst(&(*L), &(Last));
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
void DelAfter (List * L, address * Pdel, address Prec)
/* IS : L TIDAK Kosong, Prec adalah anggota List */
/* FS : menghapus Next(Prec): Pdel adalah alamat elemen list yang dihapus */
{
	*Pdel = Next(Prec);
	Next(Prec) = Next(*Pdel);
	Next(*Pdel) = Nil;
}
/**** PROSES SEMUA ELEMEN LIST  ****/
void PrintInfo (List L)
/* IS : L mungkin kosong */
/* FS : Jika List tidak kosong, semua info yang disimpan pada elemen list */
/*	diprint. Jika list kosong, hanya menuliskan "List Kosong" */
{
	address P = First(L);
	if(P == Nil){
		printf("List Kosong")	;
	}else{
		do{
			printf("%d  ",Info(P));
			P = Next(P);
		}while(P!=Nil);
	}
}

/***************************************/
/*******  PROSES TERHADAP LIST  ********/
/***************************************/

void DelAll (List * L)
/* Delete semua elemen list dan alamat elemen di dealokasi */
{
	infotype X;
	while (!ListEmpty(*L))
	{
		DelVFirst(&(*L), &X);
	} /* kosong */
}

int NbElmt(List L)
{ /* Mengirimkan banyaknya elemen list, mengirimkan Nol jika kosong */
	address P;
	int NbEl = 0;
	if (ListEmpty(L))
	{
		return 0;
	}
	else
	{ /* Tidak kosong */
		P = First(L);
		do
		{
			NbEl++;
			P = Next(P);
		} while (P != Nil);
	}
	return NbEl;
}
