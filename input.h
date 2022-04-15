#ifndef input_h
#define input_h
#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_COLUMN 10
#define MAX_ROW 10
#define SWIFT_TAB_STOP 8

#include "cursor.h"

typedef struct erow{
	int size;
    int rsize;
    char chars[MAX_COLUMN + 1];
    char render[MAX_COLUMN + 1];
}erow;

struct teksEditor{
	int numrows;
	erow row[MAX_ROW + 1];
};

struct teksEditor teks_editor;


void editorUpdateRow(erow *row);
/* ngatur untuk apa yang d render atau ditampilkan ke layar */

void editorInsertRow(int at, char *s, size_t len);
/* masukin ke array nya harusnya lewat sini tapi per row */

// void editorFreeRow(erow *row);
/* nge dealloc di memory sebesar erow*/

void editorDelRow(int at);
/* Untuk menghapus row, jika row setelah ada isinya maka akan naik ke row yang dihapus*/

void editorRowInsertChar(erow *row, int at, int c);
/* menambahkan karakter to posisi cursor dalam row */

void editorRowAppendString(erow *row, char *s, size_t len);
/* menambahkan isi chars ke sebuah row*/

void editorRowDelChar(erow *row, int at);
/* menghapus 1 karakter posisi cursor*/

/*** editor operations ***/
void editorInsertChar(int c);
/* menambahkan 1 karakter*/

void editorDelChar();
/* module untuk menghapus karakter */

void editorInsertNewline();
/* handle enter*/

#endif
