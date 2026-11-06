#include <stdlib.h>
#include <string.h>
#include "cobertura.h"

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

int contar_arestas(GrafoMatriz *g) {
    int m = 0;
    for (int i = 0; i < g->n; i++)
        for (int j = i + 1; j < g->n; j++)
            m += g->adj[i][j];
    return m;
}

void liberar_grafo_matriz(GrafoMatriz *g) {
    for (int i = 0; i < g->n; i++)
        free(g->adj[i]);
    free(g->adj);
    free(g);
}

int eh_cobertura(GrafoMatriz *g, int *na_cobertura) {
    for (int i = 0; i < g->n; i++)
        for (int j = i + 1; j < g->n; j++)
            if (g->adj[i][j] && !na_cobertura[i] && !na_cobertura[j])
                return 0;
    return 1;
}

int cobertura_gulosa(GrafoMatriz *g, int *na_cobertura) {
    int n = g->n;
    int *grau = calloc(n, sizeof(int));
    for (int i = 0; i < n; i++) {
        na_cobertura[i] = 0;
        for (int j = 0; j < n; j++)
            grau[i] += g->adj[i][j];
    }

    int tamanho = 0;
    for (;;) {
        int escolhido = -1;
        for (int i = 0; i < n; i++)
            if (!na_cobertura[i] && grau[i] > 0 && (escolhido == -1 || grau[i] > grau[escolhido]))
                escolhido = i;
        if (escolhido == -1) break;

        na_cobertura[escolhido] = 1;
        tamanho++;
        for (int j = 0; j < n; j++)
            if (g->adj[escolhido][j] && !na_cobertura[j])
                grau[j]--;
        grau[escolhido] = 0;
    }
    free(grau);
    return tamanho;
}

typedef struct {
    GrafoMatriz *g;
    int *atual;
    int tamanho_atual;
    int *melhor;
    int melhor_tamanho;
    long long nos;
} Busca;

static int primeira_aresta_descoberta(Busca *b, int *u, int *v) {
    for (int i = 0; i < b->g->n; i++) {
        if (b->atual[i]) continue;
        for (int j = i + 1; j < b->g->n; j++) {
            if (b->g->adj[i][j] && !b->atual[j]) {
                *u = i;
                *v = j;
                return 1;
            }
        }
    }
    return 0;
}

static void explorar(Busca *b) {
    int u, v;
    b->nos++;
    if (!primeira_aresta_descoberta(b, &u, &v)) {
        if (b->tamanho_atual < b->melhor_tamanho) {
            b->melhor_tamanho = b->tamanho_atual;
            memcpy(b->melhor, b->atual, b->g->n * sizeof(int));
        }
        return;
    }
    if (b->tamanho_atual + 1 >= b->melhor_tamanho)
        return;

    int extremos[2] = {u, v};
    for (int k = 0; k < 2; k++) {
        b->atual[extremos[k]] = 1;
        b->tamanho_atual++;
        explorar(b);
        b->atual[extremos[k]] = 0;
        b->tamanho_atual--;
    }
}

int cobertura_minima_bt(GrafoMatriz *g, int *na_cobertura, long long *nos_explorados) {
    Busca b;
    b.g = g;
    b.atual = calloc(g->n, sizeof(int));
    b.tamanho_atual = 0;
    b.melhor = na_cobertura;
    b.melhor_tamanho = cobertura_gulosa(g, na_cobertura);
    b.nos = 0;
    explorar(&b);
    free(b.atual);
    if (nos_explorados != NULL)
        *nos_explorados = b.nos;
    return b.melhor_tamanho;
}
