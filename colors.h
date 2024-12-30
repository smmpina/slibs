#ifndef _COLORS_H
#define _COLORS_H

// Cores do caractere

#define BLK   "\033[38;5;0m" // preto 
#define RED   "\033[38;5;1m" // vermelho
#define GRN   "\033[38;5;2m" // verde
#define YEL   "\033[38;5;3m" // amarelo
#define BLU   "\033[38;5;4m" // azul
#define MAG   "\033[38;5;5m" // magenta
#define CYN   "\033[38;5;6m" // ciano
#define WHT   "\033[38;5;7m" // branco
#define GRY   "\033[38;5;8m" // cinza
#define LRED  "\033[38;5;9m" // vermelho claro
#define LGRN  "\033[38;5;10m" // verde claro
#define LYEL  "\033[38;5;11m" // amarelo claro
#define LBLU  "\033[38;5;12m" // azul claro
#define LMAG  "\033[38;5;13m" // magenta claro
#define LCYN  "\033[38;5;14m" // ciano claro
#define LWHT  "\033[38;5;15m" // branco claro

// Cores de fundo

#define BBLK   "\033[48;5;0m" // preto
#define BRED   "\033[48;5;1m" // vermelho
#define BGRN   "\033[48;5;2m" // verde
#define BYEL   "\033[48;5;3m" // amarelo
#define BBLU   "\033[48;5;4m" // azul
#define BMAG   "\033[48;5;5m" // magenta
#define BCYN   "\033[48;5;6m" // ciano
#define BWHT   "\033[48;5;7m" // branco
#define BGRY   "\033[48;5;8m" // cinza
#define BLRED  "\033[48;5;9m" // vermelho claro
#define BLGRN  "\033[48;5;10m" // verde claro
#define BLYEL  "\033[48;5;11m" // amarelo claro
#define BLBLU  "\033[48;5;12m" // azul claro
#define BLMAG  "\033[48;5;13m" // magenta claro
#define BLCYN  "\033[48;5;14m" // ciano claro
#define BLWHT  "\033[48;5;15m" // branco claro

// estilos

#define BRIG  "\033[1m" // brilhante
#define NORM  "\033[2m" // normal
#define ITAL  "\033[3m" // italico
#define UNDE  "\033[4m" // sublinhado
#define BLKS  "\033[5m" // pisca lento
#define BLKF  "\033[6m" // pisca rapido
#define REVE  "\033[7m" // reverso
#define HIDD  "\033[8m" // oculto
#define CROS  "\033[9m" // riscado

#define RESETC "\033[0m" // reseta as cores ( caractere e fundo )

#define SETFGC "\033[38;5;%hhum" // seta uma cor do caractere
#define SETBGC "\033[48;5;%hhum" // seta uma cor de fundo

#define NBLK    0 // preto 
#define NRED    1 // vermelho
#define NGRN    2 // verde
#define NYEL    3 // amarelo
#define NBLU    4 // azul
#define NMAG    5 // magenta
#define NCYN    6 // ciano
#define NWHT    7 // branco
#define NGRY    8 // cinza
#define NLRED   9 // vermelho claro
#define NLGRN  10 // verde claro
#define NLYEL  11 // amarelo claro
#define NLBLU  12 // azul claro
#define NLMAG  13 // magenta claro
#define NLCYN  14 // ciano claro
#define NLWHT  15 // branco claro

#define NBRIG  1 // brilhante
#define NNORM  2 // normal
#define NITAL  3 // italico
#define NUNDE  4 // sublinhado
#define NBLKS  5 // pisca lento
#define NBLKF  6 // pisca rapido
#define NREVE  7 // reverso
#define NHIDD  8 // oculto
#define NCROS  9 // riscado

#endif
