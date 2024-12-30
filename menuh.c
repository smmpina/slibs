#include <stdint.h>
#include <time.h>
#include <wchar.h>
#include "newlib.h"
#include "menuh.h"

struct colors
{
    uint8_t colorc; // caractere
    uint8_t colorb; // fundo
    uint8_t colora[ 9 ]; // atributo
};

struct hints
{
    uint8_t lin;
    uint8_t col;
    struct colors color;
    wchar_t text[ 128 ];
};

struct attributes
{
    uint16_t size;
    uint8_t align;
    bool upperc;
    struct colors focus;
    struct colors nofocus;
};

struct columns
{
    uint16_t opt;
    uint8_t col;
    void (*f)();
    struct attributes attr;
    struct hints hint;
    wchar_t text[ 128 ];
    struct columns *prev;
    struct columns *next;
};

struct title
{
    wchar_t text[ 128 ];
    struct colors color;
};

struct menuh
{
    bool init;
    uint8_t lin; // linha
    uint8_t col; // coluna esquerda da linha
    uint8_t optspc; // espaço entre opcoes 
    uint16_t numcols; // total de colunas do menu
    uint8_t sizecols; // total largura das colunas
    struct colors color[ 5 ];
    struct title *tit;
    struct columns *ini;
    struct columns *fin;
    struct columns *cur;
};

MNH *create_mh()
{
    uint8_t i, ii;

    MNH *mh = ( MNH * ) malloc( sizeof( MNH ) );
    
    if( !mh ) return NULL;

    mh->init = false;
    mh->lin = 1;
    mh->col = 1;
    mh->numcols = 0;
    mh->sizecols = 0;
    mh->optspc = 1;
    
    for( i = 0; i < 5; i++ )
    {
        for( ii = 0; ii < 9; ii++ )
        {
            mh->color[ i ].colora[ ii ] = 0;
        }
    }
    
    mh->color[ COLOR_FOCUS_MH ].colorc = NBLK; // coluna foco
    mh->color[ COLOR_FOCUS_MH ].colorb = NWHT;
    mh->color[ COLOR_FOCUS_MH ].colora[ 0 ] = NNORM;
    
    mh->color[ COLOR_NOFOCUS_MH ].colorc = NBLK; // coluna
    mh->color[ COLOR_NOFOCUS_MH ].colorb = NGRY;
    mh->color[ COLOR_NOFOCUS_MH ].colora[ 0 ] = NNORM;

    mh->color[ COLOR_BACKGND_MH ].colorc = NBLK; // fundo das colunas
    mh->color[ COLOR_BACKGND_MH ].colorb = NBLK;
    mh->color[ COLOR_BACKGND_MH ].colora[ 0 ] = NNORM;

    mh->color[ COLOR_TITLE_MH ].colorc = NLYEL; // titulo
    mh->color[ COLOR_TITLE_MH ].colorb = NBLK;
    mh->color[ COLOR_TITLE_MH ].colora[ 0 ] = NNORM;

    mh->color[ COLOR_HINT_MH ].colorc = NGRY; // comentarios
    mh->color[ COLOR_HINT_MH ].colorb = NBLK;
    mh->color[ COLOR_HINT_MH ].colora[ 0 ] = NNORM;

    mh->ini = NULL;
    mh->fin = NULL;
    mh->cur = NULL;
    mh->tit = NULL;

    return mh;
}

void free_mh( MNH *mh )
{
    if( !mh ) return;

    COL *co;

    while( mh->ini )
    {
        co = mh->ini;
        mh->ini = mh->ini->next;

        free( co );
    }

    if( mh->tit ) free( mh->tit );

    free( mh );

    return;
}

COL *getinicol_mh( MNH *mh )
{
    if( !mh ) return NULL;

    return mh->ini;
}

COL *movecol_mh( COL *co )
{
    if( !co ) return NULL;

    return co->next;
}

TIH *addtitle_mh( MNH *mh, wchar_t *text )
{
    if( !mh ) return NULL;

    if( mh->tit != NULL ) return mh->tit;

    TIH *t = ( TIH * ) malloc( sizeof( TIH ) );

    if( !t ) return NULL;

    wcscpy( t->text, text );

    t->color.colorc = mh->color[ COLOR_TITLE_MH ].colorc;
    t->color.colorb = mh->color[ COLOR_TITLE_MH ].colorb;

    for( uint8_t i = 0; i < 9; i++ ) t->color.colora[ i ] = mh->color[ COLOR_TITLE_MH ].colora[ i ];

    mh->tit = t;

    return t;
}

void setcolortitle_mh( TIH *t, uint8_t colorc, uint8_t colorb, int numpars, ... )
{
    va_list lst;
    uint8_t inc;

    if( !t ) return;

    numpars = numpars < 0 ? 0 : numpars;
    numpars = numpars > 9 ? 9 : numpars;

    va_start( lst, numpars );

    t->color.colorc = colorc;
    t->color.colorb = colorb;

    for( inc = 0; inc < numpars; inc++ ) t->color.colora[ inc ] = va_arg( lst, int );

    va_end( lst );

    return;
}

