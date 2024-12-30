#ifndef _BRWS_H
#define _BRWS_H

#include <stdio.h>
#include <stdlib.h>
#include <wctype.h>
#include <stdbool.h>
#include <stdint.h>

typedef struct browse BR;
typedef struct lines LN;
typedef struct columns CO;
typedef struct titlesbars TB;
typedef struct statusbars SB;
typedef struct keys KY;
typedef struct titlecols TC;

#define COLOR_BOX_BW        0
#define COLOR_FOCUS_BW      1
#define COLOR_TITLEBAR_BW   2
#define COLOR_STATUSBAR_BW  3
#define COLOR_SEARCHBAR_BW  4

#define BOX_MENU_BW    0
#define BOX_BROWSE_BW  1

#define HIDE_BW  0
#define SHOW_BW  1

#define FOCUS_OFF  0
#define FOCUS_ON   1

#define INS    0
#define DEL    1
#define F2     2
#define F3     3
#define F4     4
#define TAB    5
#define ENTER  6
#define SPC    7

#define DINT       0
#define DFLOAT     1
#define DCHAR      2
#define DWCHAR     3
#define DDATE      4
#define CHKBOX     5

#define COORD_LIN    0
#define COORD_COL    1
#define COORD_BLIN   2
#define COORD_RCOL   3
#define COORD_WINI   4
#define COORD_WFIN   5
#define COORD_WSIZE  6

BR *create_bw(); // cria menu
void free_bw( BR *br ); // destroi menu
void freelines_bw( BR *br );
void setcurline_bw( BR *br, LN *li );
void setctrlget_bw( BR *br, bool ctrlget );
void setctrlpercent_bw( BR *br, bool ctrlpercent );
void setcoord_bw( BR *br, uint8_t lin, uint8_t col, uint8_t winsize );
void setcolor_bw( BR *br, uint8_t colorpos, uint8_t colorc, uint8_t colorb, int numpars, ... );
void setbox_bw( BR *br, uint8_t typebox );
uint8_t setsearchbar_bw( BR *br, wchar_t *text );

LN *getini_bw( BR *br );
LN *getcur_bw( BR *br );
LN *getfin_bw( BR *br );
uint32_t getsize_bw( BR *br );
uint8_t getcoords_bw( BR *br, uint8_t coords );
void *getdatacol_bw( BR *br, uint32_t opt, uint16_t idcol );
void *getlinedatacol_bw( LN *l, uint16_t idcol );

void press_keys_bw( BR *br, uint8_t key, LN * ( *fu )( BR *, LN * ) );

void disptitlecol_bw( BR *br );

LN *addline_bw( BR *br, void *kdata, uint32_t opt, void ( *f ) () );
int rmline_bw( BR *br, uint32_t idline );
LN *searchline_bw( BR *br, uint32_t opt );
void setcheckline_bw( LN *l, bool check );
uint32_t getidline_bw( LN *l );
uint32_t getoptline_bw( LN *l );
bool getcheckline_bw( LN *l );
LN *moveline_bw( LN *l );

int cmpintline_bw( const void *a, const void *b );
int cmpdecreaseintline_bw( const void *a, const void *b );
int cmpcharline_bw( const void *a, const void *b );
int cmpwcharline_bw( const void *a, const void *b );
int cmpfloatline_bw( const void *a, const void *b );
int cmpdecreasefloatline_bw( const void *a, const void *b );
int cmpdateline_bw( const void *a, const void *b );
int cmpdecreasedateline_bw( const void *a, const void *b );

void setdatatypekey_bw( BR *br, size_t datatype );
void setfunckey_bw( BR *br, int ( *f ) ( const void *, const void * ) );

CO *addcol_bw( BR *br, LN *l, void *data, size_t datatype, void ( *f ) ( bool, BR *, LN *, CO * ) );
CO *addnewcol_bw( BR *br, LN *l, void *data, uint8_t idtype );
uint8_t rmcol_bw( LN *l, uint16_t idcol );
uint8_t getnumcols_bw( LN *l );
CO *getcols_bw( LN *l );
void freecols_bw( LN *l );
LN *searchcol_bw( BR *br, LN *l, uint16_t idcol, void *data, int ( *f )( const void *, const void * ) );

void setattrcol_bw( CO *c, uint16_t size, uint8_t precision, uint8_t align, bool upperc );
void setcolorcol_bw( CO *c, uint8_t colorc, uint8_t colorb, int numpars, ... );
void setshowcol_bw( CO *c, bool show );
void setshowdbarcol_bw( CO *c, bool showdbar );

void printdcharcol_bw( bool focus, BR *br, LN *l, CO *c );
void printdscharcol_bw( bool focus, BR *br, LN *l, CO *c );
void printdintcol_bw( bool focus, BR *br, LN *l, CO *c );
void printdfloatcol_bw( bool focus, BR *br, LN *l, CO *c );
void printddatecol_bw( bool focus, BR *br, LN *l, CO *c );
void printcheckboxcol_bw( bool focus, BR *br, LN *l, CO *c );

TC *addtitlecol_bw( BR *br, wchar_t *text );
void setattrtitlecol_bw( TC *tc, uint8_t align, bool upperc );
void setcolortitlecol_bw( TC *tc, uint8_t colorc, uint8_t colorb, int numpars, ... );

TB *addtitlebar_bw( BR *br, wchar_t *text );
uint8_t rmtitlebar_bw( BR *br, uint16_t id );
void setcolortitlebar_bw( TB *t, uint8_t colorc, uint8_t colorb, int numpars, ... );
uint8_t getsizetitlebar_bw( BR *br );
uint8_t getnumtitlebars_bw( BR *br );
void printtitlebar_bw( BR *br );

SB *addstatusbar_bw( BR *br, wchar_t *text );
uint8_t rmstatusbar_bw( BR *br, uint16_t id );
void setcolorstatusbar_bw( SB *sb, uint8_t colorc, uint8_t colorb, int numpars, ... );
uint8_t getsizestatusbar_bw( BR *br );
uint8_t getnumstatusbars_bw( BR *br );
void printstatusbar_bw( BR *br );

int cmpintsearch_bw( const void *a, const void *b );
int cmpfloatsearch_bw( const void *a, const void *b );
int cmpdatesearch_bw( const void *a, const void *b );
int cmpstrexactsearch_bw( const void *a, const void *b );
int cmpstrparcsearch_bw( const void *a, const void *b );
int cmpwstrexactsearch_bw( const void *a, const void *b );
int cmpwstrparcsearch_bw( const void *a, const void *b );

bool init_bw( BR *br );
int16_t exec_bw( BR *br );
void f_void();

#endif
