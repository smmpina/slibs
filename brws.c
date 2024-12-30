#include <stdint.h>
#include <time.h>
#include <wchar.h>
#include "colors.h"
#include "newlib.h"
#include "brws.h"

struct colors
{
    uint8_t colorc; // caractere
    uint8_t colorb; // fundo
    uint8_t colora[ 9 ]; // atributo
};

struct attributes
{
    uint16_t size;
    uint8_t align;
    uint8_t precision;
    bool upperc;
    struct colors color;
};

struct titlesbars
{
    uint16_t id;
    wchar_t text[ 128 ];
    struct colors color;
    struct titlesbars *next;
};

struct statusbars
{
    uint16_t id;
    wchar_t text[ 128 ];
    struct colors color;
    struct statusbars *next;
};

struct lines
{
    uint32_t id;
    uint32_t opt;
    uint8_t ctrlcols; // controle das colunas
    bool check;
    void ( *f ) ();
    void *kdata;
    struct columns *cols;
    struct lines *prev;
    struct lines *next;
};

struct columns
{
    uint16_t id;
    uint8_t idtype; // numero que representa tipo de dado
    size_t datatype;
    bool show; // mostra a coluna
    bool showdbar; // mostra a barra divisoria entre colunas
    void ( *f ) ( bool, struct browse *, struct lines *, struct columns * );
    void *data;
    struct attributes attr;
    struct columns *next;
};

struct titlecols
{
    uint16_t id;
    wchar_t text[ 128 ];
    struct attributes attr;
    struct titlecols *next;
};

struct browse
{
    bool init; // controle da inicialização do browse
    bool ctrlget; // ativa o browse ou apenas mostra
    bool ctrlpercent; // mostra ou não o percentual no box
    uint8_t typebox; // tipo de box ( BOX_BROWSE ou BOX_MENU )
    uint32_t totallines; // total de linhas do menu
    uint16_t ctrltitlebars; // controle numero de titlesbars
    uint16_t ctrlstatusbars; // controle numero de statusbars
    uint8_t ctrltitlecols; // controle numero de titlecols

    uint16_t sizecols[ 32 ];
    uint8_t numsizecols;
    
    uint8_t lin; // linha superior do menu
    uint8_t col; // coluna superior esquerda do menu
    uint8_t blin; // linha inferior do menu
    uint8_t rcol; // coluna inferior direita do menu
    
    uint8_t winsize; // numero de linhas da janela do menu 
    uint8_t winini; // primeira linha da janela
    uint8_t winfin; // ultima linha da janela
    
    uint8_t linesize; // largura da linha
    uint8_t titlesbarsize; // largura do titulo
    uint8_t statusbarsize; // largura do rodape

    size_t kdatatype;
    int ( *kf ) ( const void *, const void * );
    
    struct colors color[ 5 ];
    struct titlesbars *titlebar;
    struct statusbars *statusbar;
    struct titlecols *titlecol;
    
    struct lines * ( *fu_ins )( struct browse *, struct lines * ); // INS 
    struct lines * ( *fu_del )( struct browse *, struct lines * ); // DEL
    struct lines * ( *fu_f2 )( struct browse *, struct lines * ); // F2
    struct lines * ( *fu_f3 )( struct browse *, struct lines * ); // F3
    struct lines * ( *fu_f4 )( struct browse *, struct lines * ); // F4
    struct lines * ( *fu_tab )( struct browse *, struct lines * ); // TAB
    struct lines * ( *fu_enter )( struct browse *, struct lines * ); // ENTER
    struct lines * ( *fu_spc )( struct browse *, struct lines * ); // ESPAÇO

    struct lines *ini;
    struct lines *fin;
    struct lines *cur;
};

void showlinecols_bw( BR *br, LN *li, uint8_t lin, uint8_t col, bool focus );
void showdbar_bw( BR *br, uint8_t lin, uint8_t col );
LN *dispwin_bw( BR *br, LN *li );
void clsarea_bw( BR *br );

BR *create_bw()
{
    uint8_t i, ii;

    BR *br = ( BR * ) malloc( sizeof( BR ) );
    
    if( !br ) return NULL;

    br->init = false;
    br->ctrlget = true;
    br->ctrlpercent = true;
    br->typebox = BOX_BROWSE_BW;
    br->totallines = 0;
    br->ctrltitlebars = 0;
    br->ctrlstatusbars = 0;
    br->ctrltitlebars = 0;

    for( uint8_t inc = 0; inc < 32; inc++ ) br->sizecols[ inc ] = 0;
    br->numsizecols = 0;

    br->lin = 0;
    br->col = 0;
    br->blin = 0;
    br->rcol = 0;

    br->winsize = 0;
    br->winini = 0;
    br->winfin = 0;

    br->linesize = 0;
    br->titlesbarsize = 0;
    br->statusbarsize = 0;
    
    for( i = 0; i < 5; i++ )
    {
        for( ii = 0; ii < 9; ii++ )
        {
            br->color[ i ].colora[ ii ] = 0;
        }
    }
    
    br->color[ COLOR_BOX_BW ].colorc = NGRY;
    br->color[ COLOR_BOX_BW ].colorb = NBLK;
    br->color[ COLOR_BOX_BW ].colora[ 0 ] = NNORM;
    
    br->color[ COLOR_FOCUS_BW ].colorc = NWHT;
    br->color[ COLOR_FOCUS_BW ].colorb = 238;
    br->color[ COLOR_FOCUS_BW ].colora[ 0 ] = NNORM;

    br->color[ COLOR_TITLEBAR_BW ].colorc = NLYEL;
    br->color[ COLOR_TITLEBAR_BW ].colorb = NBLK;
    br->color[ COLOR_TITLEBAR_BW ].colora[ 0 ] = NBRIG;

    br->color[ COLOR_STATUSBAR_BW ].colorc = NGRN;
    br->color[ COLOR_STATUSBAR_BW ].colorb = NBLK;
    br->color[ COLOR_STATUSBAR_BW ].colora[ 0 ] = NITAL;

    br->color[ COLOR_SEARCHBAR_BW ].colorc = NGRY;
    br->color[ COLOR_SEARCHBAR_BW ].colorb = NBLK;
    br->color[ COLOR_SEARCHBAR_BW ].colora[ 0 ] = NBRIG;

    br->kdatatype = sizeof( int );
    br->kf = cmpintline_bw;

    br->ini = NULL;
    br->fin = NULL;
    br->cur = NULL;

    br->titlebar = NULL;
    br->statusbar = NULL;
    br->titlecol = NULL;

    br->fu_ins = NULL;
    br->fu_del = NULL;
    br->fu_f2 = NULL;
    br->fu_f3 = NULL;
    br->fu_f4 = NULL;
    br->fu_tab = NULL;
    br->fu_enter = NULL;
    br->fu_spc = NULL;

    return br;
}

