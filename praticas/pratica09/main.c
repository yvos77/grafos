#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "cobertura.h"

static unsigned int semente = 2026u;

static unsigned int aleatorio(void) {
    semente = semente * 1103515245u + 12345u;
    return (semente >> 16) & 0x7FFF;
}

static void imprimir_conjunto(int n, int *c, const char *rotulos) {
    printf("{");
    int primeiro = 1;
    for (int i = 0; i < n; i++) {
        if (!c[i]) continue;
        if (!primeiro) printf(", ");
        if (rotulos != NULL) printf("%c", rotulos[i]);
        else printf("%d", i);
        primeiro = 0;
    }
    printf("}");
}

static void analisar(const char *titulo, GrafoMatriz *g, const char *rotulos) {
    int *c = malloc(g->n * sizeof(int));
    long long nos;
    printf("=== %s ===\n", titulo);

    int k = cobertura_gulosa(g, c);
    printf("  Gulosa:       %d vertices ", k);
    imprimir_conjunto(g->n, c, rotulos);
    printf(" | cobertura valida? %s\n", eh_cobertura(g, c) ? "sim" : "nao");

    k = cobertura_minima_bt(g, c, &nos);
    printf("  Backtracking: %d vertices ", k);
    imprimir_conjunto(g->n, c, rotulos);
    printf(" | cobertura valida? %s | nos explorados: %lld\n\n", eh_cobertura(g, c) ? "sim" : "nao", nos);

    free(c);
    liberar_grafo_matriz(g);
}

static GrafoMatriz *grafo_aleatorio(int n, int percentual) {
    GrafoMatriz *g = criar_grafo_matriz(n);
    for (int i = 0; i < n; i++)
        for (int j = i + 1; j < n; j++)
            if ((int)(aleatorio() % 100) < percentual)
                inserir_aresta_matriz(g, i, j);
    return g;
}

static double medir_bt(GrafoMatriz *g, int *c, int *tamanho, long long *nos) {
    int repeticoes = 0;
    clock_t inicio = clock(), agora;
    do {
        *tamanho = cobertura_minima_bt(g, c, nos);
        repeticoes++;
        agora = clock();
    } while (agora - inicio < CLOCKS_PER_SEC / 20);
    return 1000.0 * (double)(agora - inicio) / CLOCKS_PER_SEC / repeticoes;
}

static double medir_gulosa(GrafoMatriz *g, int *c, int *tamanho) {
    int repeticoes = 0;
    clock_t inicio = clock(), agora;
    do {
        *tamanho = cobertura_gulosa(g, c);
        repeticoes++;
        agora = clock();
    } while (agora - inicio < CLOCKS_PER_SEC / 20);
    return 1000.0 * (double)(agora - inicio) / CLOCKS_PER_SEC / repeticoes;
}

static void experimento(void) {
    int tamanhos[] = {5, 8, 10, 12, 15};
    printf("=== Experimento: grafos aleatorios com densidade 50%% ===\n");
    printf("  n |  m | otimo | guloso | tempo BT (ms) | tempo guloso (ms) | nos BT | 2^n\n");
    for (int t = 0; t < 5; t++) {
        int n = tamanhos[t];
        GrafoMatriz *g = grafo_aleatorio(n, 50);
        int *c = malloc(n * sizeof(int));
        int otimo, guloso;
        long long nos;
        double t_bt = medir_bt(g, c, &otimo, &nos);
        double t_gul = medir_gulosa(g, c, &guloso);
        printf(" %2d | %2d | %5d | %6d | %13.4f | %17.4f | %6lld | %d\n",
               n, contar_arestas(g), otimo, guloso, t_bt, t_gul, nos, 1 << n);
        free(c);
        liberar_grafo_matriz(g);
    }
}

int main(void) {
    GrafoMatriz *estrela = criar_grafo_matriz(6);
    for (int i = 1; i < 6; i++)
        inserir_aresta_matriz(estrela, 0, i);
    analisar("Estrela: centro A ligado a B..F", estrela, "ABCDEF");

    GrafoMatriz *ciclo = criar_grafo_matriz(5);
    for (int i = 0; i < 5; i++)
        inserir_aresta_matriz(ciclo, i, (i + 1) % 5);
    analisar("Ciclo C5", ciclo, "ABCDE");

    GrafoMatriz *caminho = criar_grafo_matriz(5);
    inserir_aresta_matriz(caminho, 1, 3);
    inserir_aresta_matriz(caminho, 3, 0);
    inserir_aresta_matriz(caminho, 0, 4);
    inserir_aresta_matriz(caminho, 4, 2);
    analisar("Caminho A-B-C-D-E (o guloso erra)", caminho, "CAEBD");

    experimento();
    return 0;
}
