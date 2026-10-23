#ifndef BUSCA_PROFUNDIDADE_H
#define BUSCA_PROFUNDIDADE_H

#include "grafo_lista.h"

typedef struct {
    int *dados;
    int topo, capacidade;
} Pilha;

Pilha *criar_pilha(int capacidade);
void empilhar(Pilha *p, int x);
int desempilhar(Pilha *p);
int topo_pilha(Pilha *p);
int pilha_vazia(Pilha *p);
void liberar_pilha(Pilha *p);

void dfs_recursiva(GrafoLista *g, int u, int *visitado, int *entrada, int *saida, int *tempo);
int dfs_iterativa(GrafoLista *g, int origem, int *ordem);
int contar_componentes(GrafoLista *g);
int tem_ciclo(GrafoLista *g);

#endif
