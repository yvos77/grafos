#include <limits.h>
#include <stdlib.h>
#include <string.h>
#include "tps.h"

InstanciaTSP *criar_instancia_tsp(int n) {
    InstanciaTSP *t = malloc(sizeof(InstanciaTSP));
    t->n = n;
    t->custo = malloc(n * sizeof(int *));
    for (int i = 0; i < n; i++)
        t->custo[i] = calloc(n, sizeof(int));
    return t;
}

void liberar_instancia_tsp(InstanciaTSP *t) {
    for (int i = 0; i < t->n; i++)
        free(t->custo[i]);
    free(t->custo);
    free(t);
}

int custo_rota(InstanciaTSP *t, int *rota) {
    int total = 0;
    for (int i = 0; i < t->n; i++)
        total += t->custo[rota[i]][rota[(i + 1) % t->n]];
    return total;
}

typedef struct {
    InstanciaTSP *t;
    int *atual;
    int *melhor;
    int melhor_custo;
} BuscaTSP;

static void trocar(int *v, int i, int j) {
    int tmp = v[i];
    v[i] = v[j];
    v[j] = tmp;
}

static void permutar(BuscaTSP *b, int k, int custo_parcial) {
    int n = b->t->n;
    if (k == n) {
        int total = custo_parcial + b->t->custo[b->atual[n - 1]][b->atual[0]];
        if (total < b->melhor_custo) {
            b->melhor_custo = total;
            memcpy(b->melhor, b->atual, n * sizeof(int));
        }
        return;
    }
    for (int i = k; i < n; i++) {
        trocar(b->atual, k, i);
        permutar(b, k + 1, custo_parcial + b->t->custo[b->atual[k - 1]][b->atual[k]]);
        trocar(b->atual, k, i);
    }
}

int tsp_forca_bruta(InstanciaTSP *t, int *rota) {
    BuscaTSP b;
    b.t = t;
    b.atual = malloc(t->n * sizeof(int));
    b.melhor = rota;
    b.melhor_custo = INT_MAX;
    for (int i = 0; i < t->n; i++)
        b.atual[i] = i;
    permutar(&b, 1, 0);
    free(b.atual);
    return b.melhor_custo;
}

int tsp_vizinho_mais_proximo(InstanciaTSP *t, int origem, int *rota) {
    int *visitado = calloc(t->n, sizeof(int));
    rota[0] = origem;
    visitado[origem] = 1;
    for (int k = 1; k < t->n; k++) {
        int atual = rota[k - 1], proximo = -1;
        for (int v = 0; v < t->n; v++)
            if (!visitado[v] && (proximo == -1 || t->custo[atual][v] < t->custo[atual][proximo]))
                proximo = v;
        rota[k] = proximo;
        visitado[proximo] = 1;
    }
    free(visitado);
    return custo_rota(t, rota);
}

int tsp_insercao(InstanciaTSP *t, int *rota) {
    int n = t->n;
    int *na_rota = calloc(n, sizeof(int));
    int *dist_rota = malloc(n * sizeof(int));
    rota[0] = 0;
    na_rota[0] = 1;
    int tamanho = 1;
    for (int v = 0; v < n; v++)
        dist_rota[v] = t->custo[0][v];

    while (tamanho < n) {
        int k = -1;
        for (int v = 0; v < n; v++)
            if (!na_rota[v] && (k == -1 || dist_rota[v] < dist_rota[k]))
                k = v;

        int melhor_pos = 0, melhor_aumento = INT_MAX;
        for (int i = 0; i < tamanho; i++) {
            int a = rota[i], b = rota[(i + 1) % tamanho];
            int aumento = t->custo[a][k] + t->custo[k][b] - t->custo[a][b];
            if (aumento < melhor_aumento) {
                melhor_aumento = aumento;
                melhor_pos = i;
            }
        }

        for (int j = tamanho; j > melhor_pos + 1; j--)
            rota[j] = rota[j - 1];
        rota[melhor_pos + 1] = k;
        na_rota[k] = 1;
        tamanho++;

        for (int v = 0; v < n; v++)
            if (t->custo[k][v] < dist_rota[v])
                dist_rota[v] = t->custo[k][v];
    }
    free(na_rota);
    free(dist_rota);
    return custo_rota(t, rota);
}

InstanciaTSP *grafo_para_tsp(GrafoMatriz *g) {
    InstanciaTSP *t = criar_instancia_tsp(g->n);
    for (int i = 0; i < g->n; i++)
        for (int j = 0; j < g->n; j++)
            t->custo[i][j] = (i == j) ? 0 : (g->adj[i][j] ? 1 : 2);
    return t;
}
