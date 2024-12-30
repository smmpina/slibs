#include "newlib.h"
#include "colors.h"
#include "menuh.h"
#include "brws.h"
#include <bits/types/timer_t.h>
#include <stdint.h>
#include <stdlib.h>
#include <wchar.h>

static struct termios orig;
static struct termios new;

void setcurs( uint8_t lin, uint8_t col )
{
    lin = lin <= 0 ? 1 : lin;
    col = col <= 0 ? 1 : col;

    printf( "\033[%hhu;%hhuH", lin, col );
}

void curs_up( uint8_t n )
{
    n = n <= 0 ? 1 : n;

    printf( "\033[%hhuA", n );
}

void curs_down( uint8_t n )
{
    n = n <= 0 ? 1 : n;
    
    printf( "\033[%hhuB", n );
}

void curs_forward( uint8_t n )
{
    n = n <= 0 ? 1 : n;
    
    printf( "\033[%hhuC", n );
}

void curs_back( uint8_t n )
{
    n = n <= 0 ? 1 : n;
    
    printf( "\033[%hhuD", n );
}

void curs_col( uint8_t n )
{
    n = n <= 0 ? 1 : n;
    
    printf( "\033[%hhuG", n );
}

void curs_save( void )
{
    printf( "\033[s" );
}

void curs_rest( void )
{
    printf( "\033[u" );
}

void curs_show( void )
{
    printf( "\033[?25h" );
}

void curs_hide( void )
{
    printf( "\033[?25l" );
}

void line_down( uint8_t n )
{
    n = n <= 0 ? 1 : n;
    
    printf( "\033[%hhuE", n );
}

void line_up( uint8_t n )
{
    n = n <= 0 ? 1 : n;
    
    printf( "\033[%hhuF", n );
}

void cls_line( uint8_t n )
{
    switch( n )
    {
        case 0: // limpa do cursor ate o final da linha
        case 1: // limpa do cursor ate o inicio da linha
        case 2: // limpa toda a linha
            printf( "\033[%hhuK", n );
            break;
    }
}

void cls_display( uint8_t n )
{
    switch( n )
    {
        case 0: // limpa do cursor ate o final da tela
        case 1: // limpa do cursor ate o inicio da tela
        case 2: // limpa toda a tela
            printf( "\033[%hhuJ", n );
            break;
    }
}

void cls_scr() // limpa tela
{
    printf( "\033[2J" );
    printf( "\033[H" );
}

void cls_area( uint8_t lin, uint8_t col )
{
    setcurs( lin, col );
    setcolor_off();
    cls_display( 0 );
}

//----------------------------------------------------------------------------------------

void setup_console_new( bool echo )
{
    tcgetattr( STDIN_FILENO, &orig );

    new = orig;

    new.c_lflag &= ~ICANON;

    new.c_lflag &= echo ? ECHO : ~ECHO;

    new.c_cc[ VTIME ] = 0;
    new.c_cc[ VMIN ] = 1;

    new.c_iflag &= ~( ISTRIP | INPCK );

    tcsetattr( STDIN_FILENO, TCSANOW, &new );
}

void restore_console_new( void )
{
    tcsetattr( STDIN_FILENO, TCSANOW, &orig );
}

wint_t get_ch( bool echo )
{
    wint_t ch;

    setbuf( stdin, NULL );

    setup_console_new( echo );

    ch = getwchar();

    restore_console_new();

    return ch;
}

//----------------------------------------------------------------------------------------

void box( uint8_t type, uint8_t lin, uint8_t col, uint8_t blin, uint8_t rcol )
{
    uint8_t i;

    setcurs( lin, col ); 
    printf( "\xe2\x95\xad" ); // canto superior esquerdo arrendondado

    for( i = 1; i < rcol - col; i++ )
    {
        setcurs( lin, col + i );
        printf( "\xe2\x94\x80" ); // linha horizontal superior
    }
    
    setcurs( lin, rcol );
    printf( "\xe2\x95\xae" ); // canto superior direito arrendondado

    for( i = 1; i < blin - lin; i++ ) // duas linhas verticais
    {
        setcurs( lin + i, col  );
        printf( "\xe2\x94\x82" );
        setcurs( lin + i, rcol );
        printf( "\xe2\x94\x82" );
    }

    setcurs( blin, col ); // canto inferior esquerdo arrendondado
    printf( "\xe2\x95\xb0" ); 

    for( i = 1; i < rcol - col; i++ )
    {
        setcurs( blin, col + i );
        printf( "\xe2\x94\x80" ); // linha horizontal inferior
    }
    
    setcurs( blin, rcol );
    printf( "\xe2\x95\xaf" ); // canto inferior direito arrendondado
    
    if( type == BOX_FORM )
    {
        setcurs( lin + 2, col ); // terceira linha horizontal superior
        printf( "\xe2\x94\x9c" );

        for( i = 1; i < rcol - col; i++ )
        {
            setcurs( lin + 2, col + i );
            printf( "\xe2\x94\x80" );
        }

        setcurs( lin + 2, rcol );
        printf( "\xe2\x94\xa4" );

        setcurs( blin - 2, col ); // terceira linha horizontal inferior
        printf( "\xe2\x94\x9c" );

        for( i = 1; i < rcol - col; i++ )
        {
            setcurs( blin - 2, col + i );
            printf( "\xe2\x94\x80" );
        }

        setcurs( blin - 2, rcol );
        printf( "\xe2\x94\xa4" );
    }

    if( type == BOX_MENU_BW || type == BOX_BROWSE_BW )
    {
        setcurs( lin + 1, col ); // segunda linha horizontal superior
        printf( "\xe2\x94\x9c" );

        for( i = 1; i < rcol - col; i++ )
        {
            setcurs( lin + 1, col + i );
            printf( "\xe2\x94\x80" );
        }

        setcurs( lin + 1, rcol );
        printf( "\xe2\x94\xa4" );
        
        setcurs( blin - 1, col ); // segunda linha horizontal inferior
        printf( "\xe2\x94\x9c" );

        for( i = 1; i < rcol - col; i++ )
        {
            setcurs( blin - 1, col + i );
            printf( "\xe2\x94\x80" );
        }

        setcurs( blin - 1, rcol );
        printf( "\xe2\x94\xa4" );
    }

    if( type == BOX_BROWSE_BW )
    {
        setcurs( lin + 3, col ); // terceira linha horizontal superior
        printf( "\xe2\x94\x9c" );

        for( i = 1; i < rcol - col; i++ )
        {
            setcurs( lin + 3, col + i );
            printf( "\xe2\x94\x80" );
        }

        setcurs( lin + 3, rcol );
        printf( "\xe2\x94\xa4" );

        setcurs( blin - 3, col ); // terceira linha horizontal inferior
        printf( "\xe2\x94\x9c" );

        for( i = 1; i < rcol - col; i++ )
        {
            setcurs( blin - 3, col + i );
            printf( "\xe2\x94\x80" );
        }

        setcurs( blin - 3, rcol );
        printf( "\xe2\x94\xa4" );
    }
}