void free_bw( BR *br )
{
    if( !br ) return;

    LN *li;
    CO *co;
    TB *tb;
    SB *sb;
    TC *tc;

    while( br->ini )
    {
        li = br->ini;

        br->ini = br->ini->next;

        while( li->cols )
        {
            co = li->cols;
            li->cols = li->cols->next;
            
            free( co->data );
            free( co );
        }

        free( li->kdata );
        free( li );
    }

    while( br->titlebar )
    {
        tb = br->titlebar;
        br->titlebar = br->titlebar->next;
           
        free( tb );
    }

    while( br->statusbar )
    {
        sb = br->statusbar;
        br->statusbar = br->statusbar->next;
           
        free( sb );
    }

    while( br->titlecol )
    {
        tc = br->titlecol;
        br->titlecol = br->titlecol->next;
           
        free( tc );
    }

    free( br );

    return;
}

void freelines_bw( BR *br )
{
    if( !br ) return;

    LN *li;
    CO *co;

    while( br->ini )
    {
        li = br->ini;

        br->ini = br->ini->next;

        while( li->cols )
        {
            co = li->cols;
            li->cols = li->cols->next;
            
            free( co->data );
            free( co );
        }

        free( li->kdata );
        free( li );
    }
    
    for( uint8_t inc = 0; inc < 32; inc++ ) br->sizecols[ inc ] = 0;
    br->numsizecols = 0;

    br->init = false;
    br->totallines = 0;
    br->winini = 0;
    br->winfin = 0;
    br->linesize = 0;

    br->ini = NULL;
    br->fin = NULL;
    br->cur = NULL;

    return;
}

// ------------------------------------------------------------------------------------------------
// Funcoes auxiliares que setam dados no menu  
// ------------------------------------------------------------------------------------------------

void setcurline_bw( BR *br, LN *li )
{
    if( !br ) return;

    br->cur = li;

    return;
}

void setctrlget_bw( BR *br, bool ctrlget )
{
    if( !br ) return;

    br->ctrlget = ctrlget;

    return;
}

void setctrlpercent_bw( BR *br, bool ctrlpercent )
{
    if( !br ) return;

    br->ctrlpercent = ctrlpercent;

    return;
}

void setcoord_bw( BR *br, uint8_t lin, uint8_t col, uint8_t winsize )
{
    if( !br ) return;
    
    br->lin = lin;
    br->col = col;
    br->winsize = winsize;
}

void setcolor_bw( BR *br, uint8_t colorpos, uint8_t colorc, uint8_t colorb, int numpars, ... )
{
    if( !br ) return;
    
    va_list lst;
    uint8_t inc;

    numpars = numpars < 0 ? 0 : numpars;
    numpars = numpars > 9 ? 9 : numpars;

    va_start( lst, numpars );

    br->color[ colorpos ].colorc = colorc;
    br->color[ colorpos ].colorb = colorb;

    for( inc = 0; inc < numpars; inc++ ) br->color[ colorpos ].colora[ inc ] = va_arg( lst, int );

    va_end( lst );

    return;
}

void setbox_bw( BR *br, uint8_t typebox )
{
    if( !br ) return;

    br->typebox = typebox;
}

uint8_t setsearchbar_bw( BR *br, wchar_t *text )
{
    if( !br ) return 0;
    if( br->typebox != BOX_BROWSE_BW ) return 0;
    
    uint8_t lin = br->blin - 2;
    uint8_t col = br->col + 2;
    size_t textsize = 0;
   
    setcolor_off();

    cleanarea( lin, col, lin, br->rcol - 1 );

    if( text )
    {
        textsize = wcslen( text );

        setcolor_aon( br->color[ COLOR_SEARCHBAR_BW ].colorc, 
                      br->color[ COLOR_SEARCHBAR_BW ].colorb,
                      br->color[ COLOR_SEARCHBAR_BW ].colora );

        setcurs( lin, col );

        if( textsize ) imp_wstr( text, textsize, J_ESQ, MINUSC );
    }

    setcolor_off();

    return lin;
}

// ------------------------------------------------------------------------------------------------
// Funcoes auxiliares que retornam dados do menu  
// ------------------------------------------------------------------------------------------------

LN *getini_bw( BR *br )
{
    if( !br ) return NULL;
    
    return br->ini;
}

LN *getcur_bw( BR *br )
{
    if( !br ) return NULL;

    return br->cur;
}

LN *getfin_bw( BR *br )
{
    if( !br ) return NULL;

    return br->fin;
}

uint32_t getsize_bw( BR *br )
{
    if( !br ) return 0;

    return br->totallines;
}

uint8_t getcoords_bw( BR *br, uint8_t coords )
{
    if( !br ) return 0;

    uint8_t ret = 0;

    switch( coords )
    {
        case COORD_LIN:
            ret = br->lin;
            break;
        case COORD_COL:
            ret =  br->col;
            break;
        case COORD_BLIN:
            ret = br->blin;
            break;
        case COORD_RCOL:
            ret = br->rcol;
            break;
        case COORD_WINI:
            ret = br->winini;
            break;
        case COORD_WFIN:
            ret = br->winfin;
            break;
        case COORD_WSIZE:
            ret = br->winsize;
            break;
    };

    return ret;
}

void *getdatacol_bw( BR *br, uint32_t opt, uint16_t idcol )
{
    if( !br ) return NULL;
    if( br->totallines < 1 ) return NULL;

    LN *aux = br->ini;

    while( aux && aux->opt != opt ) aux = aux->next;

    if( !aux ) return NULL;

    while( aux->cols && aux->cols->id != idcol ) aux->cols = aux->cols->next;

    if( !aux->cols ) return NULL;
    
    return aux->cols->data;
}

void *getlinedatacol_bw( LN *l, uint16_t idcol )
{
    if( !l ) return NULL;
    
    CO *aux = l->cols;

    while( aux && aux->id != idcol ) aux = aux->next;

    if( !aux ) return NULL;
    
    return aux->data;
}

// ------------------------------------------------------------------------------------------------
// Funcoes auxiliares que atribuem funçoes a determinadas teclas pressionadas  
// ------------------------------------------------------------------------------------------------

void press_keys_bw( BR *br, uint8_t key, LN * ( *fu )( BR *, LN * ) )
{
    if( !br ) return;
    
    switch( key )
    {
        case INS:
            if( fu ) br->fu_ins = fu;
            break;
        case DEL:
            if( fu ) br->fu_del = fu;
            break;
        case F2:
            if( fu ) br->fu_f2 = fu;
            break;
        case F3:
            if( fu ) br->fu_f3 = fu;
            break;
        case F4:
            if( fu ) br->fu_f4 = fu;
            break;
        case TAB:
            if( fu ) br->fu_tab = fu;
            break;
        case ENTER:
            if( fu ) br->fu_enter = fu;
            break;
        case SPC:
            if( fu ) br->fu_spc = fu;
            break;
    };

    return;
}

