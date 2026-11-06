#include <stdlib.h>
#include "hamiltoniano.h"

GrafoMatriz *criar_grafo_matriz(int n) {
    GrafoMatriz *g = malloc(sizeof(GrafoMatriz));
    g->n = n;
    g->adj = malloc(n * sizeof(int *));
    for (int i = 0; i < n; i++)
        g->adj[i] = calloc(n, sizeof(int));
    return g;
}

void inserir_aresta_matriz(GrafoMatriz *g, int u, int v) {
    g->adj[u][v] = 1;
    g->adj[v][u] = 1;
}

void liberar_grafo_matriz(GrafoMatriz *g) {
    for (int i = 0; i < g->n; i++)
        free(g->adj[i]);
    free(g->adj);
    free(g);
}

static int estender(GrafoMatriz *g, int *caminho, int *usado, int pos, int fechar) {
    if (pos == g->n)
        return !fechar || g->adj[caminho[pos - 1]][caminho[0]];
    int u = caminho[pos - 1];
    for (int v = 0; v < g->n; v++) {
        if (!g->adj[u][v] || usado[v]) continue;
        usado[v] = 1;
        caminho[pos] = v;
        if (estender(g, caminho, usado, pos + 1, fechar))
            return 1;
        usado[v] = 0;
    }
    return 0;
}

static int buscar_a_partir_de(GrafoMatriz *g, int origem, int *caminho, int fechar) {
    int *usado = calloc(g->n, sizeof(int));
    caminho[0] = origem;
    usado[origem] = 1;
    int achou = estender(g, caminho, usado, 1, fechar);
    free(usado);
    return achou;
}

int encontrar_caminho_hamiltoniano_bt(GrafoMatriz *g, int *caminho) {
    for (int origem = 0; origem < g->n; origem++)
        if (buscar_a_partir_de(g, origem, caminho, 0))
            return 1;
    return 0;
}

int encontrar_ciclo_hamiltoniano_bt(GrafoMatriz *g, int *ciclo) {
    if (g->n < 3) return 0;
    return buscar_a_partir_de(g, 0, ciclo, 1);
}

int eh_caminho_hamiltoniano(GrafoMatriz *g, int *caminho, int fechado) {
    int *visto = calloc(g->n, sizeof(int));
    int valido = 1;
    for (int i = 0; i < g->n && valido; i++) {
        if (caminho[i] < 0 || caminho[i] >= g->n || visto[caminho[i]]) valido = 0;
        else visto[caminho[i]] = 1;
        if (valido && i > 0 && !g->adj[caminho[i - 1]][caminho[i]]) valido = 0;
    }
    if (valido && fechado && (g->n < 3 || !g->adj[caminho[g->n - 1]][caminho[0]])) valido = 0;
    free(visto);
    return valido;
}
