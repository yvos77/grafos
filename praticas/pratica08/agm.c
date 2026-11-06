#include <stdlib.h>
#include <string.h>
#include "agm.h"

DSU *criar_dsu(int n) {
    DSU *d = malloc(sizeof(DSU));
    d->n = n;
    d->pai = malloc(n * sizeof(int));
    d->rank = calloc(n, sizeof(int));
    for (int i = 0; i < n; i++)
        d->pai[i] = i;
    return d;
}

int find(DSU *d, int x) {
    if (d->pai[x] != x)
        d->pai[x] = find(d, d->pai[x]);
    return d->pai[x];
}

int unir(DSU *d, int x, int y) {
    int rx = find(d, x);
    int ry = find(d, y);
    if (rx == ry) return 0;
    if (d->rank[rx] < d->rank[ry]) {
        d->pai[rx] = ry;
    } else if (d->rank[rx] > d->rank[ry]) {
        d->pai[ry] = rx;
    } else {
        d->pai[ry] = rx;
        d->rank[rx]++;
    }
    return 1;
}

void liberar_dsu(DSU *d) {
    free(d->pai);
    free(d->rank);
    free(d);
}

GrafoListaPonderado *criar_grafo_ponderado(int n) {
    GrafoListaPonderado *g = malloc(sizeof(GrafoListaPonderado));
    g->n = n;
    g->adj = calloc(n, sizeof(NoPonderado *));
    return g;
}

static void anexar(GrafoListaPonderado *g, int u, int v, int peso) {
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

void inserir_aresta_ponderada(GrafoListaPonderado *g, int u, int v, int peso) {
    anexar(g, u, v, peso);
    anexar(g, v, u, peso);
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

static int comparar_arestas(const void *a, const void *b) {
    const Aresta *x = a, *y = b;
    if (x->peso != y->peso) return (x->peso > y->peso) - (x->peso < y->peso);
    if (x->u != y->u) return x->u - y->u;
    return x->v - y->v;
}

int kruskal(int n, Aresta *arestas, int m, Aresta *agm, int *peso_total) {
    *peso_total = 0;
    if (m == 0) return 0;
    Aresta *ordenadas = malloc(m * sizeof(Aresta));
    memcpy(ordenadas, arestas, m * sizeof(Aresta));
    qsort(ordenadas, m, sizeof(Aresta), comparar_arestas);

    DSU *d = criar_dsu(n);
    int k = 0;
    for (int i = 0; i < m && k < n - 1; i++) {
        if (unir(d, ordenadas[i].u, ordenadas[i].v)) {
            agm[k++] = ordenadas[i];
            *peso_total += ordenadas[i].peso;
        }
    }
    liberar_dsu(d);
    free(ordenadas);
    return k;
}

typedef struct {
    int *vertice;
    int *pos;
    int *chave;
    int tamanho;
} MinHeap;

static MinHeap *criar_heap(int n) {
    MinHeap *h = malloc(sizeof(MinHeap));
    h->vertice = malloc(n * sizeof(int));
    h->pos = malloc(n * sizeof(int));
    h->chave = malloc(n * sizeof(int));
    h->tamanho = 0;
    return h;
}

static void liberar_heap(MinHeap *h) {
    free(h->vertice);
    free(h->pos);
    free(h->chave);
    free(h);
}

static void trocar(MinHeap *h, int i, int j) {
    int tmp = h->vertice[i];
    h->vertice[i] = h->vertice[j];
    h->vertice[j] = tmp;
    h->pos[h->vertice[i]] = i;
    h->pos[h->vertice[j]] = j;
}

static int menor_chave(MinHeap *h, int i, int j) {
    return h->chave[h->vertice[i]] < h->chave[h->vertice[j]];
}

static void subir(MinHeap *h, int i) {
    while (i > 0) {
        int pai = (i - 1) / 2;
        if (!menor_chave(h, i, pai)) break;
        trocar(h, i, pai);
        i = pai;
    }
}

static void descer(MinHeap *h, int i) {
    for (;;) {
        int menor = i;
        int esq = 2 * i + 1, dir = 2 * i + 2;
        if (esq < h->tamanho && menor_chave(h, esq, menor)) menor = esq;
        if (dir < h->tamanho && menor_chave(h, dir, menor)) menor = dir;
        if (menor == i) break;
        trocar(h, i, menor);
        i = menor;
    }
}

static void heap_inserir(MinHeap *h, int v, int chave) {
    h->chave[v] = chave;
    h->vertice[h->tamanho] = v;
    h->pos[v] = h->tamanho;
    h->tamanho++;
    subir(h, h->tamanho - 1);
}

static int heap_extrair_min(MinHeap *h) {
    int v = h->vertice[0];
    h->tamanho--;
    if (h->tamanho > 0) {
        h->vertice[0] = h->vertice[h->tamanho];
        h->pos[h->vertice[0]] = 0;
        descer(h, 0);
    }
    h->pos[v] = -1;
    return v;
}

static void heap_diminuir_chave(MinHeap *h, int v, int chave) {
    h->chave[v] = chave;
    subir(h, h->pos[v]);
}

int prim(GrafoListaPonderado *g, int origem, Aresta *agm, int *peso_total) {
    int n = g->n;
    int *pred = malloc(n * sizeof(int));
    MinHeap *h = criar_heap(n);
    for (int v = 0; v < n; v++) {
        pred[v] = -1;
        heap_inserir(h, v, v == origem ? 0 : INFINITO);
    }

    int k = 0;
    *peso_total = 0;
    while (h->tamanho > 0) {
        int u = heap_extrair_min(h);
        if (h->chave[u] == INFINITO) break;
        if (pred[u] != -1) {
            agm[k].u = pred[u];
            agm[k].v = u;
            agm[k].peso = h->chave[u];
            *peso_total += h->chave[u];
            k++;
        }
        for (NoPonderado *p = g->adj[u]; p != NULL; p = p->prox) {
            int v = p->destino;
            if (h->pos[v] != -1 && p->peso < h->chave[v]) {
                pred[v] = u;
                heap_diminuir_chave(h, v, p->peso);
            }
        }
    }
    liberar_heap(h);
    free(pred);
    return k;
}