void disptitlecol_bw( BR *br )
{
    if( !br ) return;
    if( !br->ini ) return;
    if( !br->ini->cols ) return;
   
    uint8_t lin = br->lin + 2;
    uint8_t col = br->col + 2;
    CO *co = br->ini->cols;
    TC *tc = br->titlecol;

    setcolor_aon( br->color[ COLOR_BOX_BW ].colorc, 
                  br->color[ COLOR_BOX_BW ].colorb,
                  br->color[ COLOR_BOX_BW ].colora );

    cleanarea( lin, col, lin, br->rcol - 1 );

    for( uint8_t inc = 0; inc < br->numsizecols; inc++ )
    {
        if( !co )
        {
            col += br->sizecols[ inc ];

            col++;

            if( col == br->rcol ) break;

            setcolor_aon( br->color[ COLOR_BOX_BW ].colorc, 
                          br->color[ COLOR_BOX_BW ].colorb,
                          br->color[ COLOR_BOX_BW ].colora );

            setcurs( lin, col );
            printf( "%c%c%c", '\xe2', '\x94', '\x82' );

            setcurs( lin - 1, col );
            printf( "%c%c%c", '\xe2', '\x94', '\xac' );
             
            setcurs( lin + 1, col );
            printf( "%c%c%c", '\xe2', '\x94', '\xb4' );
        }
        else 
        {
            if( co->show )
            {
                if( tc ) setcolor_aon( tc->attr.color.colorc, tc->attr.color.colorb, tc->attr.color.colora );

                setcurs( lin, col );

                if( tc ) imp_wstr( tc->text, co->attr.size, tc->attr.align, tc->attr.upperc );

                col += br->sizecols[ inc ];

                /* if( !co->next ) break;    */

                col++;
            
                if( col == br->rcol ) break;
            
                setcolor_aon( br->color[ COLOR_BOX_BW ].colorc, 
                              br->color[ COLOR_BOX_BW ].colorb,
                              br->color[ COLOR_BOX_BW ].colora );

                setcurs( lin, col );
                printf( "%c%c%c", '\xe2', '\x94', '\x82' );

                setcurs( lin - 1, col );
                printf( "%c%c%c", '\xe2', '\x94', '\xac' );
             
                setcurs( lin + 1, col );
                printf( "%c%c%c", '\xe2', '\x94', co->showdbar ? '\xbc' : '\xb4' );

                col += 2;
            }
            
            co = co->next;
            tc = tc ? tc->next : NULL;
        }
    }

    setcolor_off();
    
    return;
}

// ------------------------------------------------------------------------------------------------
// Funçoes para as linhas do menu
// ------------------------------------------------------------------------------------------------

LN *addline_bw( BR *br, void *kdata, uint32_t opt, void ( *f ) () )
{
    if( !br ) return NULL;
    if( br->kdatatype == 0 ) return NULL;
    if( !br->kf ) return NULL;

    LN *l = ( LN * ) malloc( sizeof( LN ) );

    if( !l ) return NULL;

    if( kdata == NULL )
    {
        br->kdatatype = sizeof( int );
        if( br->kf == NULL ) br->kf = cmpintline_bw;
        kdata = ( int * ) &opt;
    }

    l->kdata = malloc( br->kdatatype );

    if( !l->kdata )
    {
        free( l );
        return NULL;
    }

    memcpy( l->kdata, kdata, br->kdatatype );

    l->f = f;
    l->ctrlcols = 0;
    l->check = false;
    l->cols = NULL;

    LN **pr = &br->ini;
    LN *aux = NULL;

    while( *pr && br->kf( l->kdata, ( *pr )->kdata ) )
    {
        aux = *pr;
        pr = &( *pr )->next;
    }

    if( *pr )
        ( *pr )->prev = l;
    else
        br->fin = l;

    l->next = *pr;
    l->prev = aux;
    *pr = l;

    br->totallines++;
    l->id = br->totallines;
    l->opt = opt;
    
    return l;
}

int rmline_bw( BR *br, uint32_t idline )
{
    if( !br ) return 0;

    LN **pr = &br->ini;
    LN *aux;
    CO *co;

    while( *pr && ( *pr )->id != idline ) pr = &( *pr )->next;

    aux = *pr;

    if( !aux ) return 0;

    if( aux->next )
        aux->next->prev = aux->prev;
    else
        br->fin = aux->prev;

    *pr = aux->next;

    br->totallines--;

    while( aux->cols )
    {
        co = aux->cols;
        aux->cols = aux->cols->next;
            
        free( co->data );
        free( co );
    }

    free( aux->kdata );
    free( aux );

    return 1;
}

LN *searchline_bw( BR *br, uint32_t opt )
{
    if( !br ) return NULL;

    LN *pr = br->ini;

    while( pr && pr->opt != opt ) pr = pr->next;

    if( pr ) return pr;
    
    return NULL;
}

void setcheckline_bw( LN *l, bool check )
{
    if( !l ) return;

    l->check = check;

    return;
}

uint32_t getidline_bw( LN *l )
{
    if( !l ) return 0;

    return l->id;
}

uint32_t getoptline_bw( LN *l )
{
    if( !l ) return 0;

    return l->opt;
}

bool getcheckline_bw( LN *l )
{
    if( !l ) return 0;

    return l->check;
}

LN *moveline_bw( LN *l )
{
    if( !l ) return NULL;

    return l->next;
}

// ------------------------------------------------------------------------------------------------
// Funçoes auxiliares para chave de tipos de dados na linha do menu
// ------------------------------------------------------------------------------------------------

int cmpintline_bw( const void *a, const void *b )
{
    return *( int * )a > *( int * )b;
}

int cmpdecreaseintline_bw( const void *a, const void *b )
{
    return *( int * )b > *( int * )a;
}

int cmpcharline_bw( const void *a, const void *b )
{
    return strcoll( ( char * )a, ( char * )b ) > 0; 
}

int cmpwcharline_bw( const void *a, const void *b )
{
    return wcscoll( ( wchar_t * )a, ( wchar_t * )b ) > 0;
}

int cmpfloatline_bw( const void *a, const void *b )
{
    return *( double * )a > *( double * )b;
}

int cmpdecreasefloatline_bw( const void *a, const void *b )
{
    return *( double * )b > *( double * )a;
}

int cmpdateline_bw( const void *a, const void *b )
{
    return *( time_t * )a > *( time_t * )b;
}

int cmpdecreasedateline_bw( const void *a, const void *b )
{
    return *( time_t * )b > *( time_t * )a;
}

// ------------------------------------------------------------------------------------------------
// Funcoes para a chave de ordenacao das linhas
// ------------------------------------------------------------------------------------------------

