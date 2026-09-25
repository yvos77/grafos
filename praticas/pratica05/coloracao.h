#ifndef COLORACAO_H
#define COLORACAO_H

#include "grafo_lista.h"

int *coloracao_gulosa(GrafoLista *g, int *num_cores);
int *coloracao_welsh_powell(GrafoLista *g, int *num_cores);
int eh_bipartido(GrafoLista *g);
int coloracao_valida(GrafoLista *g, int *cor);

#endif
