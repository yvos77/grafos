#include <stdlib.h>
#include "coloracao.h"

int *coloracao_gulosa(GrafoLista *g, int *num_cores) {
    int n = g->n;
    int *cor = malloc(n * sizeof(int));
    int *cor_usada = malloc(n * sizeof(int));
    for (int i = 0; i < n; i++)
        cor[i] = -1;
    *num_cores = 0;
    for (int u = 0; u < n; u++) {
        for (int c = 0; c < n; c++)
            cor_usada[c] = 0;
        for (No *p = g->adj[u]; p != NULL; p = p->prox)
            if (cor[p->destino] != -1)
                cor_usada[cor[p->destino]] = 1;
        int c = 0;
        while (cor_usada[c])
            c++;
        cor[u] = c;
        if (c + 1 > *num_cores)
            *num_cores = c + 1;
    }
    free(cor_usada);
    return cor;
}

static int *ordenar_por_grau(GrafoLista *g) {
    int n = g->n;
    int *ordem = malloc(n * sizeof(int));
    int *grau = malloc(n * sizeof(int));
    for (int i = 0; i < n; i++) {
        ordem[i] = i;
        grau[i] = grau_lista(g, i);
    }
    for (int i = 1; i < n; i++) {
        int atual = ordem[i];
        int j = i - 1;
        while (j >= 0 && grau[ordem[j]] < grau[atual]) {
            ordem[j + 1] = ordem[j];
            j--;
        }
        ordem[j + 1] = atual;
    }
    free(grau);
    return ordem;
}

static int tem_vizinho_com_cor(GrafoLista *g, int *cor, int u, int c) {
    for (No *p = g->adj[u]; p != NULL; p = p->prox)
        if (cor[p->destino] == c)
            return 1;
    return 0;
}

int *coloracao_welsh_powell(GrafoLista *g, int *num_cores) {
    int n = g->n;
    int *ordem = ordenar_por_grau(g);
    int *cor = malloc(n * sizeof(int));
    for (int i = 0; i < n; i++)
        cor[i] = -1;
    int coloridos = 0, c = 0;
    while (coloridos < n) {
        for (int k = 0; k < n; k++) {
            int u = ordem[k];
            if (cor[u] == -1 && !tem_vizinho_com_cor(g, cor, u, c)) {
                cor[u] = c;
                coloridos++;
            }
        }
        c++;
    }
    *num_cores = c;
    free(ordem);
    return cor;
}

int eh_bipartido(GrafoLista *g) {
    int n = g->n;
    int *cor = malloc(n * sizeof(int));
    int *fila = malloc(n * sizeof(int));
    for (int i = 0; i < n; i++)
        cor[i] = -1;
    int bipartido = 1;
    for (int s = 0; s < n && bipartido; s++) {
        if (cor[s] != -1) continue;
        int inicio = 0, fim = 0;
        cor[s] = 0;
        fila[fim++] = s;
        while (inicio < fim && bipartido) {
            int u = fila[inicio++];
            for (No *p = g->adj[u]; p != NULL; p = p->prox) {
                int v = p->destino;
                if (cor[v] == -1) {
                    cor[v] = 1 - cor[u];
                    fila[fim++] = v;
                } else if (cor[v] == cor[u]) {
                    bipartido = 0;
                    break;
                }
            }
        }
    }
    free(cor);
    free(fila);
    return bipartido;
}

int coloracao_valida(GrafoLista *g, int *cor) {
    for (int u = 0; u < g->n; u++)
        for (No *p = g->adj[u]; p != NULL; p = p->prox)
            if (cor[u] == cor[p->destino])
                return 0;
    return 1;
}