void setdatatypekey_bw( BR *br, size_t datatype )
{
    if( !br ) return;
    if( datatype == 0 ) return;

    br->kdatatype = datatype;
}

void setfunckey_bw( BR *br, int ( *f ) ( const void *, const void * ) )
{
    if( !br ) return;
    if( f == NULL ) return;

    br->kf = f;
}

// ------------------------------------------------------------------------------------------------
// Funçoes para as colunas do menu
// ------------------------------------------------------------------------------------------------

CO *addcol_bw( BR *br, LN *l, void *data, size_t datatype, void ( *f ) ( bool, BR *, LN *, CO * ) )
{
    if( !br ) return NULL;
    if( !l ) return NULL;
    if( !f ) return NULL;

    CO *c = ( CO * ) malloc( sizeof( CO ) );

    if( !c ) return NULL;

    c->f = f;

    c->datatype = datatype;

    c->data = malloc( c->datatype );
    
    if( !c->data )
    {
        free( c );
        return NULL;
    }

    memcpy( c->data, data, datatype );

    c->id = l->ctrlcols++;
    c->show = true;
    c->showdbar = true;

    c->attr.size = 5;
    c->attr.precision = 0;
    c->attr.align = J_ESQ;
    c->attr.upperc = false;
    
    c->attr.color.colorc = NWHT;
    c->attr.color.colorb = NBLK;

    for( uint8_t i = 0; i < 9; i++ ) c->attr.color.colora[ i ] = 0;

    c->attr.color.colora[ 0 ] = NBRIG;

    CO **pr = &l->cols;
    CO *aux = NULL;

    while( *pr ) pr = &( *pr )->next;

    aux = *pr;
    c->next = aux;
    *pr = c;

    return c;
}

CO *addnewcol_bw( BR *br, LN *l, void *data, uint8_t idtype )
{
    if( !br ) return NULL;
    if( !l ) return NULL;

    CO *c = ( CO * ) malloc( sizeof( CO ) );
    
    if( !c ) return NULL;
    
    char dcchar[ 6 ];
    char dchar[ 128 ];
    wchar_t dwchar[ 128 ];

    size_t datatype = sizeof( dcchar );

    if( data == NULL ) data = ( char * ) dcchar;

    switch( idtype )
    {
        case DINT:
            datatype = sizeof( int );
            c->f = printdintcol_bw;
            break;
        case DFLOAT:
            datatype = sizeof( double );
            c->f = printdfloatcol_bw;
            break;
        case DCHAR:
            datatype = sizeof( dchar );
            c->f = printdscharcol_bw;
            break;
        case DWCHAR:
            datatype = sizeof( dwchar );
            c->f = printdcharcol_bw;
            break;
        case DDATE:
            datatype = sizeof( time_t );
            c->f = printddatecol_bw;
            break;
        case CHKBOX:
            c->f = printcheckboxcol_bw;
            break;
    };

    c->data = malloc( datatype );
    
    if( !c->data )
    {
        free( c );
        return NULL;
    }

    memcpy( c->data, data, datatype );

    c->datatype = datatype;
    c->idtype = idtype;
    c->id = l->ctrlcols++;
    c->show = true;
    c->showdbar = true;

    c->attr.size = 5;
    c->attr.precision = 0;
    c->attr.align = J_ESQ;
    c->attr.upperc = false;
    
    c->attr.color.colorc = NWHT;
    c->attr.color.colorb = NBLK;

    for( uint8_t i = 0; i < 9; i++ ) c->attr.color.colora[ i ] = 0;

    c->attr.color.colora[ 0 ] = NBRIG;

    CO **pr = &l->cols;
    CO *aux = NULL;

    while( *pr ) pr = &( *pr )->next;

    aux = *pr;
    c->next = aux;
    *pr = c;

    return c;
}

uint8_t rmcol_bw( LN *l, uint16_t idcol )
{
    if( !l ) return 0;
    if( l->ctrlcols == 0 ) return 0;

    CO **pr = &l->cols;
    CO *aux = NULL;

    while( *pr && ( *pr )->id != idcol ) pr = &( *pr )->next;

    if( !*pr ) return 0;

    aux = *pr;
    *pr = aux->next;

    l->ctrlcols--;

    free( aux->data );
    free( aux );

    return 1;
}

uint8_t getnumcols_bw( LN *l )
{
    if( !l ) return 0;

    return l->ctrlcols;
}

CO *getcols_bw( LN *l )
{
    if( !l ) return NULL;

    return l->cols;
}

void freecols_bw( LN *l )
{
    if( !l ) return;

    CO *co;

    while( l->cols )
    {
        co = l->cols;
        l->cols = l->cols->next;

        free( co->data );
        free( co );
    }

    l->ctrlcols = 0;

    return;
}

LN *searchcol_bw( BR *br, LN *l, uint16_t idcol, void *data, int ( *f )( const void *, const void * ) )
{
    if( !br ) return NULL;
    if( !l ) return NULL;

    LN *li = l;
    CO *co = NULL;

    while( li )
    {
        co = li->cols;

        while( li->cols && li->cols->id != idcol ) li->cols = li->cols->next;

        if( li->cols && f( li->cols->data, data ) )
        {
            li->cols = co;
            break;
        }

        li->cols = co;

        li = li->next;
    }

    return li;
}

// ------------------------------------------------------------------------------------------------
// Funcoes auxiliares para setar dados na coluna 
// ------------------------------------------------------------------------------------------------

void setattrcol_bw( CO *c, uint16_t size, uint8_t precision, uint8_t align, bool upperc )
{
    if( !c ) return;

    c->attr.size = size;
    c->attr.precision = precision;
    c->attr.align = align;
    c->attr.upperc = upperc;
}

void setcolorcol_bw( CO *c, uint8_t colorc, uint8_t colorb, int numpars, ... )
{
    if( !c ) return;
    
    va_list l;
    uint8_t inc;

    numpars = numpars < 0 ? 0 : numpars;
    numpars = numpars > 9 ? 9 : numpars;

    va_start( l, numpars );

    c->attr.color.colorc = colorc;
    c->attr.color.colorb = colorb;

    for( inc = 0; inc < numpars; inc++ ) c->attr.color.colora[ inc ] = va_arg( l, int );

    va_end( l );

    return;
}

void setshowcol_bw( CO *c, bool show )
{
    if( !c ) return;

    c->show = show;
}

void setshowdbarcol_bw( CO *c, bool showdbar )
{
    if( !c ) return;

    c->showdbar = showdbar;
}

// ------------------------------------------------------------------------------------------------
// Funcoes auxiliares para imprimir dados da coluna de acordo com o tipo 
// ------------------------------------------------------------------------------------------------

