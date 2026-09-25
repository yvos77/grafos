#include <stdlib.h>
#include "planaridade.h"

typedef struct {
    int n;
    int adj[MAX_KURATOWSKI][MAX_KURATOWSKI];
    int grau[MAX_KURATOWSKI];
    int ramo[MAX_KURATOWSKI];
    int usado[MAX_KURATOWSKI];
    int origem[10], destino[10];
    int num_pares;
} Busca;

int contar_arestas(GrafoLista *g) {
    int total = 0;
    for (int i = 0; i < g->n; i++)
        for (No *p = g->adj[i]; p != NULL; p = p->prox)
            total++;
    return total / 2;
}

int eh_planar_euler(GrafoLista *g) {
    if (g->n < 3) return 1;
    return contar_arestas(g) <= 3 * g->n - 6;
}

int numero_de_faces(GrafoLista *g) {
    return 2 - g->n + contar_arestas(g);
}

static void montar_busca(Busca *b, GrafoLista *g) {
    b->n = g->n;
    for (int i = 0; i < b->n; i++) {
        b->grau[i] = 0;
        for (int j = 0; j < b->n; j++)
            b->adj[i][j] = 0;
    }
    for (int i = 0; i < b->n; i++) {
        for (No *p = g->adj[i]; p != NULL; p = p->prox) {
            if (!b->adj[i][p->destino]) {
                b->adj[i][p->destino] = 1;
                b->grau[i]++;
            }
        }
    }
}

static void preparar_tentativa(Busca *b, int *vertices, int k) {
    for (int i = 0; i < b->n; i++) {
        b->ramo[i] = 0;
        b->usado[i] = 0;
    }
    for (int i = 0; i < k; i++)
        b->ramo[vertices[i]] = 1;
    b->num_pares = 0;
}

static void adicionar_par(Busca *b, int u, int v) {
    b->origem[b->num_pares] = u;
    b->destino[b->num_pares] = v;
    b->num_pares++;
}

static int ligar_pares(Busca *b, int k);

static int estender_caminho(Busca *b, int atual, int k) {
    for (int w = 0; w < b->n; w++) {
        if (!b->adj[atual][w]) continue;
        if (w == b->destino[k]) {
            if (ligar_pares(b, k + 1)) return 1;
            continue;
        }
        if (b->ramo[w] || b->usado[w]) continue;
        b->usado[w] = 1;
        if (estender_caminho(b, w, k)) return 1;
        b->usado[w] = 0;
    }
    return 0;
}

static int ligar_pares(Busca *b, int k) {
    if (k == b->num_pares) return 1;
    return estender_caminho(b, b->origem[k], k);
}

static int selecionar(Busca *b, int mascara, int tamanho, int grau_minimo, int *vertices) {
    int k = 0;
    for (int i = 0; i < b->n; i++) {
        if (mascara & (1 << i)) {
            if (k == tamanho || b->grau[i] < grau_minimo) return 0;
            vertices[k++] = i;
        }
    }
    return k == tamanho;
}

int contem_subdivisao_k5(GrafoLista *g) {
    if (g->n < 5 || g->n > MAX_KURATOWSKI) return 0;
    Busca b;
    montar_busca(&b, g);
    int v[5];
    for (int mascara = 0; mascara < (1 << b.n); mascara++) {
        if (!selecionar(&b, mascara, 5, 4, v)) continue;
        preparar_tentativa(&b, v, 5);
        for (int i = 0; i < 5; i++)
            for (int j = i + 1; j < 5; j++)
                adicionar_par(&b, v[i], v[j]);
        if (ligar_pares(&b, 0)) return 1;
    }
    return 0;
}

int contem_subdivisao_k33(GrafoLista *g) {
    if (g->n < 6 || g->n > MAX_KURATOWSKI) return 0;
    Busca b;
    montar_busca(&b, g);
    int v[6];
    for (int mascara = 0; mascara < (1 << b.n); mascara++) {
        if (!selecionar(&b, mascara, 6, 3, v)) continue;
        for (int i = 1; i < 6; i++) {
            for (int j = i + 1; j < 6; j++) {
                int lado_a[3] = {v[0], v[i], v[j]};
                int lado_b[3], k = 0;
                for (int x = 1; x < 6; x++)
                    if (x != i && x != j)
                        lado_b[k++] = v[x];
                preparar_tentativa(&b, v, 6);
                for (int a = 0; a < 3; a++)
                    for (int c = 0; c < 3; c++)
                        adicionar_par(&b, lado_a[a], lado_b[c]);
                if (ligar_pares(&b, 0)) return 1;
            }
        }
    }
    return 0;
}

int eh_planar(GrafoLista *g) {
    if (!eh_planar_euler(g)) return 0;
    if (g->n > MAX_KURATOWSKI) return -1;
    if (contem_subdivisao_k5(g) || contem_subdivisao_k33(g)) return 0;
    return 1;
}