void setcolor_on( uint8_t colorc, uint8_t colorb, int numpars, ... )
{
    va_list l;
    uint8_t inc;
    uint8_t t[ 9 ] = { 0 }; 

    numpars = numpars < 0 ? 0 : numpars;
    numpars = numpars > 9 ? 9 : numpars;

    va_start( l, numpars );

    for( inc = 0; inc < numpars; inc++ ) t[ inc ] = va_arg( l, int );

    printf( "\033[%hhu;%hhu;%hhu;%hhu;%hhu;%hhu;%hhu;%hhu;%hhu;38;5;%hhu;48;5;%hhum",
            t[ 8 ], t[ 7 ], t[ 6 ], t[ 5 ], t[ 4 ], t[ 3 ], t[ 2 ], t[ 1 ], t[ 0 ],
            colorc, colorb );

    va_end( l );

    return;
}

void setcolor_aon( uint8_t colorc, uint8_t colorb, uint8_t colora[] )
{
    printf( "\033[%hhu;%hhu;%hhu;%hhu;%hhu;%hhu;%hhu;%hhu;%hhu;38;5;%hhu;48;5;%hhum",
            colora[ 8 ],
            colora[ 7 ],
            colora[ 6 ],
            colora[ 5 ],
            colora[ 4 ],
            colora[ 3 ],
            colora[ 2 ],
            colora[ 1 ],
            colora[ 0 ],
            colorc, colorb );
    return;
}

void setcolor_off()
{
    printf( "\033[0m" );

    return;
}

void cleanarea( uint8_t lin, uint8_t col, uint8_t blin, uint8_t rcol )
{
    uint8_t l;
    uint8_t c;

    for( l = lin; l <= blin; l++ )
    {
        for( c = col; c <= rcol; c++ )
        {
            setcurs( l, c );
            putchar( ' ' );
        }
    }

    setcurs( lin, col );

    return;
}

void ret_wmsg_form( wchar_t *text, bool error )
{
    size_t t = wcslen( text );
    
    curs_hide();

    setcolor_on( NWHT, error ? NRED : NBLU, 3, NBRIG, NBLKS, NITAL );
    cleanarea( LIN_FORM_ROD, COL_FORM_ROD, LIN_FORM_ROD, COLD_FORM - 1 );

    setcurs( LIN_FORM_ROD, COL_FORM_ROD + 1 );
    
    imp_wstr( text, t, J_ESQ, MINUSC );

    get_ch( ECHO_OFF );

    setcolor_on( NLWHT, 238, 1, NNORM );
    cleanarea( LIN_FORM_ROD, COL_FORM_ROD, LIN_FORM_ROD, COLD_FORM - 1 );

    setcolor_off();

    curs_show();

    return;
}
//----------------------------------------------------------------------------------------

void disp_get( uint8_t lin, uint8_t col, wchar_t *text, uint16_t size )
{
    setcurs( lin, col );
    setcolor_off();
    
    setcolor_on( NLYEL, COR_INPUT, 1, NBRIG );
    
    imp_wstr( text, size, J_ESQ, MINUSC );
    
    setcolor_on( NBLK, 108, 1, NBRIG );

    printf( "[\xe2\x96\xb2 Limpa]" ); 

    setcolor_on( NLYEL, COR_INPUT, 1, NBRIG );
    
    return;
}

bool filter_input_num( wint_t c )
{
    return( c > 47 && c < 58 );
}

bool filter_input_doub( wint_t c )
{
    bool nu = c > 47 && c < 58;

    return nu || c == 44;
}

bool filter_input_no( wint_t c )
{
    return true;
}