void printdcharcol_bw( bool focus, BR *br, LN *l, CO *c )
{
    if( focus )
        setcolor_aon( br->color[ COLOR_FOCUS_BW ].colorc,
                      br->color[ COLOR_FOCUS_BW ].colorb,
                      br->color[ COLOR_FOCUS_BW ].colora );
    else
        setcolor_aon( c->attr.color.colorc, c->attr.color.colorb, c->attr.color.colora );

    imp_wstr( ( wchar_t * )c->data, c->attr.size, c->attr.align, c->attr.upperc );
}

void printdscharcol_bw( bool focus, BR *br, LN *l, CO *c )
{
    if( focus )
        setcolor_aon( br->color[ COLOR_FOCUS_BW ].colorc,
                 br->color[ COLOR_FOCUS_BW ].colorb,
                 br->color[ COLOR_FOCUS_BW ].colora );
    else
        setcolor_aon( c->attr.color.colorc, c->attr.color.colorb, c->attr.color.colora );

    imp_str( ( char * )c->data, c->attr.size, c->attr.align, c->attr.upperc );
}

void printdintcol_bw( bool focus, BR *br, LN *l, CO *c )
{
    if( focus )
        setcolor_aon( br->color[ COLOR_FOCUS_BW ].colorc,
                 br->color[ COLOR_FOCUS_BW ].colorb,
                 br->color[ COLOR_FOCUS_BW ].colora );
    else
        setcolor_aon( c->attr.color.colorc, c->attr.color.colorb, c->attr.color.colora );
    
    imp_wint( *( int * )c->data, c->attr.size, c->attr.precision, c->attr.align );
}

void printdfloatcol_bw( bool focus, BR *br, LN *l, CO *c )
{
    if( focus )
        setcolor_aon( br->color[ COLOR_FOCUS_BW ].colorc,
                 br->color[ COLOR_FOCUS_BW ].colorb,
                 br->color[ COLOR_FOCUS_BW ].colora );
    else
        setcolor_aon( c->attr.color.colorc, c->attr.color.colorb, c->attr.color.colora );

    imp_wdoub( fabs( *( double * )c->data ), c->attr.size, c->attr.precision, c->attr.align );
}

void printddatecol_bw( bool focus, BR *br, LN *l, CO *c )
{
    struct tm *t;
    wchar_t buffer[ 64 ];
    uint8_t colorweek;
    time_t segs = *( time_t * )c->data;

    if( focus )
    {
        setcolor_aon( br->color[ COLOR_FOCUS_BW ].colorc,
                 br->color[ COLOR_FOCUS_BW ].colorb,
                 br->color[ COLOR_FOCUS_BW ].colora );

        imp_wstr( dweek_to_wstr( segs, buffer ), 3, J_DIR, c->attr.upperc );
        imp_wstr( L", ", 2, J_ESQ, MINUSC );
        imp_wstr( date_to_wstr( segs, buffer ), c->attr.size - 5, c->attr.align, MINUSC );
    }
    else
    {
        t = localtime( &segs );

        if( t->tm_wday == 0 ) colorweek = NRED;
        else if( t->tm_wday == 6 ) colorweek = NYEL;
        else colorweek = NLBLU;

        setcolor_aon( colorweek, c->attr.color.colorb, c->attr.color.colora );

        imp_wstr( dweek_to_wstr( segs, buffer ), 3, J_DIR, c->attr.upperc );

        setcolor_aon( c->attr.color.colorc, c->attr.color.colorb, c->attr.color.colora );
        
        imp_wstr( L", ", 2, J_ESQ, MINUSC );

        imp_wstr( date_to_wstr( segs, buffer ), c->attr.size - 5, c->attr.align, MINUSC );
    }

    return;
}

void printcheckboxcol_bw( bool focus, BR *br, LN *l, CO *c )
{
    if( focus )
        setcolor_aon( br->color[ COLOR_FOCUS_BW ].colorc,
                 br->color[ COLOR_FOCUS_BW ].colorb,
                 br->color[ COLOR_FOCUS_BW ].colora );
    else
        setcolor_aon( c->attr.color.colorc, c->attr.color.colorb, c->attr.color.colora );

    if( l->check )
        printf( "[\xe2\x97\x86]" );
    else
        printf( "[ ]" );

    return;
}

// ------------------------------------------------------------------------------------------------
// Funcoes auxiliares para setar dados do titulo da coluna 
// ------------------------------------------------------------------------------------------------

TC *addtitlecol_bw( BR *br, wchar_t *text )
{
    if( !br ) return NULL;

    TC *tc = ( TC * ) malloc( sizeof( TC ) );

    if( !tc ) return NULL;

    tc->attr.color.colorc = NGRY;
    tc->attr.color.colorb = NBLK;

    for( int i = 0; i < 9; i++ ) tc->attr.color.colora[ i ] = 0;

    tc->attr.color.colora[ 0 ] = NBRIG;

    tc->attr.size = 0;
    tc->attr.precision = 0;

    tc->attr.align = J_ESQ;
    tc->attr.upperc = MINUSC;

    tc->id = br->ctrltitlecols++;

    wcscpy( tc->text, text );

    TC **pr = &br->titlecol;
    TC *aux = NULL;

    while( *pr ) pr = &( *pr )->next;

    aux = *pr;
    tc->next = aux;
    *pr = tc;

    return tc;
}

void setattrtitlecol_bw( TC *tc, uint8_t align, bool upperc )
{
    if( !tc ) return;

    tc->attr.align = align;
    tc->attr.upperc = upperc;
}

void setcolortitlecol_bw( TC *tc, uint8_t colorc, uint8_t colorb, int numpars, ... )
{
    va_list l;
    uint8_t inc;

    if( !tc ) return;

    numpars = numpars < 0 ? 0 : numpars;
    numpars = numpars > 9 ? 9 : numpars;

    va_start( l, numpars );

    tc->attr.color.colorc = colorc;
    tc->attr.color.colorb = colorb;

    for( inc = 0; inc < numpars; inc++ ) tc->attr.color.colora[ inc ] = va_arg( l, int );

    va_end( l );

    return;
}

// ------------------------------------------------------------------------------------------------
// Funcoes para o titulo do menu 
// ------------------------------------------------------------------------------------------------

TB *addtitlebar_bw( BR *br, wchar_t *text )
{
    if( !br ) return NULL;

    TB *t = ( TB * ) malloc( sizeof( TB ) );

    if( !t ) return NULL;

    t->color.colorc = br->color[ COLOR_TITLEBAR_BW ].colorc;
    t->color.colorb = br->color[ COLOR_TITLEBAR_BW ].colorb;

    for( int i = 0; i < 9; i++ ) t->color.colora[ i ] = br->color[ COLOR_TITLEBAR_BW ].colora[ i ];

    wcscpy( t->text, text );
    
    t->id = br->ctrltitlebars++;

    br->titlesbarsize += wcslen( t->text );

    TB **pr = &br->titlebar;
    TB *aux = NULL;

    while( *pr ) pr = &( *pr )->next;

    aux = *pr;
    t->next = aux;
    *pr = t;

    return t;
}

