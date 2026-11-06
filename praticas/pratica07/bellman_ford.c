#include <stdio.h>
#include <stdlib.h>
#include "bellman_ford.h"

GrafoListaPonderado *criar_grafo_ponderado(int n) {
    GrafoListaPonderado *g = malloc(sizeof(GrafoListaPonderado));
    g->n = n;
    g->adj = calloc(n, sizeof(NoPonderado *));
    return g;
}

void inserir_arco_ponderado(GrafoListaPonderado *g, int u, int v, int peso) {
    NoPonderado *novo = malloc(sizeof(NoPonderado));
    novo->destino = v;
    novo->peso = peso;
    novo->prox = NULL;
    if (g->adj[u] == NULL) {
        g->adj[u] = novo;
        return;
    }
    NoPonderado *p = g->adj[u];
    while (p->prox != NULL)
        p = p->prox;
    p->prox = novo;
}

void liberar_grafo_ponderado(GrafoListaPonderado *g) {
    for (int i = 0; i < g->n; i++) {
        NoPonderado *p = g->adj[i];
        while (p != NULL) {
            NoPonderado *prox = p->prox;
            free(p);
            p = prox;
        }
    }
    free(g->adj);
    free(g);
}

static int relaxar_todas(GrafoListaPonderado *g, int *dist, int *pred) {
    int houve_mudanca = 0;
    for (int u = 0; u < g->n; u++) {
        if (dist[u] == INFINITO) continue;
        for (NoPonderado *p = g->adj[u]; p != NULL; p = p->prox) {
            int v = p->destino;
            if (dist[u] + p->peso < dist[v]) {
                dist[v] = dist[u] + p->peso;
                pred[v] = u;
                houve_mudanca = 1;
            }
        }
    }
    return houve_mudanca;
}

int bellman_ford(GrafoListaPonderado *g, int origem, int *dist, int *pred) {
    for (int i = 0; i < g->n; i++) {
        dist[i] = INFINITO;
        pred[i] = -1;
    }
    dist[origem] = 0;
    for (int i = 1; i <= g->n - 1; i++)
        if (!relaxar_todas(g, dist, pred))
            return 1;
    return !relaxar_todas(g, dist, pred);
}

static int vertice_em_ciclo(int n, int *pred) {
    for (int v = 0; v < n; v++) {
        int x = v;
        for (int passo = 0; passo < n && x != -1; passo++)
            x = pred[x];
        if (x != -1)
            return x;
    }
    return -1;
}

static int peso_arco(GrafoListaPonderado *g, int u, int v) {
    int menor = INFINITO;
    for (NoPonderado *p = g->adj[u]; p != NULL; p = p->prox)
        if (p->destino == v && p->peso < menor)
            menor = p->peso;
    return menor;
}

int reconstruir_ciclo_negativo(GrafoListaPonderado *g, int *dist, int *pred, int *ciclo) {
    int x;
    while ((x = vertice_em_ciclo(g->n, pred)) == -1)
        if (!relaxar_todas(g, dist, pred))
            return 0;

    int tamanho = 0, v = x;
    do {
        ciclo[tamanho++] = v;
        v = pred[v];
    } while (v != x);

    for (int i = 0, j = tamanho - 1; i < j; i++, j--) {
        int tmp = ciclo[i];
        ciclo[i] = ciclo[j];
        ciclo[j] = tmp;
    }

    int custo = 0;
    printf("Ciclo negativo: ");
    for (int i = 0; i < tamanho; i++) {
        custo += peso_arco(g, ciclo[i], ciclo[(i + 1) % tamanho]);
        printf("%d -> ", ciclo[i]);
    }
    printf("%d (custo %d)\n", ciclo[0], custo);
    return tamanho;
}