int input_wstr( uint8_t lin, uint8_t col, wchar_t *text, uint16_t size, bool mode, bool ( *ffi ) ( wint_t ) )
{
    uint8_t i;
    uint8_t inc = 0;
    uint8_t pos = 0;
    int ret = 0;
    int bytes;
    wint_t c;
    wchar_t buffer[ 256 ];
    wchar_t tmp[ 256 ];

    if( mode == ENTMODO_EDIT )
    {
        pos = wcslen( text );
    
        pos = pos > size ? size : pos;

        for( inc = 0; inc < pos; inc++ ) buffer[ inc ] = text[ inc ];
    }

    buffer[ inc ] = '\0';

    disp_get( lin, col, buffer, size );

    setcurs( lin, col + pos );

    i = pos;

    while( i < size )
    {
        setbuf( stdin, NULL );

        c = get_ch( ECHO_ON );

        if( c == 9 ) setcurs( lin, col + pos ); // TAB
        else if( c == 10 ) break; // ENTER
        else if( c == 27 ) // ESCAPE
        {
            ioctl( STDIN_FILENO, FIONREAD, &bytes );

            if( bytes )
            {
                c = get_ch( ECHO_ON );

                if( c == 79 )
                {
                    c = get_ch( ECHO_ON );

                    switch( c )
                    {
                        case 80:
                        case 81:
                        case 82:
                        case 83:
                            disp_get( lin, col, L" ", size );
                            
                            buffer[ 0 ] = '\0';
                            pos = 0;
                            i = 0;
                            
                            setcurs( lin, col + pos );
                            break;
                    }
                }

                if( c == 91 )
                {
                    c = get_ch( ECHO_ON );

                    switch( c )
                    {
                        case 49:
                            c = get_ch( ECHO_ON );

                            switch( c )
                            {
                                case 53:
                                case 55:
                                case 56:
                                case 57:
                                    get_ch( ECHO_ON );
                                    break;
                            }

                            break;
                        case 50:
                            c = get_ch( ECHO_ON );

                            switch( c )
                            {
                                case 48:
                                case 49:
                                case 51:
                                case 52:
                                    get_ch( ECHO_ON );
                                    break;
                            }

                            break;
                        case 51: // DEL
                            get_ch( ECHO_ON );

                            if( buffer[ pos ] )
                            {
                                i--;
                                inc = pos;

                                do
                                {
                                    buffer[ inc ] = buffer[ inc + 1 ];
                                    inc++;
                                }
                                while( buffer[ inc ] );

                                disp_get( lin, col, buffer, size );
                            }

                            setcurs( lin, col + pos );

                            break;
                        case 53:
                        case 54:
                            get_ch( ECHO_ON );

                            break;
                        case 65: // SETA ACIMA
                            disp_get( lin, col, L" ", size );
                            
                            buffer[ 0 ] = '\0';
                            pos = 0;
                            i = 0;
                            
                            setcurs( lin, col + pos );
                            
                            break;
                        case 66: // SETA ABAIXO
                            setcurs( lin, col + pos );

                            break;
                        case 67: // SETA DIREITA
                            if( pos < i ) pos++;  
                            
                            setcurs( lin, col + pos );
                            
                            break;
                        case 68: // SETA ESQUERDA
                            if( pos > 0 ) pos--;  
                            
                            setcurs( lin, col + pos );

                            break;
                        case 70: // END
                            pos = i;
                            setcurs( lin, col + pos );

                            break;
                        case 72: // HOME
                            pos = 0;
                            setcurs( lin, col + pos );
                         
                            break;
                    }
                }
            }
            else
            {
                ret = -1;
                i = 0;

                break;
            }
        }
        else if( c == 127 ) // Backspace
        {
            if( pos > 0 )
            {
                for( i = pos - 1; buffer[ i ]; i++ ) buffer[ i ] = buffer[ i + 1 ];

                disp_get( lin, col, buffer, size );
                
                pos--;
                i--;

                setcurs( lin, col + pos );
            }
        }
        else
        {
            if( !ffi( c ) ) break;

            if( pos < i )
            {
                wcscpy( tmp, buffer );

                buffer[ pos ] = c;

                for( inc = pos; tmp[ inc ]; inc++ ) buffer[ inc + 1 ] = tmp[ inc ];

                buffer[ inc + 1 ] = '\0';

                disp_get( lin, col, buffer, size );

                i++;
                pos++;

                setcurs( lin, col + pos );
            }
            else
            {
                buffer[ i ] = c;

                wprintf( L"%lc", buffer[ i ] );

                i++;
                pos++;

                buffer[ i ] = '\0';
            }
        }
    }

    setcolor_off();

    cleanarea( lin, col, lin, col + size + 9 );

    buffer[ i ] = '\0';

    allwtrim( buffer );
    
    if( !ret ) ret = wcslen( buffer );

    wcscpy( text, buffer );

    return( ret );
}

int wstr_to_int( const wchar_t *str, int *num )
{
    int ret;

    for( *num = 0, ret = 0; *str && iswdigit( *str ); ret++ )
    {
        *num *= 10;
        *num += *str++ - '0';
    }

    return ret;
}

int wstr_to_doub( const wchar_t *str, double *num, int precision )
{
    int ret;
    int n;

    for( n = 0, ret = 0; *str && iswdigit( *str ); ret++ )
    {
        n *= 10;
        n += *str++ - '0';
    }

    *num = ret ? ( double ) n / pow( 10, precision ) : 0.0;

    return ret;
}

int input_wint( uint8_t lin, uint8_t col, int *n, uint16_t size, bool mode )
{
    wchar_t buffer[ 128 ];
    int ret;
    bool ( *fi ) ( wint_t );

    fi = filter_input_num;

    swprintf( buffer, 128, L"%d", ( int ) *n );

    ret = input_wstr( lin, col, buffer, size, mode, fi );

    if( ret > 0 ) wstr_to_int( buffer, n );

    return ret;
}

int input_wdouble( uint8_t lin, uint8_t col, double *n, uint16_t size, uint8_t precision, bool mode )
{
    wchar_t buffer[ 128 ];
    int ret;
    double aux = 0.00;
    bool ( *fi ) ( wint_t );

    fi = filter_input_num;

    aux = *n * pow( 10, precision );

    swprintf( buffer, 128, L"%.*lf", 0, ( double ) aux );

    ret = input_wstr( lin, col, buffer, size, mode, fi );

    if( ret > 0 ) wstr_to_doub( buffer, n, precision );

    return ret;
}

//----------------------------------------------------------------------------------------

void reverse_str( char *str )
{
    char *ini, *fim;
    char c;

    ini = fim = str;

    while( *fim ) fim++;

    for( fim--; ini <= fim; ini++, fim-- )
    {
        c = *ini;
        *ini = *fim;
        *fim = c;
    }
}