void setcoord_mh( MNH *mh, uint8_t lin, uint8_t col, uint8_t optspc )
{
    if( !mh ) return;

    mh->lin = lin;
    mh->col = col;
    mh->optspc = optspc;

    return;
}

void setcolor_mh( MNH *mh, uint8_t colorpos, uint8_t colorc, uint8_t colorb, int numpars, ... )
{
    va_list lst;
    uint8_t inc;

    if( !mh ) return;

    numpars = numpars < 0 ? 0 : numpars;
    numpars = numpars > 9 ? 9 : numpars;

    va_start( lst, numpars );

    mh->color[ colorpos ].colorc = colorc;
    mh->color[ colorpos ].colorb = colorb;

    for( inc = 0; inc < numpars; inc++ ) mh->color[ colorpos ].colora[ inc ] = va_arg( lst, int );

    va_end( lst );

    return;
}

uint8_t getsizecols_mh( MNH *mh )
{
    return mh->sizecols;
}

COL *addcol_mh( MNH *mh, uint16_t opt, wchar_t *text, void (*f)() )
{
    if( !mh ) return NULL;

    COL *co = ( COL * ) malloc( sizeof( COL ) );

    if( !co ) return NULL;

    co->opt = opt;
    co->col = 0;
    co->f = f;
    wcscpy( co->text, text );

    co->hint.lin = 0;
    co->hint.col = 0;
    wcscpy( co->hint.text, L"" );

    co->hint.color.colorc = mh->color[ COLOR_HINT_MH ].colorc;
    co->hint.color.colorb = mh->color[ COLOR_HINT_MH ].colorb;

    for( int i = 0; i < 9; i++ ) co->hint.color.colora[ i ] = mh->color[ COLOR_HINT_MH ].colora[ i ];

    co->attr.size = wcslen( text );
    co->attr.align = J_ESQ;
    co->attr.upperc = MINUSC;

    co->attr.nofocus.colorc = mh->color[ COLOR_NOFOCUS_MH ].colorc;
    co->attr.nofocus.colorb = mh->color[ COLOR_NOFOCUS_MH ].colorb;

    for( int i = 0; i < 9; i++ ) co->attr.nofocus.colora[ i ] = mh->color[ COLOR_NOFOCUS_MH ].colora[ i ];
    
    co->attr.focus.colorc = mh->color[ COLOR_FOCUS_MH ].colorc;
    co->attr.focus.colorb = mh->color[ COLOR_FOCUS_MH ].colorb;

    for( int i = 0; i < 9; i++ ) co->attr.focus.colora[ i ] = mh->color[ COLOR_FOCUS_MH ].colora[ i ];

    COL **pr = &mh->ini;
    COL *aux = NULL;

    while( *pr )
    {
        aux = *pr;
        pr = &( *pr )->next;
    }

    if( *pr )
        ( *pr )->prev = co;
    else
        mh->fin = co;

    co->next = *pr;
    co->prev = aux;
    *pr = co;

    mh->numcols++;
    
    return co;
}

void setattrcol_mh( COL *co, uint16_t size, uint8_t align, bool upperc )
{
    if( !co ) return;

    co->attr.size = size;
    co->attr.align = align;
    co->attr.upperc = upperc;
}

void setcolorcol_mh( COL *co, bool focus, uint8_t colorc, uint8_t colorb, int numpars, ... )
{
    va_list lst;
    uint8_t inc;

    if( !co ) return;

    numpars = numpars < 0 ? 0 : numpars;
    numpars = numpars > 9 ? 9 : numpars;

    va_start( lst, numpars );

    if( focus )
    {
        co->attr.focus.colorc = colorc;
        co->attr.focus.colorb = colorb;

        for( inc = 0; inc < numpars; inc++ ) co->attr.focus.colora[ inc ] = va_arg( lst, int );
    }
    else
    {
        co->attr.nofocus.colorc = colorc;
        co->attr.nofocus.colorb = colorb;

        for( inc = 0; inc < numpars; inc++ ) co->attr.nofocus.colora[ inc ] = va_arg( lst, int );
    }

    va_end( lst );

    return;
}

COL *setfocus_mh( MNH *mh, uint8_t opt )
{
    if( !mh ) return NULL;

    COL *co = mh->ini;

    while( co && co->opt != opt ) co = co->next;

    mh->cur = co;

    return co;
}

void addhintcol_mh( COL *co, uint8_t lin, uint8_t col, wchar_t *text )
{
    if( !co ) return;

    co->hint.lin = lin;
    co->hint.col = col;
    wcscpy( co->hint.text, text );

    return;
}

void setcolorhintcol_mh( COL *co, uint8_t colorc, uint8_t colorb, int numpars, ... )
{
    va_list lst;
    uint8_t inc;

    if( !co ) return;

    numpars = numpars < 0 ? 0 : numpars;
    numpars = numpars > 9 ? 9 : numpars;

    va_start( lst, numpars );

    co->hint.color.colorc = colorc;
    co->hint.color.colorb = colorb;

    for( inc = 0; inc < numpars; inc++ ) co->hint.color.colora[ inc ] = va_arg( lst, int );

    va_end( lst );

    return;
}

