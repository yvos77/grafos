#include <stdlib.h>
#include "conectividade.h"

static Tarjan *executar_tarjan(GrafoLista *g) {
    Tarjan *t = malloc(sizeof(Tarjan));
    t->descoberta = calloc(g->n, sizeof(int));
    t->low = calloc(g->n, sizeof(int));
    t->eh_articulacao = calloc(g->n, sizeof(int));
    t->pontes = malloc(g->n * sizeof(Aresta));
    t->num_pontes = 0;
    t->tempo = 0;
    for (int i = 0; i < g->n; i++)
        if (t->descoberta[i] == 0)
            dfs_articulacoes(g, i, -1, t);
    return t;
}

static void liberar_tarjan(Tarjan *t) {
    free(t->descoberta);
    free(t->low);
    free(t->eh_articulacao);
    free(t->pontes);
    free(t);
}

void dfs_articulacoes(GrafoLista *g, int u, int pai, Tarjan *t) {
    int filhos = 0;
    t->descoberta[u] = t->low[u] = ++t->tempo;
    for (No *p = g->adj[u]; p != NULL; p = p->prox) {
        int v = p->destino;
        if (t->descoberta[v] == 0) {
            filhos++;
            dfs_articulacoes(g, v, u, t);
            if (t->low[v] < t->low[u])
                t->low[u] = t->low[v];
            if (pai != -1 && t->low[v] >= t->descoberta[u])
                t->eh_articulacao[u] = 1;
            if (t->low[v] > t->descoberta[u]) {
                t->pontes[t->num_pontes].u = u;
                t->pontes[t->num_pontes].v = v;
                t->num_pontes++;
            }
        } else if (v != pai && t->descoberta[v] < t->low[u]) {
            t->low[u] = t->descoberta[v];
        }
    }
    if (pai == -1 && filhos > 1)
        t->eh_articulacao[u] = 1;
}

int encontrar_articulacoes(GrafoLista *g, int *eh_articulacao) {
    Tarjan *t = executar_tarjan(g);
    int total = 0;
    for (int i = 0; i < g->n; i++) {
        eh_articulacao[i] = t->eh_articulacao[i];
        total += eh_articulacao[i];
    }
    liberar_tarjan(t);
    return total;
}

int detectar_pontes(GrafoLista *g, Aresta *pontes) {
    Tarjan *t = executar_tarjan(g);
    int total = t->num_pontes;
    for (int i = 0; i < total; i++)
        pontes[i] = t->pontes[i];
    liberar_tarjan(t);
    return total;
}