void reverse_wstr( wchar_t *str )
{
    wchar_t *ini, *fim;
    wchar_t c;

    ini = fim = str;

    while( *fim ) fim++;

    for( fim--; ini <= fim; ini++, fim-- )
    {
        c = *ini;
        *ini = *fim;
        *fim = c;
    }
}

void alltrim( char *str )
{
    char *p_str = NULL;
    char *p_aux = str;

    while( isblank( *p_aux ) ) p_aux++;

    p_str = p_aux;

    if( *p_str )
    {
        while( *p_aux ) p_aux++;

        while( isblank( *--p_aux ) );

        *++p_aux = '\0';
    }

    while( ( *str++ = *p_str++ ) );

    return;
}

void allwtrim( wchar_t *str )
{
    wchar_t *p_str = NULL;
    wchar_t *p_aux = str;

    while( iswblank( *p_aux ) ) p_aux++;

    p_str = p_aux;

    if( *p_str )
    {
        while( *p_aux ) p_aux++;

        while( iswblank( *--p_aux ) );

        *++p_aux = '\0';
    }

    while( ( *str++ = *p_str++ ) );

    return;
}

char *padl( char *dest, char *orig, uint16_t padlen )
{
    char *p_orig = orig;
    char *p_dest = dest;

    for( uint8_t i = 0; i < padlen; i++, p_dest++ ) *p_dest = ' ';

    while( ( *p_dest++ = *p_orig++ ) );

    *p_dest = '\0';

    return dest;
}

wchar_t *padwl( wchar_t *dest, wchar_t *orig, uint16_t padlen )
{
    wchar_t *p_orig = orig;
    wchar_t *p_dest = dest;

    for( uint8_t i = 0; i < padlen; i++, p_dest++ ) *p_dest = L' ';

    while( ( *p_dest++ = *p_orig++ ) );

    *p_dest = '\0';

    return dest;
}

wchar_t *doub_to_wstr( double num, uint8_t precision, wchar_t *str )
{
    wchar_t buffer[ 128 ];
    wchar_t *pbuf = NULL;
    wchar_t *pstr = str;
    uint8_t inc = 0;

    if( precision <= 0 ) precision = 6;

    swprintf( buffer, 128, L"%.*lf", precision, num );

    reverse_wstr( buffer );

    pbuf = buffer;

    while( *pbuf != ',' && *pbuf != '.' ) *pstr++ = *pbuf++;

    *pstr++ = *pbuf++;

    for( inc = 0; *pbuf; inc++ )
    {
        if( inc == 3 )
        {
            *pstr++ = '.';
            inc = 0;
        }

        *pstr++ = *pbuf++;
    }

    *pstr = '\0';

    reverse_wstr( str );

    return str;
}

wchar_t *int_to_wstr( int num, uint8_t precision, wchar_t *str )
{
    wchar_t buffer[ 128 ];
    wchar_t *pbuf = NULL;
    wchar_t *pstr = str;
    uint8_t inc;

    swprintf( buffer, 128, L"%.*d", precision, ( int ) num );

    reverse_wstr( buffer );

    pbuf = buffer;

    for( inc = 0; *pbuf; inc++ )    
    {
        if( inc == 3 )
        {
            *pstr++ = '.';
            inc = 0;
        }

        *pstr++ = *pbuf++;
    }

    *pstr = '\0';

    reverse_wstr( str );

    return str;
}

wchar_t *date_to_wstr( time_t segs, wchar_t *str )
{
    struct tm *t;
    wchar_t *p = str;

    t = localtime( &segs );

    wcsftime( str, 64, L"%d/%m/%Y", t ); 

    return p;
}

wchar_t *datehours_to_wstr( time_t segs, wchar_t *str )
{
    struct tm *t;
    wchar_t *p = str;

    t = localtime( &segs );

    wcsftime( str, 64, L"%d/%m/%Y %H:%M:%S", t ); 

    return p;
}

wchar_t *dweek_to_wstr( time_t segs, wchar_t *str )
{
    struct tm *t;
    wchar_t *p = str;

    t = localtime( &segs );
    
    wcsftime( str, 64, L"%a", t );

    return p;
}

//----------------------------------------------------------------------------------------

void imp_str( char *str, uint16_t size, uint8_t align, bool upperc )
{
    int strsize = strlen( str );
    uint16_t padlen = size < strsize ? 0 : ( size - strsize ) / 2; 
    char buf_pad[ padlen + strsize + 1 ];
    char buf_upp[ strsize + 1 ];
    char *p_upp;

    strcpy( buf_upp, str );

    p_upp = buf_upp;

    for( uint8_t i = 0; buf_upp[ i ]; i++ ) buf_upp[ i ] = toupper( buf_upp[ i ] );
    
    switch( align )
    {
        case 0:
            printf( "%s", str );
            break;
        case J_ESQ:
            printf( "%-*.*s", size, size, upperc ? p_upp : str );
            break;
        case J_CEN:
            printf( "%-*.*s", size, size, upperc ? padl( buf_pad, p_upp, padlen ) :
                                                   padl( buf_pad, str, padlen ) );
            break;
        case J_DIR:
            printf( "%*.*s", size, size, upperc ? p_upp : str );
            break;
    }
    
    return;
}

void imp_wstr( wchar_t *str, uint16_t size, uint8_t align, bool upperc )
{
    size_t strwsize = wcslen( str );
    uint16_t padlen = size < strwsize ? 0 : ( size - strwsize ) / 2; 
    wchar_t buf_pad[ padlen + strwsize + 1 ];
    wchar_t buf_upp[ strwsize + 1 ];
    wchar_t *p_upp;

    wcscpy( buf_upp, str );

    p_upp = buf_upp;

    for( uint8_t i = 0; buf_upp[ i ]; i++ ) buf_upp[ i ] = towupper( buf_upp[ i ] );

    freopen( NULL, "w", stdout );

    switch( align )
    {
        case J_ESQ:
            wprintf( L"%-*.*ls", size, size, upperc ? p_upp : str );
            break;
        case J_CEN:
            wprintf( L"%-*.*ls", size, size, upperc ? padwl( buf_pad, p_upp, padlen ) :
                                                      padwl( buf_pad, str, padlen ) );
            break;
        case J_DIR:
            wprintf( L"%*.*ls", size, size, upperc ? p_upp : str );
            break;
    }

    freopen( NULL, "w", stdout );
}

