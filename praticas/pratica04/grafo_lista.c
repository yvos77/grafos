#include <stdio.h>
#include <stdlib.h>
#include "grafo_lista.h"

GrafoLista *criar_grafo_lista(int n) {
    GrafoLista *g = malloc(sizeof(GrafoLista));
    g->n = n;
    g->adj = calloc(n, sizeof(No *));
    return g;
}

static void anexar(GrafoLista *g, int u, int v) {
    No *novo = malloc(sizeof(No));
    novo->destino = v;
    novo->prox = NULL;
    if (g->adj[u] == NULL) {
        g->adj[u] = novo;
        return;
    }
    No *p = g->adj[u];
    while (p->prox != NULL)
        p = p->prox;
    p->prox = novo;
}

void inserir_aresta_lista(GrafoLista *g, int u, int v) {
    anexar(g, u, v);
    anexar(g, v, u);
}

void inserir_arco_lista(GrafoLista *g, int u, int v) {
    anexar(g, u, v);
}

int grau_lista(GrafoLista *g, int v) {
    int grau = 0;
    for (No *p = g->adj[v]; p != NULL; p = p->prox)
        grau++;
    return grau;
}

static void imprimir_vertice(int v, const char *rotulos) {
    if (rotulos != NULL) printf("%c", rotulos[v]);
    else printf("%d", v);
}

void imprimir_grafo_lista(GrafoLista *g, const char *rotulos) {
    for (int i = 0; i < g->n; i++) {
        imprimir_vertice(i, rotulos);
        printf(":");
        for (No *p = g->adj[i]; p != NULL; p = p->prox) {
            printf(" -> ");
            imprimir_vertice(p->destino, rotulos);
        }
        printf(" -> NULL\n");
    }
}

void liberar_grafo_lista(GrafoLista *g) {
    for (int i = 0; i < g->n; i++) {
        No *p = g->adj[i];
        while (p != NULL) {
            No *prox = p->prox;
            free(p);
            p = prox;
        }
    }
    free(g->adj);
    free(g);
}
