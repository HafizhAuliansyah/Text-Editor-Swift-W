#ifndef input_h
#define input_h
#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "row.h"

#define SWIFT_TAB_STOP 8
#define SWIFT_QUIT_TIMES 1
#define CTRL(k) ((k)&0x1f)

enum editorKey
{
    BACKSPACE = 127,
    ARROW_LEFT = 1000,
    ARROW_RIGHT,
    ARROW_UP,
    ARROW_DOWN,
    DEL_KEY,
    HOME_KEY,
    END_KEY,
    PAGE_UP,
    PAGE_DOWN,
    // Shift + Key
    SHIFT_ARROW_LEFT,
    SHIFT_ARROW_RIGHT,
    SHIFT_ARROW_UP,
    SHIFT_ARROW_DOWN,
};

struct teksEditor
{
    int numrows;
    address_row first_row;
};
#include "cursor.h"
#include "file_io.h"
#include "text_operation.h"
#include "output.h"

// membaca setiap press key di keyboard
int readKey();
void keyProcess();

void updateRow(erow *row);
/* ngatur untuk apa yang d render atau ditampilkan ke layar */

void insertRow(int at, address_column s, int len);
/* masukin ke array nya harusnya lewat sini tapi per row */

// void editorFreeRow(erow *row);
/* nge dealloc di memory sebesar erow*/

void deleteRow(int at);
/* Untuk menghapus row, jika row setelah ada isinya maka akan naik ke row yang dihapus*/

void rowInsertChar(erow *row, int at, int c);
/* menambahkan karakter to posisi cursor dalam row */

void rowAppendString(erow *row, address_column s, int len);
/* menambahkan isi chars ke sebuah row*/

void rowDelChar(erow *row, int at);
/* menghapus 1 karakter posisi cursor*/

/*** editor operations ***/
void insertChar(int c);
/* menambahkan 1 karakter*/

void deleteChar();
/* module untuk menghapus karakter */

void insertNewline();
/* handle enter*/

teksEditor getTeksEditor();
/* untuk mengambil teks editor */

void inputInit();
/* init kondisi awal input */

void columnInit(infotype *row);
/* init kondisi column awal */
#endif