void imp_wint( int num, uint16_t size, uint8_t precision, uint8_t align )
{
    wchar_t buffer[ 128 ];

    imp_wstr( int_to_wstr( num, precision, buffer ), size, align, MINUSC );
}

void imp_wdoub( double num, uint16_t size, uint8_t precision, uint8_t align )
{
    wchar_t buffer[ 128 ];

    imp_wstr( doub_to_wstr( num, precision, buffer ), size, align, MINUSC );
}

void imp_date( time_t segs, uint16_t size, uint8_t align, bool upperc )
{
    wchar_t date[ 128 ];
    wchar_t buffer[ 64 ];

    dweek_to_wstr( segs, date );
    wcscat( date, L", " );
    wcscat( date, date_to_wstr( segs, buffer ) );

    imp_wstr( date, size, align, upperc );
}

void imp_colordate( time_t segs, uint16_t size, uint8_t colorc, uint8_t colorb, int numpars, ... )
{
    va_list l;
    struct tm *t;
    wchar_t buffer[ 64 ];
    uint8_t colorweek;
    uint8_t colora[ 9 ] = { 0 };

    numpars = numpars < 0 ? 0 : numpars;
    numpars = numpars > 9 ? 9 : numpars;

    va_start( l, numpars );

    for( uint8_t inc = 0; inc < numpars; inc++ ) colora[ inc ] = va_arg( l, int );

    t = localtime( &segs );

    if( t->tm_wday == 0 ) colorweek = NRED;
    else if( t->tm_wday == 6 ) colorweek = NYEL;
    else colorweek = NBLU;

    setcolor_aon( colorweek, colorb, colora );

    imp_wstr( dweek_to_wstr( segs, buffer ), 3, J_ESQ, MINUSC );

    setcolor_aon( colorc, colorb, colora );
    
    imp_wstr( L", ", 2, J_ESQ, MINUSC );

    imp_wstr( datehours_to_wstr( segs, buffer ), size - 5, J_ESQ, MINUSC );

    va_end( l );

    setcolor_off();
}

//----------------------------------------------------------------------------------------

void init_form( wchar_t *title )
{
    size_t titlesize = wcslen( title );
    time_t now;
    uint8_t datesize = 21;

    now = time( NULL );

    cls_scr();

    setcolor_on( NGRY, NBLK, 1, NNORM );
    box( BOX_FORM, LIN_FORM, COL_FORM, LIND_FORM, COLD_FORM );
    setcolor_off();

    setcolor_on( NLWHT, COR_FUNDO_TIT, 2, NBRIG, NITAL );
    cleanarea( LIN_FORM_TIT, COL_FORM_TIT, LIN_FORM_TIT, COLD_FORM - 1 );
    setcurs( LIN_FORM_TIT, COL_FORM_TIT + 1 );
    imp_wstr( title, titlesize, J_ESQ, MINUSC );
    setcolor_off();

    setcurs( LIN_FORM_TIT, COLD_FORM - datesize - 1 );

    imp_colordate( now, datesize, NLWHT, 238, 2, NBRIG, NITAL );

    setcolor_on( NLWHT, COR_FUNDO_ROD, 1, NNORM );
    cleanarea( LIN_FORM_ROD, COL_FORM_ROD, LIN_FORM_ROD, COLD_FORM - 1 );
    
    setcolor_off();

    return;
}

int8_t hdialog_button( wchar_t *text, bool optdefault )
{
    MNH *menu = NULL;
    COL *co = NULL;
    TIH *tit = NULL;
    uint8_t ret;
    uint8_t lin = LIN_FORM_ROD;
    uint8_t col = COL_FORM_ROD + 1;
    wchar_t *opts[] = { L"Cancelar", L"Ok" };
 
    menu = create_mh();

    setcoord_mh( menu, lin, col, 1 );
    setcolor_mh( menu, COLOR_NOFOCUS_MH, NBLK, NGRY, 1, NNORM );
    setcolor_mh( menu, COLOR_BACKGND_MH, NBLK, COR_FUNDO_ROD, 1, NNORM );

    tit = addtitle_mh( menu, text );
    setcolortitle_mh( tit, NLYEL, COR_FUNDO_ROD, 1, NBRIG );

    for( int8_t i = 1; i >= 0; i-- )
    {
        co = addcol_mh( menu, i, opts[ i ], NULL );
        setcolorcol_mh( co, COLOR_FOCUS_MH, NBLK, NWHT, 2, NBRIG, NBLKS );
        setattrcol_mh( co, 10, J_CEN, MINUSC );
    }

    if( !optdefault ) setfocus_mh( menu, 0 );

    init_mh( menu );

    ret = exec_mh( menu );

    setcolor_off();

    free_mh( menu );

    return ret;
}

//---------------------------------------------------------------------------------------

bool valdt_n( uint8_t day, uint8_t mon, uint32_t year )
{
    uint8_t days_mon[ 12 ] = { 31, 29, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };
    uint8_t tot_days_mon;
    struct tm t = { 0 };

    if( year >= 0 && year <= 9999 )
    {
        if( mon >= 1 && mon <= 12 )
        {
            tot_days_mon = days_mon[ mon - 1 ];

            if( day >= 1 && day <= tot_days_mon )
            {
                t.tm_mday = day;
                t.tm_mon = mon - 1;
                t.tm_year = year - 1900;

                mktime( &t );

                return( t.tm_mday == day ? true : false );
            }
        }
    }

    return false;
}

