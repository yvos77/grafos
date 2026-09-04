#include <stdio.h>
#include <stdlib.h>
#include "grafo_lista.h"

static int vertice_valido(GrafoLista *g, int v) {
    return g != NULL && v >= 0 && v < g->n;
}

static int inserir_ordenado(No **lista, int destino) {
    No *ant = NULL;
    No *atual = *lista;

    while (atual != NULL && atual->destino < destino) {
        ant = atual;
        atual = atual->prox;
    }

    if (atual != NULL && atual->destino == destino) return 0;

    No *novo = malloc(sizeof(No));
    if (novo == NULL) return 0;

    novo->destino = destino;
    novo->prox = atual;

    if (ant == NULL) *lista = novo;
    else ant->prox = novo;

    return 1;
}

static int remover_no(No **lista, int destino) {
    No *ant = NULL;
    No *atual = *lista;

    while (atual != NULL && atual->destino != destino) {
        ant = atual;
        atual = atual->prox;
    }

    if (atual == NULL) return 0;

    if (ant == NULL) *lista = atual->prox;
    else ant->prox = atual->prox;

    free(atual);
    return 1;
}

GrafoLista *criar_grafo_lista(int n) {
    if (n <= 0) return NULL;

    GrafoLista *g = malloc(sizeof(GrafoLista));
    if (g == NULL) return NULL;

    g->n = n;
    g->adj = malloc(n * sizeof(No *));
    if (g->adj == NULL) {
        free(g);
        return NULL;
    }

    for (int i = 0; i < n; i++) g->adj[i] = NULL;

    return g;
}

int inserir_aresta_lista(GrafoLista *g, int u, int v) {
    if (!vertice_valido(g, u) || !vertice_valido(g, v)) return 0;
    if (u == v) return 0;

    if (!inserir_ordenado(&g->adj[u], v)) return 0;

    if (!inserir_ordenado(&g->adj[v], u)) {
        remover_no(&g->adj[u], v);
        return 0;
    }

    return 1;
}

int remover_aresta_lista(GrafoLista *g, int u, int v) {
    if (!vertice_valido(g, u) || !vertice_valido(g, v)) return 0;
    if (!remover_no(&g->adj[u], v)) return 0;

    remover_no(&g->adj[v], u);
    return 1;
}

int grau_lista(GrafoLista *g, int v) {
    if (!vertice_valido(g, v)) return -1;

    int grau = 0;
    for (No *p = g->adj[v]; p != NULL; p = p->prox) grau++;
    return grau;
}

int sao_adjacentes_lista(GrafoLista *g, int u, int v) {
    if (!vertice_valido(g, u) || !vertice_valido(g, v)) return 0;

    for (No *p = g->adj[u]; p != NULL; p = p->prox) {
        if (p->destino == v) return 1;
    }
    return 0;
}

void imprimir_grafo_lista(GrafoLista *g) {
    if (g == NULL) return;

    for (int i = 0; i < g->n; i++) {
        printf("%3d ->", i);
        for (No *p = g->adj[i]; p != NULL; p = p->prox) printf(" %d", p->destino);
        printf("\n");
    }
}

void liberar_grafo_lista(GrafoLista *g) {
    if (g == NULL) return;

    for (int i = 0; i < g->n; i++) {
        No *p = g->adj[i];
        while (p != NULL) {
            No *tmp = p;
            p = p->prox;
            free(tmp);
        }
    }

    free(g->adj);
    free(g);
}