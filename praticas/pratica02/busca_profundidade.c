#include <stdlib.h>
#include "busca_profundidade.h"

Pilha *criar_pilha(int capacidade) {
    Pilha *p = malloc(sizeof(Pilha));
    p->dados = malloc(capacidade * sizeof(int));
    p->capacidade = capacidade;
    p->topo = -1;
    return p;
}

void empilhar(Pilha *p, int x) {
    p->dados[++p->topo] = x;
}

int desempilhar(Pilha *p) {
    return p->dados[p->topo--];
}

int topo_pilha(Pilha *p) {
    return p->dados[p->topo];
}

int pilha_vazia(Pilha *p) {
    return p->topo == -1;
}

void liberar_pilha(Pilha *p) {
    free(p->dados);
    free(p);
}

void dfs_recursiva(GrafoLista *g, int u, int *visitado, int *entrada, int *saida, int *tempo) {
    visitado[u] = 1;
    entrada[u] = ++(*tempo);
    for (No *p = g->adj[u]; p != NULL; p = p->prox)
        if (!visitado[p->destino])
            dfs_recursiva(g, p->destino, visitado, entrada, saida, tempo);
    saida[u] = ++(*tempo);
}

int dfs_iterativa(GrafoLista *g, int origem, int *ordem) {
    int *visitado = calloc(g->n, sizeof(int));
    No **proximo = malloc(g->n * sizeof(No *));
    for (int i = 0; i < g->n; i++)
        proximo[i] = g->adj[i];
    Pilha *pilha = criar_pilha(g->n);
    int k = 0;
    visitado[origem] = 1;
    ordem[k++] = origem;
    empilhar(pilha, origem);
    while (!pilha_vazia(pilha)) {
        int u = topo_pilha(pilha);
        while (proximo[u] != NULL && visitado[proximo[u]->destino])
            proximo[u] = proximo[u]->prox;
        if (proximo[u] == NULL) {
            desempilhar(pilha);
            continue;
        }
        int v = proximo[u]->destino;
        visitado[v] = 1;
        ordem[k++] = v;
        empilhar(pilha, v);
    }
    liberar_pilha(pilha);
    free(proximo);
    free(visitado);
    return k;
}

int contar_componentes(GrafoLista *g) {
    int *visitado = calloc(g->n, sizeof(int));
    int *entrada = malloc(g->n * sizeof(int));
    int *saida = malloc(g->n * sizeof(int));
    int tempo = 0, componentes = 0;
    for (int i = 0; i < g->n; i++) {
        if (!visitado[i]) {
            componentes++;
            dfs_recursiva(g, i, visitado, entrada, saida, &tempo);
        }
    }
    free(visitado);
    free(entrada);
    free(saida);
    return componentes;
}

static int ciclo_a_partir(GrafoLista *g, int u, int pai, int *visitado) {
    visitado[u] = 1;
    for (No *p = g->adj[u]; p != NULL; p = p->prox) {
        int v = p->destino;
        if (!visitado[v]) {
            if (ciclo_a_partir(g, v, u, visitado)) return 1;
        } else if (v != pai) {
            return 1;
        }
    }
    return 0;
}

int tem_ciclo(GrafoLista *g) {
    int *visitado = calloc(g->n, sizeof(int));
    int encontrou = 0;
    for (int i = 0; i < g->n && !encontrou; i++)
        if (!visitado[i])
            encontrou = ciclo_a_partir(g, i, -1, visitado);
    free(visitado);
    return encontrou;
}