time_t date_segs( uint8_t day, uint8_t mon, uint32_t year )
{
    struct tm *t;
    time_t segs = time( NULL );

    if( !valdt_n( day, mon, year ) ) return 0;

    t = localtime( &segs );

    t->tm_mday = day;
    t->tm_mon = mon - 1;
    t->tm_year = year - 1900;
    t->tm_sec = 0;
    t->tm_min = 0;
    t->tm_hour = 7;
    t->tm_isdst = 0;

    return( mktime( t ) );    
}

time_t now_segs() // data de hoje as 7:00 hs
{
    struct tm *t;
    time_t segs = time( NULL );

    t = localtime( &segs );

    t->tm_sec = 0;
    t->tm_min = 0;
    t->tm_hour = 7;
    t->tm_isdst = 0;

    return( mktime( t ) );
}

int16_t decomp_date( time_t segs, uint8_t n ) // retorna se n = 1 dia, n = 2 mes, n = 3 ano, n = 4 dia da semana
{
    struct tm *t;
    int16_t r;
    
    t = localtime( &segs );

    switch( n )
    {
        case 1:
            r = t->tm_mday;
            break;
        case 2:
            r = t->tm_mon + 1; // 1 a 12
            break;
        case 3:
            r = t->tm_year + 1900;
            break;
        case 4:
            r = t->tm_wday + 1; // 1 a 7
            break;
        default:
            r = -1;    
    }

    return r;
}

int16_t input_year( uint8_t lin, uint8_t col, uint16_t winsize )
{
    time_t hoje = now_segs();
    int16_t ano_ini = decomp_date( hoje, 3 ) + 1;
    int16_t ano;
    int i;

    BR *menu = NULL;
    LN *li = NULL;
    CO *co = NULL;
    TB *tb = NULL;
    SB *sb = NULL;
    wchar_t *statusbar1[] = { L"[ESC]", L"[HOME]", L"[END]" };
    wchar_t *statusbar2[] = { L"Sai", L"Inicio", L"Fim" };

    menu = create_bw();

    setbox_bw( menu, BOX_MENU_BW );
    setcolor_bw( menu, COLOR_FOCUS_BW, NLYEL, 238, 1, NNORM );
    setcoord_bw( menu, lin, col, winsize );
    setctrlpercent_bw( menu, false );
    /* setdatatypekey_bw( menu, sizeof( int ) ); */
    setfunckey_bw( menu, cmpdecreaseintline_bw );

    for( uint8_t inc = 0; inc < 3; inc++ )
    {
        sb = addstatusbar_bw( menu, statusbar1[ inc ] );
        setcolorstatusbar_bw( sb, NGRY, NBLK, 2, NNORM, NREVE );
        
        sb = addstatusbar_bw( menu, statusbar2[ inc ] );
        setcolorstatusbar_bw( sb, NLCYN, NWHT, 3, NNORM, NITAL, NREVE );
    
        if( inc < 2 ) addstatusbar_bw( menu, L" " );
    }

    tb = addtitlebar_bw( menu, L" Ano " );
    setcolortitlebar_bw( tb, NGRY, NBLK, 2, NBRIG, NREVE );

    for( uint8_t inc = 0; inc < ANOS; inc++ )
    {
        i = ano_ini - inc;

        li = addline_bw( menu, NULL, i, f_void );

        co = addnewcol_bw( menu, li, &i, DINT );
        setattrcol_bw( co, 7, 0, J_CEN, false );
        setcolorcol_bw( co, NGRY, NBLK, 1, NNORM );
    }

    init_bw( menu );

    ano = exec_bw( menu );

    free_bw( menu );
    
    return ano;
}

int8_t input_mon( uint8_t lin, uint8_t col, uint16_t winsize )
{
    int8_t m;
    int i;

    BR *menu = NULL;
    LN *li = NULL;
    CO *co = NULL;
    TB *tb = NULL;
    SB *sb = NULL;
    wchar_t *statusbar1[] = { L"[ESC]", L"[HOME]", L"[END]" };
    wchar_t *statusbar2[] = { L"Sai", L"Inicio", L"Fim" };

    wchar_t *mes[ 12 ] = { L"Janeiro",
                           L"Fevereiro",
                           L"Março",
                           L"Abril",
                           L"Maio",
                           L"Junho",
                           L"Julho",
                           L"Agosto",
                           L"Setembro",
                           L"Outubro",
                           L"Novembro",
                           L"Dezembro" }; 

    menu = create_bw();

    setbox_bw( menu, BOX_MENU_BW );
    setcolor_bw( menu, COLOR_FOCUS_BW, NLYEL, 238, 1, NNORM );
    setcoord_bw( menu, lin, col, winsize );
    setctrlpercent_bw( menu, false );

    for( uint8_t inc = 0; inc < 3; inc++ )
    {
        sb = addstatusbar_bw( menu, statusbar1[ inc ] );
        setcolorstatusbar_bw( sb, NGRY, NBLK, 2, NNORM, NREVE );
        
        sb = addstatusbar_bw( menu, statusbar2[ inc ] );
        setcolorstatusbar_bw( sb, NLCYN, NWHT, 3, NNORM, NITAL, NREVE );
    
        if( inc < 2 ) addstatusbar_bw( menu, L" " );
    }

    tb = addtitlebar_bw( menu, L" Mes " );
    setcolortitlebar_bw( tb, NGRY, NBLK, 2, NBRIG, NREVE );

    for( uint8_t inc = 0; inc < 12; inc++ )
    {
        i = inc + 1;
        li = addline_bw( menu, NULL, i, f_void );

        co = addnewcol_bw( menu, li, mes[ inc ], DWCHAR );
        setattrcol_bw( co, wcslen( mes[ inc ] ), 0, J_ESQ, false );
        setcolorcol_bw( co, NGRY, NBLK, 1, NNORM );
    }

    init_bw( menu );

    m = exec_bw( menu );

    free_bw( menu );
    
    return m;    
}