uint8_t rmtitlebar_bw( BR *br, uint16_t id )
{
    if( !br ) return 0;

    TB **tb = &br->titlebar;
    TB *aux = NULL;

    while( *tb && ( *tb )->id != id ) tb = &( *tb )->next;

    if( !*tb ) return 0;

    aux = *tb;
    *tb = aux->next;

    br->titlesbarsize -= wcslen( aux->text );
    br->ctrltitlebars--;

    free( aux );

    return 1;
}

void setcolortitlebar_bw( TB *t, uint8_t colorc, uint8_t colorb, int numpars, ... )
{
    if( !t ) return;
    
    va_list l;
    uint8_t inc;

    numpars = numpars < 0 ? 0 : numpars;
    numpars = numpars > 9 ? 9 : numpars;

    va_start( l, numpars );

    t->color.colorc = colorc;
    t->color.colorb = colorb;

    for( inc = 0; inc < numpars; inc++ ) t->color.colora[ inc ] = va_arg( l, int );

    va_end( l );

    return;
}

uint8_t getsizetitlebar_bw( BR *br )
{
    if( !br ) return 0;

    return br->titlesbarsize;
}

uint8_t getnumtitlebars_bw( BR *br )
{
    if( !br ) return 0;

    return br->ctrltitlebars;
}

void printtitlebar_bw( BR *br )
{
    if( !br ) return;

    size_t sizetext;
    
    TB *aux = br->titlebar;

    while( aux )
    {
        setcolor_aon(  aux->color.colorc, aux->color.colorb, aux->color.colora );

        sizetext = wcslen( aux->text );
        
        imp_wstr( aux->text, sizetext, J_ESQ, MINUSC );
        
        aux = aux->next;
    }
}

// ------------------------------------------------------------------------------------------------
// Funcoes para o rodape do menu 
// ------------------------------------------------------------------------------------------------

SB *addstatusbar_bw( BR *br, wchar_t *text )
{
    if( !br ) return NULL;

    SB *sb = ( SB * ) malloc( sizeof( SB ) );

    if( !sb ) return NULL;

    sb->color.colorc = br->color[ COLOR_STATUSBAR_BW ].colorc;
    sb->color.colorb = br->color[ COLOR_STATUSBAR_BW ].colorb;

    for( int i = 0; i < 9; i++ ) sb->color.colora[ i ] = br->color[ COLOR_STATUSBAR_BW ].colora[ i ];

    wcscpy( sb->text, text );
    
    sb->id = br->ctrlstatusbars++;

    br->statusbarsize += wcslen( sb->text );
    
    SB **pr = &br->statusbar;
    SB *aux = NULL;

    while( *pr ) pr = &( *pr )->next;

    aux = *pr;
    sb->next = aux;
    *pr = sb;


    return sb;
}

uint8_t rmstatusbar_bw( BR *br, uint16_t id )
{
    if( !br ) return 0;

    SB **sb = &br->statusbar;
    SB *aux = NULL;

    while( *sb && ( *sb )->id != id ) sb = &( *sb )->next;

    if( !*sb ) return 0;

    aux = *sb;
    *sb = aux->next;

    br->statusbarsize -= wcslen( aux->text );
    br->ctrlstatusbars--;

    free( aux );

    return 1;
}

void setcolorstatusbar_bw( SB *sb, uint8_t colorc, uint8_t colorb, int numpars, ... )
{
    va_list l;
    uint8_t inc;

    if( !sb ) return;

    numpars = numpars < 0 ? 0 : numpars;
    numpars = numpars > 9 ? 9 : numpars;

    va_start( l, numpars );

    sb->color.colorc = colorc;
    sb->color.colorb = colorb;

    for( inc = 0; inc < numpars; inc++ ) sb->color.colora[ inc ] = va_arg( l, int );

    va_end( l );

    return;
}

uint8_t getsizestatusbar_bw( BR *br )
{
    if( !br ) return 0;

    return br->statusbarsize;
}

uint8_t getnumstatusbars_bw( BR *br )
{
    if( !br ) return 0;

    return br->ctrlstatusbars;
}

void printstatusbar_bw( BR *br )
{
    if( !br ) return;
    
    size_t sizetext;
    
    SB *aux = br->statusbar;

    while( aux )
    {
        setcolor_aon(  aux->color.colorc, aux->color.colorb, aux->color.colora );
        
        sizetext = wcslen( aux->text );
        
        imp_wstr( aux->text, sizetext, J_ESQ, MINUSC );

        aux = aux->next;
    }
}

// ------------------------------------------------------------------------------------------------
// Funcoes auxiliares para consulta de linha e coluna no menu
// ------------------------------------------------------------------------------------------------

int cmpintsearch_bw( const void *a, const void *b )
{
    return *( int * )a == *( int * )b;
}

int cmpfloatsearch_bw( const void *a, const void *b )
{
    return *( double * )a == *( double * )b;
}

int cmpdatesearch_bw( const void *a, const void *b )
{
    return *( time_t * )a == *( time_t * )b;
}

int cmpstrexactsearch_bw( const void *a, const void *b )
{
    return ( strcmp( ( char * )a, ( char * )b ) == 0 ); 
}

int cmpstrparcsearch_bw( const void *a, const void *b )
{
    return ( strstr( ( char * )a, ( char * )b ) != NULL ); 
}

int cmpwstrexactsearch_bw( const void *a, const void *b )
{
    return ( wcscmp( ( wchar_t * )a, ( wchar_t * )b ) == 0 ); 
}

int cmpwstrparcsearch_bw( const void *a, const void *b )
{
    return ( wcsstr( ( wchar_t * )a, ( wchar_t * )b ) != NULL ); 
}

