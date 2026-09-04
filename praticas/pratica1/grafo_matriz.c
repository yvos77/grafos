#include <stdio.h>
#include <stdlib.h>
#include "grafo_matriz.h"

static int vertice_valido(GrafoMatriz *g, int v) {
    return g != NULL && v >= 0 && v < g->n;
}

GrafoMatriz *criar_grafo_matriz(int n) {
    if (n <= 0) return NULL;

    GrafoMatriz *g = malloc(sizeof(GrafoMatriz));
    if (g == NULL) return NULL;

    g->n = n;
    g->adj = malloc(n * sizeof(int *));
    if (g->adj == NULL) {
        free(g);
        return NULL;
    }

    for (int i = 0; i < n; i++) {
        g->adj[i] = calloc(n, sizeof(int));
        if (g->adj[i] == NULL) {
            for (int j = 0; j < i; j++) free(g->adj[j]);
            free(g->adj);
            free(g);
            return NULL;
        }
    }

    return g;
}

int inserir_aresta_matriz(GrafoMatriz *g, int u, int v) {
    if (!vertice_valido(g, u) || !vertice_valido(g, v)) return 0;
    if (u == v) return 0;
    if (g->adj[u][v] == 1) return 0;

    g->adj[u][v] = 1;
    g->adj[v][u] = 1;
    return 1;
}

int remover_aresta_matriz(GrafoMatriz *g, int u, int v) {
    if (!vertice_valido(g, u) || !vertice_valido(g, v)) return 0;
    if (g->adj[u][v] == 0) return 0;

    g->adj[u][v] = 0;
    g->adj[v][u] = 0;
    return 1;
}

int grau_matriz(GrafoMatriz *g, int v) {
    if (!vertice_valido(g, v)) return -1;

    int grau = 0;
    for (int i = 0; i < g->n; i++) grau += g->adj[v][i];
    return grau;
}

int sao_adjacentes_matriz(GrafoMatriz *g, int u, int v) {
    if (!vertice_valido(g, u) || !vertice_valido(g, v)) return 0;
    return g->adj[u][v] == 1;
}

void imprimir_grafo_matriz(GrafoMatriz *g) {
    if (g == NULL) return;

    printf("    ");
    for (int i = 0; i < g->n; i++) printf("%3d", i);
    printf("\n");

    for (int i = 0; i < g->n; i++) {
        printf("%3d ", i);
        for (int j = 0; j < g->n; j++) printf("%3d", g->adj[i][j]);
        printf("\n");
    }
}

void liberar_grafo_matriz(GrafoMatriz *g) {
    if (g == NULL) return;

    for (int i = 0; i < g->n; i++) free(g->adj[i]);
    free(g->adj);
    free(g);
}