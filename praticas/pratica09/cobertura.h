#ifndef COBERTURA_H
#define COBERTURA_H

typedef struct {
    int n;
    int **adj;
} GrafoMatriz;

GrafoMatriz *criar_grafo_matriz(int n);
void inserir_aresta_matriz(GrafoMatriz *g, int u, int v);
int contar_arestas(GrafoMatriz *g);
void liberar_grafo_matriz(GrafoMatriz *g);

int eh_cobertura(GrafoMatriz *g, int *na_cobertura);
int cobertura_gulosa(GrafoMatriz *g, int *na_cobertura);
int cobertura_minima_bt(GrafoMatriz *g, int *na_cobertura, long long *nos_explorados);

#endif