bool init_bw( BR *br )
{
    if( !br ) return false;
    if( !br->ini ) return false;
    if( br->totallines == 0 ) return false;

    br->winsize = br->winsize < 1 ? 1 : br->winsize;
    
    LN *li = br->ini;
    CO *co;

    uint16_t sum = 0;
    uint16_t sizeline = 0;
    uint16_t sizecol = 0;
    uint32_t pos = 1;
    uint16_t sizetitlebar = br->titlesbarsize + 9;
    uint16_t sizestatusbar = br->statusbarsize + 3;

    while( li )
    {
        co = li->cols;

        while( co )
        {
            br->sizecols[ co->id ] = co->attr.size > br->sizecols[ co->id ] 
            ? co->attr.size : br->sizecols[ co->id ];

            co = co->next; 
        }

        li = li->next;
    }

    for( uint8_t inc = 0; inc < 32; inc++ )
    {
        if( br->sizecols[ inc ] == 0 )
        {
            br->numsizecols = inc;
            break;
        }
    }

    li = br->ini;
    br->linesize = 0;
    
    while( li )
    {
        co = li->cols;

        while( co )
        {
            if( co->show )
            {
                co->attr.size = br->sizecols[ co->id ];
                sizecol += co->attr.size;
                sum++;
            }

            co = co->next; 
        }

        sizeline = sizecol + ( sum * 3 ) - 3;
        br->linesize = br->linesize > sizeline ? br->linesize : sizeline;
        sum = 0;
        sizecol = 0;
        li->id = pos;

        li = li->next;

        pos++;
    }

    br->linesize = br->linesize + 3;
    br->linesize = sizetitlebar > br->linesize ? sizetitlebar : br->linesize;
    br->linesize = sizestatusbar > br->linesize ? sizestatusbar : br->linesize;

    br->winini = br->lin + ( br->typebox == BOX_BROWSE_BW ? 4 : 2 );

    if( br->winsize > br->totallines ) br->winsize = br->totallines;

    br->blin = br->winini + br->winsize + ( br->typebox == BOX_BROWSE_BW ? 3 : 1 );
    br->rcol = br->col + br->linesize;
    
    br->winfin = br->winini + br->winsize - 1;

    br->init = true;

    if( br->cur == NULL ) br->cur = br->ini;

    return true;
}

int16_t exec_bw( BR *br )
{
    if( !br ) return -1;
    if( !br->ini ) return -1;
    if( br->totallines == 0 ) return -1;
    if( br->init == false ) return -1;
    
    br->init = false;

    wint_t c;
    int bytes;
    int8_t pos = 0;
    uint8_t col = br->col + 2;
    wchar_t buffer[ 64 ];

    LN *li = NULL;
    LN *liini = NULL;
    LN *lisrc = NULL;

    curs_hide();
    setcolor_off();

    setcolor_aon( br->color[ COLOR_BOX_BW ].colorc,
                  br->color[ COLOR_BOX_BW ].colorb,
                  br->color[ COLOR_BOX_BW ].colora );

    cleanarea( br->lin, br->col, br->blin, br->rcol );
    
    box( br->typebox, br->lin, br->col, br->blin, br->rcol );

    setcurs( br->lin, col );
    printtitlebar_bw( br );

    setcurs( br->blin, col );
    printstatusbar_bw( br );
    
    setcolor_off();

    li = liini = lisrc = br->cur;

    if( lisrc != br->ini ) lisrc = br->cur->next;

    if( br->typebox == BOX_BROWSE_BW ) disptitlecol_bw( br );

    dispwin_bw( br, li );

    if( !br->ctrlget )
    {
        setcolor_off();
        curs_show();
        return -1;
    }
    
    showlinecols_bw( br, liini, br->winini, col, FOCUS_ON );
    
    li = liini;

    setcolor_off();

    do
    {
        if( br->ctrlpercent )
        {
            setcolor_aon( br->color[ COLOR_BOX_BW ].colorc,
                          br->color[ COLOR_BOX_BW ].colorb,
                          br->color[ COLOR_BOX_BW ].colora );

            for( uint8_t inc = 1; inc < br->rcol - br->col; inc++ )
            {
                setcurs( br->lin, br->col + inc );
                printf( "\xe2\x94\x80" );
            }

            setcurs( br->lin, br->col + 2 );
            printtitlebar_bw( br );

            swprintf( buffer, 64, L" %d/%d - %2.1f %% ", li->id, br->totallines, 
                      ( float ) li->id / br->totallines * 100 );

            setcolor_on( br->color[ COLOR_BOX_BW ].colorc,
                         br->color[ COLOR_BOX_BW ].colorb,
                         2, NBRIG, NREVE );

            setcurs( br->lin, br->rcol - wcslen( buffer ) - 1 );
            imp_wstr( buffer, wcslen( buffer ), J_DIR, MINUSC );
        }

        c = get_ch( ECHO_OFF );

        setcolor_off();

        setsearchbar_bw( br, NULL );
        
        if( c == 27 )
        {
            ioctl( STDIN_FILENO, FIONREAD, &bytes );
                
            if( bytes )
            {
                c = get_ch( ECHO_OFF );

                if( c == 79 )
                {
                    c = get_ch( ECHO_OFF );

                    switch( c )
                    {
                        case 80: // F1 
                            break;
                        case 81: // F2

                            if( br->fu_f2 )
                            {
                                uint8_t lin = setsearchbar_bw( br, NULL );
                                
                                if( lin == 0 ) break;

                                liini = li;

                                curs_show();

                                li = br->fu_f2( br, lisrc );

                                cleanarea( lin, br->col + 1, lin, br->rcol - 1 ); 

                                curs_hide();

                                if( li == NULL ) li = liini;
                                
                                lisrc = li->next;
                               
                                if( !lisrc || lisrc == br->fin ) lisrc = br->ini;
                                
                                liini = li;
                                
                                dispwin_bw( br, li );
                                
                                pos = 0;
                            
                                showlinecols_bw( br, li, br->winini + pos, col, FOCUS_ON );
                            }

                            break;
                        case 82: // F3
                
                            if( br->fu_f3 )
                            {
                                curs_show();
                                li = br->fu_f3( br, li );
                                cleanarea( br->lin, br->col, br->blin, br->rcol );
                                return li->opt;
                            }

                            break;
                        case 83: // F4
                            
                            if( br->fu_f4 )
                            {
                                curs_show();
                                li = br->fu_f4( br, li );
                                cleanarea( br->lin, br->col, br->blin, br->rcol );
                                return li->opt;
                            }

                            break;
                    }
                }
                
                if( c == 91 )
                {
                    c = get_ch( ECHO_OFF );

                    switch( c )
                    {
                        case 49:
                            c = get_ch( ECHO_OFF );

                            switch( c )
                            {
                                case 53: // F5
                                case 55: // F6
                                case 56: // F7
                                case 57: // F8
                                    get_ch( ECHO_OFF );
                                    break;
                            }

                            break;
                        case 50:
                            c = get_ch( ECHO_OFF );

                            switch( c )
                            {
                                case 48: // F9
                                case 49: // F10
                                case 51: // F11
                                case 52: // F12
                                         //
                                    get_ch( ECHO_OFF );
                                    
                                    break;
                                case 126: // INS 

                                    if( br->fu_ins )
                                    {
                                        curs_show();
                                        li = br->fu_ins( br, li );
                                        cleanarea( br->lin, br->col, br->blin, br->rcol );
                                        return li->opt;
                                    }

                                    break;
                            }
                            
                            break;
                        case 51: // DEL
                            get_ch( ECHO_OFF );

                            curs_show();
                            
                            if( br->fu_del )
                            {
                                curs_show();
                                li = br->fu_del( br, li );
                                cleanarea( br->lin, br->col, br->blin, br->rcol );
                                return li->opt;
                            }

                            break;
                        case 53: // PGUP
                        case 54: // PGDN
                            
                            get_ch( ECHO_OFF );
                            
                            break;
                        case 66: // SETA ABAIXO
                        case 67: // SETA A DIREITA

                            showlinecols_bw( br, li, br->winini + pos, col, FOCUS_OFF );

                            li = li->next;
                            pos++;

                            if( pos == br->winsize )
                            {
                                if( li )
                                {
                                    li = liini->next;
                                    liini = li;
                                    
                                    li = dispwin_bw( br, li );
                                    
                                    pos--;
                                }
                            }
                            
                            if( !li )
                            {
                                li = br->ini;
                                
                                liini = li;
                                
                                dispwin_bw( br, li );
                                
                                pos = 0;
                            }

                            lisrc = li;

                            showlinecols_bw( br, li, br->winini + pos, col, FOCUS_ON );
                            
                            break;
                        case 65: // SETA ACIMA
                        case 68: // SETA A ESQUERDA
                            
                            showlinecols_bw( br, li, br->winini + pos, col, FOCUS_OFF );

                            li = li->prev;
                            pos--;

                            if( pos < 0 )
                            {
                                if( li )
                                {
                                    li = liini->prev;
                                    liini = li;
                                    
                                    dispwin_bw( br, li );

                                    pos++;
                                }
                                else
                                {
                                    li = br->fin;
                                
                                    for( pos = 0; li && pos < br->winsize; pos++ )
                                    {
                                        liini = li;
                                        li = li->prev;
                                    }

                                    li = liini;
                                
                                    li = dispwin_bw( br, li );

                                    pos--;
                                }
                            }
                            
                            lisrc = li;
                        
                            showlinecols_bw( br, li, br->winini + pos, col, FOCUS_ON );
                            
                            break;
                        case 70: // END
                            
                            li = br->fin;
                                
                            for( pos = 0; li && pos < br->winsize; pos++ )
                            {
                                liini = li;
                                li = li->prev;
                            }

                            li = liini;
                                
                            li = dispwin_bw( br, li );

                            pos--;
                            
                            lisrc = br->ini;
                            
                            showlinecols_bw( br, li, br->winini + pos, col, FOCUS_ON );

                            break;
                        case 72: // HOME
                            
                            li = br->ini;
                                
                            liini = li;
                                
                            dispwin_bw( br, li );
                                
                            pos = 0;

                            lisrc = li;
                            
                            showlinecols_bw( br, li, br->winini + pos, col, FOCUS_ON );

                            break;
                    }
                }
            }
            else
            {
                curs_show();
                cleanarea( br->lin, br->col, br->blin, br->rcol );
                return -1;
            }
        }

        if( c == 9 ) // tecla TAB
        {
            if( br->fu_tab )
            {
                curs_show();
                li = br->fu_tab( br, li );
                cleanarea( br->lin, br->col, br->blin, br->rcol );
                return li->opt;
            }
        }
    
        if( c == 10 ) // tecla ENTER
        {
            if( br->fu_enter )
            {
                curs_show();
                li = br->fu_enter( br, li );
                cleanarea( br->lin, br->col, br->blin, br->rcol );
            
                return li->opt;
            }

            if( li->f )
            {
                curs_show();
                li->f();
                cleanarea( br->lin, br->col, br->blin, br->rcol );
                
                return li->opt;
            }
        }
        
        if( c == 32 ) // tecla ESPAÇO
        {
            if( br->fu_spc )
            {
                curs_show();
                li = br->fu_spc( br, li );
                cleanarea( br->lin, br->col, br->blin, br->rcol );
                return li->opt;
            }
            else 
            {
                li->check = li->check ? false : true;
                showlinecols_bw( br, li, br->winini + pos, col, FOCUS_ON );
            }
        }
    }
    while( true );
}


