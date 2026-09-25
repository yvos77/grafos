#include <stdio.h>
#include <stdlib.h>
#include "grafo_lista.h"
#include "coloracao.h"

static GrafoLista *grafo_de_arestas(int n, int arestas[][2], int m) {
    GrafoLista *g = criar_grafo_lista(n);
    for (int i = 0; i < m; i++)
        inserir_aresta_lista(g, arestas[i][0], arestas[i][1]);
    return g;
}

static void imprimir_cores(const char *metodo, GrafoLista *g, int *cor, int num_cores, const char *rotulos) {
    printf("  %-13s %d cores |", metodo, num_cores);
    for (int i = 0; i < g->n; i++)
        printf(" c(%c)=%d", rotulos[i], cor[i]);
    printf(" | valida? %s\n", coloracao_valida(g, cor) ? "sim" : "nao");
}

static void analisar(const char *nome, GrafoLista *g, const char *rotulos) {
    int num_cores;
    printf("=== %s ===\n", nome);
    int *gulosa = coloracao_gulosa(g, &num_cores);
    imprimir_cores("Gulosa:", g, gulosa, num_cores, rotulos);
    free(gulosa);
    int *wp = coloracao_welsh_powell(g, &num_cores);
    imprimir_cores("Welsh-Powell:", g, wp, num_cores, rotulos);
    free(wp);
    printf("  Bipartido? %s\n\n", eh_bipartido(g) ? "sim (numero cromatico <= 2)" : "nao");
    liberar_grafo_lista(g);
}

int main(void) {
    int caminho[][2] = {{0, 1}, {1, 2}};
    int atividade[][2] = {{0, 1}, {1, 2}, {3, 4}};
    int ordem_ruim[][2] = {{0, 2}, {1, 3}, {2, 3}};
    int k4[][2] = {{0, 1}, {0, 2}, {0, 3}, {1, 2}, {1, 3}, {2, 3}};
    int c5[][2] = {{0, 1}, {1, 2}, {2, 3}, {3, 4}, {4, 0}};
    int coroa[][2] = {{0, 3}, {0, 5}, {0, 7}, {2, 1}, {2, 5}, {2, 7},
                      {4, 1}, {4, 3}, {4, 7}, {6, 1}, {6, 3}, {6, 5}};

    analisar("Caminho 1-2-3 (Aula 05)", grafo_de_arestas(3, caminho, 2), "123");
    analisar("Atividade guiada da Aula 05", grafo_de_arestas(5, atividade, 3), "12345");
    analisar("Caminho em ordem ruim para a gulosa", grafo_de_arestas(4, ordem_ruim, 3), "0123");
    analisar("K4", grafo_de_arestas(4, k4, 6), "ABCD");
    analisar("Ciclo impar C5", grafo_de_arestas(5, c5, 5), "ABCDE");
    analisar("Coroa com 8 vertices (bipartido, mas as duas heuristicas usam 4 cores)", grafo_de_arestas(8, coroa, 12), "01234567");
    return 0;
}
