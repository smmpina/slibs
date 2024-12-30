#ifndef _ORDENACAO_H
#define _ORDENACAO_H

#include <stdio.h>

struct controle // para rotina de ordenacao externa
{
    int idctrl; // numero controle
    int idarq; // numero do arquivo temporario
    int bufpos; // posicao relativa do buffer
    int bufini; // posicao inicial do buffer
    int buffin; // posicao final do buffer
    int arqmaxregs; // numero de registros do arquivo temporario
    FILE *arq; // ponteiro para o arquivo temporario
    void *dados; // ponteiro para o buffer de dados
};

int ord_arq_ext( char *nome_orig, char *nome_orde, int t_mem, long int t_struct,
                 int ( *f_cmp ) ( const void *, const void * ) );
int ger_tmps( FILE *fp, int t_buffer, long int t_struct,
              int ( *f_cmp ) ( const void *, const void * ) );
int ordena_mem( FILE *arq_orig, FILE *arq_dest, int t_buffer, long int t_struct,
                int ( *f_cmp ) ( const void *, const void * ) );
int copia_arqbin( char *nome_orig, char *nome_dest, long int t_struct );

#endif
