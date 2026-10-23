#ifndef GRAFO_MATRIZ_H
#define GRAFO_MATRIZ_H

typedef struct {
    int n;
    int **adj;
} GrafoMatriz;

GrafoMatriz *criar_grafo_matriz(int n);
int inserir_aresta_matriz(GrafoMatriz *g, int u, int v);
int remover_aresta_matriz(GrafoMatriz *g, int u, int v);
int grau_matriz(GrafoMatriz *g, int v);
int sao_adjacentes_matriz(GrafoMatriz *g, int u, int v);
void imprimir_grafo_matriz(GrafoMatriz *g);
void liberar_grafo_matriz(GrafoMatriz *g);

#endif