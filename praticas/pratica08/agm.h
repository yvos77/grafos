#ifndef AGM_H
#define AGM_H

#include <limits.h>

#define INFINITO INT_MAX

typedef struct {
    int *pai, *rank;
    int n;
} DSU;

typedef struct {
    int u, v, peso;
} Aresta;

typedef struct NoPonderado {
    int destino;
    int peso;
    struct NoPonderado *prox;
} NoPonderado;

typedef struct {
    int n;
    NoPonderado **adj;
} GrafoListaPonderado;

DSU *criar_dsu(int n);
int find(DSU *d, int x);
int unir(DSU *d, int x, int y);
void liberar_dsu(DSU *d);

GrafoListaPonderado *criar_grafo_ponderado(int n);
void inserir_aresta_ponderada(GrafoListaPonderado *g, int u, int v, int peso);
void liberar_grafo_ponderado(GrafoListaPonderado *g);

int kruskal(int n, Aresta *arestas, int m, Aresta *agm, int *peso_total);
int prim(GrafoListaPonderado *g, int origem, Aresta *agm, int *peso_total);

#endif
