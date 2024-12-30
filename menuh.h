#ifndef _MENUH_H
#define _MENUH_H

#include <stdio.h>
#include <stdlib.h>
#include <wctype.h>
#include <stdbool.h>
#include <stdint.h>

typedef struct menuh MNH;
typedef struct columns COL;
typedef struct title TIH;

#define COLOR_NOFOCUS_MH  0
#define COLOR_FOCUS_MH    1
#define COLOR_BACKGND_MH  2
#define COLOR_TITLE_MH    3
#define COLOR_HINT_MH     4

#define FOCUS_OFF     0
#define FOCUS_ON      1

MNH *create_mh(); // cria menu
void free_mh( MNH *mh ); // destroi menu
COL *getinicol_mh( MNH *mh );
COL *movecol_mh( COL *co );

TIH *addtitle_mh( MNH *mh, wchar_t *text );
void setcolortitle_mh( TIH *t, uint8_t colorc, uint8_t colorb, int numpars, ... );

void setcoord_mh( MNH *mh, uint8_t lin, uint8_t col, uint8_t optspc );
void setcolor_mh( MNH *mh, uint8_t colorpos, uint8_t colorc, uint8_t colorb, int numpars, ... );
uint8_t getsizecols_mh( MNH *mh );

COL *addcol_mh( MNH *mh, uint16_t opt, wchar_t *text, void (*f)() );
void setattrcol_mh( COL *co, uint16_t size, uint8_t align, bool upperc );
void setcolorcol_mh( COL *co, bool focus, uint8_t colorc, uint8_t colorb, int numpars, ... );
COL *setfocus_mh( MNH *mh, uint8_t opt );

void addhintcol_mh( COL *co, uint8_t lin, uint8_t col, wchar_t *text );
void setcolorhintcol_mh( COL *co, uint8_t colorc, uint8_t colorb, int numpars, ... );

bool init_mh( MNH *mh );
int16_t exec_mh( MNH *mh );
void showopt_mh( MNH *mh, COL *co, bool focus );
void clsbackgnd_mh( MNH *mh );
void hinton_mh( COL *co );
void hintoff_mh( COL *co );

#endif
