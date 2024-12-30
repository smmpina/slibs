#include "newlib.h"
#include "ordenacao.h"

int ord_arq_ext( char *nome_orig, char *nome_orde, int t_mem, long int t_struct,
                 int ( *f_cmp ) ( const void *, const void * ) )
{
    FILE *arq_orig;
    FILE *arq_ord;
    struct controle *cte = NULL;
    struct controle aux;
    struct controle tmp;
    void *buf_saida = NULL;
    size_t r;
    int inc;
    int i;
    int count;
    int n_regs;
    int num_arqs;
    int t_buf;
    int ret;
    char nome_tmp[ 64 ];
    
    arq_orig = fopen( nome_orig, "rb+" );

    if( !arq_orig ) return 0;

    n_regs = num_regs( arq_orig, t_struct );

    if( n_regs < 2 )
    {
        fclose( arq_orig );
        return 1;
    } 
	
	arq_ord = fopen( nome_orde, "wb+" );
	
    if( !arq_ord )
	{
        fclose( arq_orig );
		return 0;
	}	
    
    if( n_regs <= t_mem )
    {
        ret = ordena_mem( arq_orig, arq_ord, n_regs, t_struct, f_cmp );
        
        fclose( arq_orig );
        fclose( arq_ord );
        
        return ret;
    }

    num_arqs = ger_tmps( arq_orig, t_mem, t_struct, f_cmp );

    if( !num_arqs )
    {
        fclose( arq_orig );
        fclose( arq_ord );
        return 0;
    }

    t_buf = t_mem / ( num_arqs + 1 );

    if( t_buf < 1 )
    {
        fclose( arq_orig );
        fclose( arq_ord );
        return 0;
    }

    cte = ( struct controle * ) malloc( sizeof( struct controle ) * num_arqs );

    if( !cte )
    {
        fclose( arq_orig );
        fclose( arq_ord );
        return 0;
    }
    
    for( inc = 0; inc < num_arqs; inc++ )
    {
        cte[ inc ].dados = NULL;
        cte[ inc ].arq = NULL;
        cte[ inc ].idctrl = inc;
        cte[ inc ].idarq = inc + 1;
        cte[ inc ].bufpos = 0;
        cte[ inc ].bufini = 0;
        cte[ inc ].buffin = t_buf;
        cte[ inc ].dados = malloc( t_struct * t_buf );
        
        /* sprintf( nome_tmp, "temp%d.dat", inc + 1 ); */
        snprintf( nome_tmp, sizeof( nome_tmp ), "temp%d.dat", inc + 1 );

        cte[ inc ].arq = fopen( nome_tmp, "rb+" );
        
        if( !cte[ inc ].arq || !cte[ inc ].dados )
        {
            for( i = 0; i <= inc; i++ )
            {
                if( cte[ i ].arq ) fclose( cte[ i ].arq );
                if( cte[ i ].dados ) free( cte[ i ].dados );
            }

            free( cte );
            fclose( arq_orig );
            fclose( arq_ord );
            return 0;
        }

        cte[ inc ].arqmaxregs = num_regs( cte[ inc ].arq, t_struct );

        fseek( cte[ inc ].arq, cte[ inc ].bufpos * t_struct, SEEK_SET );

        r = fread( cte[ inc ].dados, t_struct, t_buf, cte[ inc ].arq );

        cte[ inc ].buffin = r;
    }

    buf_saida =  malloc( t_struct * t_buf );

    if( !buf_saida )
    {
        for( inc = 0; inc < num_arqs; inc++ ) free( cte[ inc ].dados );
        free( cte );
        fclose( arq_orig );
        fclose( arq_ord );
        return 0;
    }

    count = 0;

    while( 1 )    
    {
        aux = cte[ 0 ];

        for( inc = 0; inc < num_arqs; inc++ )
        {
            tmp = cte[ inc ];

            if( f_cmp( aux.dados + ( aux.bufini * t_struct ), tmp.dados + ( tmp.bufini * t_struct ) ) > 0 )
                aux = tmp;
        }

        memcpy( buf_saida + ( count * t_struct ), aux.dados + ( aux.bufini * t_struct ), t_struct );

        count++;

        if( count == t_buf )
        {
            fseek( arq_ord, 0, SEEK_END );
            fwrite( buf_saida, t_struct, count, arq_ord );
            count = 0;
        }

        cte[ aux.idctrl ].bufini++;
        cte[ aux.idctrl ].bufpos++;
        
        if( cte[ aux.idctrl ].bufini == cte[ aux.idctrl ].buffin )
        {
            cte[ aux.idctrl ].bufini = 0;

            fseek( cte[ aux.idctrl ].arq, cte[ aux.idctrl ].bufpos * t_struct, SEEK_SET );

            r = fread( cte[ aux.idctrl ].dados, t_struct, t_buf, cte[ aux.idctrl ].arq );

            cte[ aux.idctrl ].buffin = r;
        }

        if( cte[ aux.idctrl ].bufpos == cte[ aux.idctrl ].arqmaxregs )
        {
            fclose( cte[ aux.idctrl ].arq );

            sprintf( nome_tmp, "temp%d.dat", aux.idarq );

            remove( nome_tmp );

            free( cte[ aux.idctrl ].dados );

            if( num_arqs == 1 )
            {
                if( count )
                {
                    fseek( arq_ord, 0, SEEK_END );
                    fwrite( buf_saida, t_struct, count, arq_ord );
                }

                break;
            }

            cte[ aux.idctrl ].arq = NULL;

            for( inc = 0, i = 0; inc < num_arqs; inc++ )
            {
                while( !cte[ inc ].arq ) inc++;
                cte[ i ] = cte[ inc ];
                cte[ i ].idctrl = i;
                i++;
            }

            num_arqs--;
        }
    }

    free( cte );
    free( buf_saida );
    
    fclose( arq_orig );
    fclose( arq_ord );

    return 1;
}