bool init_mh( MNH *mh )
{
    if( !mh ) return false;
    if( !mh->ini ) return false;
    if( mh->numcols == 0 ) false;
    
    uint8_t t_opc = 0;
    COL *co = mh->ini;
    TIH *tit = mh->tit;

    if( tit ) mh->sizecols = wcslen( tit->text ) + mh->optspc;

    while( co )
    {
        co->col = mh->col + mh->sizecols;
        
        t_opc = mh->optspc + co->attr.size;

        mh->sizecols += t_opc;
        
        co = co->next;
    }

    mh->init = true;

    if( mh->cur == NULL ) mh->cur = mh->ini;

    return mh->init;
}

int16_t exec_mh( MNH *mh )
{
    if( !mh ) return -1;
    if( !mh->ini ) return -1;
    if( mh->numcols == 0 ) return -1;
    if( mh->init == false ) return -1;

    wint_t c;
    int bytes;
    uint8_t tdesc;

    COL *co = mh->ini;
    TIH *tit = mh->tit;

    curs_hide();
    clsbackgnd_mh( mh );
    
    if( tit )
    {
        tdesc = wcslen( tit->text );

        setcolor_aon( tit->color.colorc, tit->color.colorb, tit->color.colora );
        setcurs( mh->lin, mh->col );
        imp_wstr( tit->text, tdesc, J_ESQ, MINUSC );
    }

    while( co )
    {
        showopt_mh( mh, co, FOCUS_OFF );

        co = co->next;
    }
    
    co = mh->cur;

    showopt_mh( mh, co, FOCUS_ON );
    hinton_mh( co );

    setcolor_off();

    do
    {
        c = get_ch( ECHO_OFF );

        setcolor_off();
        
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
                            break;
                        case 82: // F3
                            break;
                        case 83: // F4
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
                                    get_ch( ECHO_OFF );
                                    break;
                                case 126: // INS 
                                    break;
                            }
                            
                            break;
                        case 51: // DEL
                            get_ch( ECHO_OFF );
                            break;
                        case 53: // PGUP
                        case 54: // PGDN
                            get_ch( ECHO_OFF );
                            break;
                        case 66: // SETA ABAIXO
                        case 67: // SETA A DIREITA
                            showopt_mh( mh, co, FOCUS_OFF );
                            hintoff_mh( co );

                            co = co->next;

                            if( !co ) co = mh->ini;
                        
                            showopt_mh( mh, co, FOCUS_ON );
                            hinton_mh( co );
                            
                            break;
                        case 65: // SETA ACIMA
                        case 68: // SETA A ESQUERDA
                            showopt_mh( mh, co, FOCUS_OFF );
                            hintoff_mh( co );

                            co = co->prev;

                            if( !co ) co = mh->fin;
                        
                            showopt_mh( mh, co, FOCUS_ON );
                            hinton_mh( co );
                            
                            break;
                        case 70: // END
                            break;
                        case 72: // HOME
                            break;
                    }
                }
            }
            else
            {
                curs_show();
                clsbackgnd_mh( mh );
                hintoff_mh( co );

                return -1;
            }
        }
    
        if( c == 10 ) // tecla ENTER
        {
            curs_show();

            if(co->f ) co->f();

            clsbackgnd_mh( mh );
            hintoff_mh( co );
            
            return co->opt;
        }
    }
    while( true );
    
    clsbackgnd_mh( mh );
    hintoff_mh( co );
}

void showopt_mh( MNH *mh, COL *co, bool focus )
{
    setcolor_aon( co->attr.nofocus.colorc, co->attr.nofocus.colorb, co->attr.nofocus.colora );
    
    if( focus ) setcolor_aon( co->attr.focus.colorc, co->attr.focus.colorb, co->attr.focus.colora );

    setcurs( mh->lin, co->col );
    imp_wstr( co->text, co->attr.size, co->attr.align, co->attr.upperc );

    return;
}

void clsbackgnd_mh( MNH *mh )
{
    setcolor_aon( mh->color[ COLOR_BACKGND_MH ].colorc, mh->color[ COLOR_BACKGND_MH ].colorb,
             mh->color[ COLOR_BACKGND_MH ].colora );
    cleanarea( mh->lin, mh->col, mh->lin, mh->col + mh->sizecols );

    return;
}

void hinton_mh( COL *co )
{
    if( !co ) return;

    uint8_t t = wcslen( co->hint.text );

    if( t )
    {
        setcolor_aon( co->hint.color.colorc, co->hint.color.colorb, co->hint.color.colora );
        cleanarea( co->hint.lin, co->hint.col, co->hint.lin, co->hint.col + t );
        setcurs( co->hint.lin, co->hint.col );
        imp_wstr( co->hint.text, t, J_ESQ, MINUSC );
    }

    return;
}

void hintoff_mh( COL *co )
{
    if( !co ) return;

    uint8_t t = wcslen( co->hint.text );

    if( t )
    {
        setcolor_aon( co->hint.color.colorc, co->hint.color.colorb, co->hint.color.colora );
        cleanarea( co->hint.lin, co->hint.col, co->hint.lin, co->hint.col + t );
    }

    return;
}
