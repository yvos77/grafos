#ifndef CONECTIVIDADE_H
#define CONECTIVIDADE_H

#include "grafo_lista.h"

typedef struct {
    int u, v;
} Aresta;

typedef struct {
    int *descoberta;
    int *low;
    int *eh_articulacao;
    Aresta *pontes;
    int num_pontes;
    int tempo;
} Tarjan;

void dfs_articulacoes(GrafoLista *g, int u, int pai, Tarjan *t);
int encontrar_articulacoes(GrafoLista *g, int *eh_articulacao);
int detectar_pontes(GrafoLista *g, Aresta *pontes);

#endif