int ger_tmps( FILE *fp, int t_buffer, long int t_struct, int ( *f_cmp ) ( const void *, const void * ) ) 
{
    FILE *tmp;
    int count = 0;
    char arq_tmp[ 64 ];
    void *buffer = NULL;
    size_t r;
    int tot_r = 0;
    int n_regs = num_regs( fp, t_struct );
    
    buffer =  malloc( t_struct * t_buffer + 1 );

    if( !buffer ) return 0;

    rewind( fp );

    while( true )
    {
        r = fread( buffer, t_struct, t_buffer, fp );

        tot_r += r;
        
        if( r )    
        {
            count++;

            qsort( buffer, r, t_struct, f_cmp );

            sprintf( arq_tmp, "temp%d.dat", count );

            tmp = fopen( arq_tmp, "wb+" );

            if( !tmp ) break;

            fseek( tmp, 0, SEEK_END );
            fwrite( buffer, t_struct, r, tmp );

            fclose( tmp );
        }
        else
            break;
    }

    free( buffer );

    if( tot_r != n_regs ) return 0;

    return count;
}

int ordena_mem( FILE *arq_orig, FILE *arq_dest, int t_buffer, long int t_struct,
                int ( *f_cmp ) ( const void *, const void * ) )
{
    void *buffer = NULL;
    size_t r;
    
    buffer = malloc( t_struct * t_buffer + 1 );
    
    if( !buffer ) return 0;
    
    rewind( arq_orig );

    r = fread( buffer, t_struct, t_buffer, arq_orig );
        
    if( r == t_buffer )
    {
        qsort( buffer, r, t_struct, f_cmp );

        fseek( arq_dest, 0, SEEK_END );
        fwrite( buffer, t_struct, r, arq_dest );
        
        free( buffer );
    }
    else
        return 0;

    return 1;
}

int copia_arqbin( char *nome_orig, char *nome_dest, long int t_struct )
{
    FILE *orig;
    FILE *dest;
    size_t rw;
    int n_regs;
    void *buffer = NULL;

    orig = fopen( nome_orig, "rb" );

    if( !orig ) return 0;

    n_regs = num_regs( orig, t_struct );

    if( !n_regs )
    {
        fclose( orig );
        return 0;
    }

    dest = fopen( nome_dest, "wb" );

    if( !dest )
    {
        fclose( orig );
        return 0;
    }

    rewind( orig );

    buffer = malloc( t_struct );
    
    if( !buffer )
    {
        fclose( orig );
        fclose( dest );
        return 0;
    }

    while( fread( buffer, t_struct, 1, orig ) )
    {
        fseek( dest, 0, SEEK_END );

        rw = fwrite( buffer, t_struct, 1, dest );

        if( rw != 1 )
        {
            fclose( orig );
            fclose( dest );
            free( buffer );
            remove( nome_dest );
            return 0;
        }
    }

    if( ferror( orig ) )
    {
        fclose( orig );
        fclose( dest );
        free( buffer );
        remove( nome_dest );
        return 0;
    }

    free( buffer );
    fclose( orig );
    fclose( dest );

    return 1;
}