int8_t input_day( uint8_t lin, uint8_t col, uint8_t mon, uint16_t year, uint16_t winsize )
{
    int8_t d;
    uint8_t tdia;
    int dia;

    BR *menu = NULL;
    LN *li = NULL;
    CO *co = NULL;
    TB *tb = NULL;
    SB *sb = NULL;
    wchar_t *statusbar1[] = { L"[ESC]", L"[HOME]", L"[END]" };
    wchar_t *statusbar2[] = { L"Sai", L"Inicio", L"Fim" };

    uint8_t dias_mes[ 12 ] = { 31, 29, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };

    if( mon < 1 || mon > 12 ) return 0;
    
    tdia = mon == 2 ? ( valdt_n( 29, 2, year ) ? 29 : 28 ) : dias_mes[ mon - 1 ];

    menu = create_bw();

    setbox_bw( menu, BOX_MENU_BW );
    setcolor_bw( menu, COLOR_FOCUS_BW, NLYEL, 238, 1, NNORM );
    setcoord_bw( menu, lin, col, winsize );
    setctrlpercent_bw( menu, false );

    for( uint8_t inc = 0; inc < 3; inc++ )
    {
        sb = addstatusbar_bw( menu, statusbar1[ inc ] );
        setcolorstatusbar_bw( sb, NGRY, NBLK, 2, NNORM, NREVE );
        
        sb = addstatusbar_bw( menu, statusbar2[ inc ] );
        setcolorstatusbar_bw( sb, NLCYN, NWHT, 3, NNORM, NITAL, NREVE );
    
        if( inc < 2 ) addstatusbar_bw( menu, L" " );
    }

    tb = addtitlebar_bw( menu, L" Dia " );
    setcolortitlebar_bw( tb, NGRY, NBLK, 2, NBRIG, NREVE );

    for( uint8_t inc = 0; inc < tdia; inc++ )
    {
        dia = inc + 1;
        li = addline_bw( menu, NULL, dia, f_void );

        co = addnewcol_bw( menu, li, &dia, DINT );
        setattrcol_bw( co, 3, 0, J_DIR, false );
        setcolorcol_bw( co, NGRY, NBLK, 1, NNORM );
    }
   
    init_bw( menu );

    d = exec_bw( menu );

    free_bw( menu );

    return d;
}

time_t input_date( wchar_t *msg, uint8_t lin, uint8_t col )
{
    int8_t dia = -1;
    int8_t mes = -1;
    int16_t ano = -1;
    size_t t_msg;
    wchar_t buf_msg[ 128 ];
    wchar_t buffer[ 128 ];
    time_t ret = 0;

    wcsncpy( buf_msg, msg, 64 );

    t_msg = wcslen( buf_msg );

    if( !t_msg )
    {
        wcscpy( buf_msg, L"Data: " );
        t_msg = wcslen( buf_msg );
    }

    wcscpy( buffer, L"   ,   /  /    " );

    cleanarea( lin, col, lin, col + t_msg + 16 );

    setcolor_on( NBLU, NBLK, 1, NBRIG );
    imp_wstr( buf_msg, t_msg, J_ESQ, MINUSC );

    setcurs( lin, col + t_msg );

    setcolor_on( NLYEL, COR_INPUT, 1, NBRIG );
    imp_wstr( buffer, 15, J_ESQ, MINUSC );

    setcolor_off();

    while( true )
    {
        ano = input_year( lin + 2, col, 10 );

        if( ano == -1 ) break;

        setcurs( lin, col + t_msg + 11 );
        setcolor_on( NLYEL, COR_INPUT, 1, NBRIG );
        printf( "%04d", ano );
        setcolor_off();

        mes = input_mon( lin + 2, col, 6 );

        if( mes == -1 ) break;

        setcurs( lin, col + t_msg + 8 );
        setcolor_on( NLYEL, COR_INPUT, 1, NBRIG );
        printf( "%02d", mes );
        setcolor_off();

        dia = input_day( lin + 2, col, mes, ano, 16 );

        if( dia != -1 )
        {
            setcurs( lin, col + t_msg + 5 );
            setcolor_on( NLYEL, COR_INPUT, 1, NBRIG );
            printf( "%02d", dia );
            setcolor_off();

            setcurs( lin, col + t_msg );
            setcolor_on( NLYEL, COR_INPUT, 1, NBRIG );
            imp_wstr( dweek_to_wstr( date_segs( dia, mes, ano ), buffer ), 3, J_ESQ, MINUSC );
            setcolor_off();

            if( hdialog_button( L"Continua?", CONFIRM_DEFAULT_Y ) == 1 )
            {
                cleanarea( lin, col, lin, col + t_msg + 16 );

                setcolor_on( NBLU, NBLK, 1, NBRIG );
                imp_wstr( buf_msg, t_msg, J_ESQ, MINUSC );

                setcurs( lin, col + t_msg );
                imp_colordate( date_segs( dia, mes, ano ), 15, NWHT, NBLK, 1, NNORM );

                ret = date_segs( dia, mes, ano );
            }
        }

        break;
    }
    
    return ret;
}

