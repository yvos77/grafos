#ifndef DIJKSTRA_H
#define DIJKSTRA_H

#include <limits.h>

#define INFINITO INT_MAX

typedef struct NoAdj {
    int destino;
    int peso;
    struct NoAdj *prox;
} NoAdj;

typedef struct {
    int n;
    NoAdj **adj;
} GrafoListaPonderado;

typedef struct {
    int vertice;
    int distancia;
} NoHeap;

typedef struct {
    NoHeap *dados;
    int capacidade, tamanho;
    int *posicao;
} MinHeap;

GrafoListaPonderado *criar_grafo_ponderado(int n);
int inserir_aresta_ponderada(GrafoListaPonderado *g, int u, int v, int peso);
void imprimir_grafo_ponderado(GrafoListaPonderado *g);
void liberar_grafo_ponderado(GrafoListaPonderado *g);

MinHeap *criar_min_heap(int capacidade);
int heap_vazio(MinHeap *h);
int esta_no_heap(MinHeap *h, int vertice);
int inserir_min_heap(MinHeap *h, int vertice, int distancia);
NoHeap extrair_minimo(MinHeap *h);
int diminuir_chave(MinHeap *h, int vertice, int nova_distancia);
void liberar_min_heap(MinHeap *h);

int dijkstra(GrafoListaPonderado *g, int origem, int *dist, int *pred);
int *reconstruir_caminho(int origem, int destino, int *pred);
void imprimir_caminho(int origem, int destino, int *dist, int *pred);
int exportar_dot(GrafoListaPonderado *g, int origem, int destino,
                 int *dist, int *pred, const char *arquivo);

#endif