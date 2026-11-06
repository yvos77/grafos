#ifndef BELLMAN_FORD_H
#define BELLMAN_FORD_H

#include <limits.h>

#define INFINITO INT_MAX

typedef struct NoPonderado {
    int destino;
    int peso;
    struct NoPonderado *prox;
} NoPonderado;

typedef struct {
    int n;
    NoPonderado **adj;
} GrafoListaPonderado;

GrafoListaPonderado *criar_grafo_ponderado(int n);
void inserir_arco_ponderado(GrafoListaPonderado *g, int u, int v, int peso);
void liberar_grafo_ponderado(GrafoListaPonderado *g);

int bellman_ford(GrafoListaPonderado *g, int origem, int *dist, int *pred);
int reconstruir_ciclo_negativo(GrafoListaPonderado *g, int *dist, int *pred, int *ciclo);

#endif