int16_t input_daterange( uint8_t lin, uint8_t col, time_t *segs_ini, time_t *segs_fin )
{
    int8_t daterange;
    int16_t mon;
    int16_t year;
    time_t segs_now;

    BR *menu = NULL;
    LN *li = NULL;
    CO *co = NULL;
    TB *tb = NULL;
    SB *sb = NULL;
    wchar_t *statusbar1[] = { L"[ESC]", L"[HOME]", L"[END]" };
    wchar_t *statusbar2[] = { L"Sai", L"Inicio", L"Fim" };
    wchar_t *opts[] = { L"Todas", L"Uma data", L"Um mês", L"Um periodo", L"Vencidas" };
    wchar_t buffer[ 64 ];

    segs_now = now_segs();

    menu = create_bw();

    setbox_bw( menu, BOX_MENU_BW );
    setcolor_bw( menu, COLOR_FOCUS_BW, NWHT, 238, 1, NNORM );
    setcoord_bw( menu, lin, col, 5 );
    setctrlpercent_bw( menu, false );
    /* setdatatypekey_bw( menu, sizeof( int ) ); */
    /* setfunckey_bw( menu, cmpintline_bw ); */

    tb = addtitlebar_bw( menu, L" Selecione periodo " );
    setcolortitlebar_bw( tb, NGRY, NBLK, 2, NBRIG, NREVE );

    for( uint8_t inc = 0; inc < 3; inc++ )
    {
        sb = addstatusbar_bw( menu, statusbar1[ inc ] );
        setcolorstatusbar_bw( sb, NGRY, NBLK, 2, NNORM, NREVE );
        
        sb = addstatusbar_bw( menu, statusbar2[ inc ] );
        setcolorstatusbar_bw( sb, NLCYN, NWHT, 3, NNORM, NITAL, NREVE );
    
        if( inc < 2 ) addstatusbar_bw( menu, L" " );
    }

    for( uint8_t inc = 0; inc < 5; inc++ )
    {
        /* li = addline_bw( menu, &inc, inc + 1, f[ inc ] ); */
        li = addline_bw( menu, NULL, inc, f_void );

        co = addnewcol_bw( menu, li, opts[ inc ], DWCHAR );
        setattrcol_bw( co, wcslen( opts[ inc ] ), 0, J_ESQ, false );
        setcolorcol_bw( co, NGRY, NBLK, 1, NNORM );
    }

    init_bw( menu );

    daterange = exec_bw( menu );

    switch( daterange )
	{
        case 0:
            break;    
        case 1:
            *segs_ini = input_date( L"Data: ", lin, col );

            if( !*segs_ini )
            {
                daterange = -1;
                break;
            }

            *segs_fin = *segs_ini;

            break;
        case 2:
            if( !input_monyear( lin, col, segs_ini, segs_fin ) )
            {
                daterange = -1;
                break;
            }

            mon = decomp_date( *segs_ini, 2 );
            year = decomp_date( *segs_ini, 3 );

            break;
	    case 3:
            *segs_ini = input_date( L"Data 1: ", lin, col );

            if( !*segs_ini )
            {
                daterange = -1;
                break;
            }
           
            *segs_fin = input_date( L"Data 2: ", lin + 1, col );

            if( !*segs_fin )
            {
                daterange = -1;
                break;
            }

			break;
        case 4:
            *segs_ini = date_segs( 1, 1, 1970 );
            *segs_fin = segs_now;
	}

    setcurs( lin + 1, col );

    if( daterange != -1 )
    {
        cleanarea( lin, col, lin + 1, col + 24 );
        setcurs( lin, col );
        setcolor_on( NBLU, NBLK, 1, NBRIG );
        wcscpy( buffer, (  wchar_t * ) getdatacol_bw( menu, daterange, 0 ) );
        imp_wstr( buffer, wcslen( buffer ), 1, 0 );
        setcolor_off();

        if( daterange == 1 )
        {
            printf( " -> " );
            imp_colordate( *segs_ini, 15, NWHT, NBLK, 1, NNORM );
        }

        if( daterange == 2 ) printf( " -> %02d/%04d", mon, year );

        if( daterange == 3 )
        {
            printf( ":  " );
            imp_colordate( *segs_ini, 15, NWHT, NBLK, 1, NNORM );
            printf( " a " );
            imp_colordate( *segs_fin, 15, NWHT, NBLK, 1, NNORM );
        }
    }
    
    free_bw( menu );
    
    return daterange;
}

bool input_monyear( uint8_t lin, uint8_t col, time_t *segs_ini, time_t *segs_fin )
{
    int16_t year;
    int8_t mon;
    uint8_t tday;
    uint8_t days_mon[ 12 ] = { 31, 29, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };
    int16_t ret;

    cleanarea( lin, col, lin, col + 16 );

    setcolor_on( NBLU, NBLK, 1, NBRIG );
    imp_wstr( L"Mes/Ano: ", 9, J_ESQ, MINUSC );
    
    setcurs( lin, col + 9 );

    setcolor_on( NLYEL, COR_INPUT, 1, NBRIG );
    imp_wstr( L"  /    ", 7, J_ESQ, MINUSC );

    setcolor_off();

    year = input_year( lin + 2, col, 8 );

    if( year == -1 ) return 0;

    setcurs( lin, col + 12 );
    setcolor_on( NLYEL, COR_INPUT, 1, NBRIG );
    printf( "%04d", year );
    setcolor_off();

    mon = input_mon( lin + 2, col, 8 );

    if( mon == -1 ) return 0;

    tday = mon == 2 ? ( valdt_n( 29, 2, year ) ? 29 : 28 ) : days_mon[ mon - 1 ];

    setcurs( lin, col + 9 );
    setcolor_on( NLYEL, COR_INPUT, 1, NBRIG );
    printf( "%02d", mon );
    setcolor_off();

    *segs_ini = date_segs( 1, mon, year );
    *segs_fin = date_segs( tday, mon, year );

    ret = hdialog_button( L"Continua?", CONFIRM_DEFAULT_Y );

    cleanarea( lin, col, lin, col + 16 );

    return ret == -1 ? 0 : ret;
}

//---------------------------------------------------------------------------------------

int num_regs( FILE *arq, long int t_struct )
{
    long int t_arq = tam_arq( arq );

    return t_arq > 0 ? t_arq / t_struct : 0;
}

long int tam_arq( FILE *arq )
{
    long int tam;

    fseek( arq, 0, SEEK_END );

    tam = ftell( arq );

    rewind( arq );

    return tam;
}