void showlinecols_bw( BR *br, LN *li, uint8_t lin, uint8_t col, bool focus )
{
    if( !br ) return;
    if( !li ) return;
    
    uint8_t c = col;
    
    CO *co = li->cols;
    
    setcolor_aon( br->color[ COLOR_BOX_BW ].colorc,
                  br->color[ COLOR_BOX_BW ].colorb, 
                  br->color[ COLOR_BOX_BW ].colora );

    cleanarea( lin, br->col + 1, lin, br->rcol - 1 );

    for( uint8_t inc = 0; inc < br->numsizecols; inc++ )
    {
        if( !co )
        {
            c += br->sizecols[ inc ];

            c++;

            if( c == br->rcol ) break;

            showdbar_bw( br, lin, c );
        }
        else
        {
            if( co->show )
            {
                setcurs( lin, c );

                co->f( focus, br, li, co );
            
                c += br->sizecols[ inc ];

                if( co->showdbar )
                {
                    c++;

                    if( c == br->rcol ) break;

                    showdbar_bw( br, lin, c );
                }

                c += 2;
            }
            
            co = co->next;
        }
    }
}

void showdbar_bw( BR *br, uint8_t lin, uint8_t col )
{
    if( !br ) return;
    
    setcolor_aon( br->color[ COLOR_BOX_BW ].colorc,
                  br->color[ COLOR_BOX_BW ].colorb, 
                  br->color[ COLOR_BOX_BW ].colora );

    if( lin == br->winini )
    {
        setcurs( lin - 1, col );
        printf( "%c%c%c", '\xe2', '\x94', br->typebox == BOX_BROWSE_BW ? '\xbc' : '\xac' );
    }

    if( lin == br->winfin )
    {
        setcurs( lin + 1, col );
        printf( "%c%c%c", '\xe2', '\x94', '\xb4' );
    }
               
    setcurs( lin, col );
    printf( "%c%c%c", '\xe2', '\x94', '\x82' );

    return;
}

LN *dispwin_bw( BR *br, LN *li )
{
    LN *aux = NULL;

    clsarea_bw( br );

    for( uint8_t inc = 0; li && inc < br->winsize; inc ++ )
    {
        showlinecols_bw( br, li, br->winini + inc, br->col + 2, FOCUS_OFF );

        aux = li;

        li = li->next;
    }

    li = aux;

    return li;
}

void clsarea_bw( BR *br )
{
    uint8_t inc;

    cleanarea( br->winini, br->col + 1, br->winfin, br->rcol - 1 );  

    for( inc = 0; inc < br->linesize - 2; inc++ )
    {
        setcolor_aon( br->color[ COLOR_BOX_BW ].colorc,
                      br->color[ COLOR_BOX_BW ].colorb,
                      br->color[ COLOR_BOX_BW ].colora );
        
        setcurs( br->winfin + 1, br->col + 1 + inc );

        printf( "%c%c%c", '\xe2', '\x94', '\x80' );
    }
}

void f_void()
{
    return;
}
