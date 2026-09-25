#include <stdlib.h>
#include "dag.h"

enum { BRANCO, CINZA, PRETO };

int *ordenacao_topologica_kahn(GrafoLista *g, int *tamanho) {
    int n = g->n;
    int *grau_entrada = calloc(n, sizeof(int));
    for (int u = 0; u < n; u++)
        for (No *p = g->adj[u]; p != NULL; p = p->prox)
            grau_entrada[p->destino]++;

    int *fila = malloc(n * sizeof(int));
    int inicio = 0, fim = 0;
    for (int i = 0; i < n; i++)
        if (grau_entrada[i] == 0)
            fila[fim++] = i;

    int *ordem = malloc(n * sizeof(int));
    int k = 0;
    while (inicio < fim) {
        int u = fila[inicio++];
        ordem[k++] = u;
        for (No *p = g->adj[u]; p != NULL; p = p->prox) {
            int v = p->destino;
            grau_entrada[v]--;
            if (grau_entrada[v] == 0)
                fila[fim++] = v;
        }
    }
    free(grau_entrada);
    free(fila);

    if (k < n) {
        free(ordem);
        *tamanho = 0;
        return NULL;
    }
    *tamanho = k;
    return ordem;
}

static int visitar(GrafoLista *g, int u, int *estado, int *ordem, int *pos) {
    estado[u] = CINZA;
    for (No *p = g->adj[u]; p != NULL; p = p->prox) {
        int v = p->destino;
        if (estado[v] == CINZA) return 0;
        if (estado[v] == BRANCO && !visitar(g, v, estado, ordem, pos)) return 0;
    }
    estado[u] = PRETO;
    ordem[--(*pos)] = u;
    return 1;
}

int *ordenacao_topologica_dfs(GrafoLista *g, int *tamanho) {
    int n = g->n;
    int *estado = calloc(n, sizeof(int));
    int *ordem = malloc(n * sizeof(int));
    int pos = n;
    for (int i = 0; i < n; i++) {
        if (estado[i] == BRANCO && !visitar(g, i, estado, ordem, &pos)) {
            free(estado);
            free(ordem);
            *tamanho = 0;
            return NULL;
        }
    }
    free(estado);
    *tamanho = n;
    return ordem;
}

int eh_dag(GrafoLista *g) {
    int tamanho;
    int *ordem = ordenacao_topologica_kahn(g, &tamanho);
    if (ordem == NULL) return 0;
    free(ordem);
    return 1;
}
