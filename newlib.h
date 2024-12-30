#ifndef _NEWLIB_H
#define _NEWLIB_H

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <termios.h>
#include <sys/ioctl.h>
#include <string.h>
#include <time.h>
#include <ctype.h>
#include <wchar.h>
#include <errno.h>
#include <math.h>
#include <wctype.h>
#include <locale.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stdint.h>
#include "colors.h"

#define BOX       3
#define BOX_FORM  4

#define J_NAT   0
#define J_ESQ   1 
#define J_CEN   2
#define J_DIR   3

#define MINUSC  0
#define MAIUSC  1

#define ECHO_ON   1
#define ECHO_OFF  0

#define CONFIRM_DEFAULT_Y  1
#define CONFIRM_DEFAULT_N  0

#define RETMSG_ERRO     1
#define RETMSG_NO_ERRO  0

#define ENTMODO_CAD   0
#define ENTMODO_EDIT  1

#define LIN_FORM     1 
#define COL_FORM     1
#define LIND_FORM   42
#define COLD_FORM  130

#define LIN_FORM_APP   LIN_FORM  + 3
#define COL_FORM_APP   COL_FORM  + 1
#define LIND_FORM_APP  LIND_FORM - 3
#define COLD_FORM_APP  COLD_FORM - 1
#define LIN_FORM_TIT   LIN_FORM  + 1
#define COL_FORM_TIT   COL_FORM  + 1
#define LIN_FORM_ROD   LIND_FORM - 1
#define COL_FORM_ROD   COL_FORM  + 1

#define ANO_LIMITE     2022
#define ANOS            200 
#define COR_INPUT       240
#define COR_FUNDO_TIT   238
#define COR_FUNDO_ROD   238

void setcurs( uint8_t lin, uint8_t col );
void curs_up( uint8_t n );
void curs_down( uint8_t n );
void curs_forward( uint8_t n );
void curs_back( uint8_t n );
void curs_col( uint8_t n );
void curs_save( void );
void curs_rest( void );
void curs_show( void );
void curs_hide( void );
void line_down( uint8_t n );
void line_up( uint8_t n );
void cls_line( uint8_t n );
void cls_display( uint8_t n );
void cls_scr(); // limpa tela
void cls_area( uint8_t lin, uint8_t col );

void setup_console_new( bool echo );
void restore_console_new( void );
wint_t get_ch( bool echo );

void box( uint8_t type, uint8_t lin, uint8_t col, uint8_t blin, uint8_t rcol );
void setcolor_on( uint8_t colorc, uint8_t colorb, int numpars, ... );
void setcolor_aon( uint8_t colorc, uint8_t colorb, uint8_t colora[] );
void setcolor_off();
void cleanarea( uint8_t lin, uint8_t col, uint8_t blin, uint8_t rcol );
void ret_wmsg_form( wchar_t *text, bool error );

void disp_get( uint8_t lin, uint8_t col, wchar_t *text, uint16_t size );
bool filter_input_num( wint_t c );
bool filter_input_doub( wint_t c );
bool filter_input_no( wint_t c );
int input_wstr( uint8_t lin, uint8_t col, wchar_t *text, uint16_t size, bool mode, bool ( *ffi ) ( wint_t ) );
int wstr_to_int( const wchar_t *str, int *num );
int wstr_to_doub( const wchar_t *str, double *num, int precision );
int input_wint( uint8_t lin, uint8_t col, int *n, uint16_t size, bool mode );
int input_wdouble( uint8_t lin, uint8_t col, double *n, uint16_t size, uint8_t precision, bool mode );

void reverse_str( char *str );
void reverse_wstr( wchar_t *str );
void alltrim( char *str );
void allwtrim( wchar_t *str );
char *padl( char *dest, char *orig, uint16_t padlen );
wchar_t *padwl( wchar_t *dest, wchar_t *orig, uint16_t padlen );
wchar_t *doub_to_wstr( double num, uint8_t precision, wchar_t *str );
wchar_t *int_to_wstr( int num, uint8_t precision, wchar_t *str );
wchar_t *date_to_wstr( time_t segs, wchar_t *str );
wchar_t *datehours_to_wstr( time_t segs, wchar_t *str );
wchar_t *dweek_to_wstr( time_t segs, wchar_t *str );

void imp_str( char *str, uint16_t size, uint8_t align, bool upperc );
void imp_wstr( wchar_t *str, uint16_t size, uint8_t align, bool upperc );
void imp_wint( int num, uint16_t size, uint8_t precision, uint8_t align );
void imp_wdoub( double num, uint16_t size, uint8_t precision, uint8_t align );
void imp_date( time_t segs, uint16_t size, uint8_t align, bool upperc );
void imp_colordate( time_t segs, uint16_t size, uint8_t colorc, uint8_t colorb, int numpars, ... );

void init_form( wchar_t *title );
int8_t hdialog_button( wchar_t *text, bool optdefault );

bool valdt_n( uint8_t day, uint8_t mon, uint32_t year );
time_t date_segs( uint8_t day, uint8_t mon, uint32_t year );
time_t now_segs(); // data de hoje as 7:00 hs;
int16_t decomp_date( time_t segs, uint8_t n ); // retorna se n = 1 dia, n = 2 mes, n = 3 ano, n = 4 dia da semana
int16_t input_year( uint8_t lin, uint8_t col, uint16_t winsize );
int8_t input_mon( uint8_t lin, uint8_t col, uint16_t winsize );
int8_t input_day( uint8_t lin, uint8_t col, uint8_t mon, uint16_t year, uint16_t winsize );
time_t input_date( wchar_t *msg, uint8_t lin, uint8_t col );
int16_t input_daterange( uint8_t lin, uint8_t col, time_t *segs_ini, time_t *segs_fin );
bool input_monyear( uint8_t lin, uint8_t col, time_t *segs_ini, time_t *segs_fin );

int num_regs( FILE *arq, long int t_struct );
long int tam_arq( FILE *arq );

#endif
