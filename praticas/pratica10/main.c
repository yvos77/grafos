#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "hamiltoniano.h"
#include "tps.h"

static unsigned int semente = 2026u;

static unsigned int aleatorio(void) {
    semente = semente * 1103515245u + 12345u;
    return (semente >> 16) & 0x7FFF;
}

static void imprimir_sequencia(int *v, int n, int fechar) {
    for (int i = 0; i < n; i++)
        printf("%d%s", v[i], i + 1 < n ? " -> " : "");
    if (fechar) printf(" -> %d", v[0]);
}

static GrafoMatriz *grafo_de_arestas(int n, int arestas[][2], int m) {
    GrafoMatriz *g = criar_grafo_matriz(n);
    for (int i = 0; i < m; i++)
        inserir_aresta_matriz(g, arestas[i][0], arestas[i][1]);
    return g;
}

static void analisar_hamiltoniano(const char *titulo, GrafoMatriz *g) {
    int *seq = malloc(g->n * sizeof(int));
    printf("=== %s (n = %d) ===\n", titulo, g->n);

    printf("  Caminho hamiltoniano: ");
    if (encontrar_caminho_hamiltoniano_bt(g, seq)) imprimir_sequencia(seq, g->n, 0);
    else printf("nao existe");

    printf("\n  Ciclo hamiltoniano:   ");
    if (encontrar_ciclo_hamiltoniano_bt(g, seq)) imprimir_sequencia(seq, g->n, 1);
    else printf("nao existe");

    InstanciaTSP *t = grafo_para_tsp(g);
    int custo = tsp_forca_bruta(t, seq);
    printf("\n  Reducao para TSP (peso 1 = aresta, 2 = nao aresta): custo otimo = %d -> %s\n\n",
           custo, custo == g->n ? "ha ciclo hamiltoniano" : "nao ha ciclo hamiltoniano");
    liberar_instancia_tsp(t);
    free(seq);
    liberar_grafo_matriz(g);
}

static void exemplos_hamiltoniano(void) {
    int cubo[][2] = {{0, 1}, {1, 2}, {2, 3}, {3, 0}, {4, 5}, {5, 6}, {6, 7}, {7, 4},
                     {0, 4}, {1, 5}, {2, 6}, {3, 7}};
    int petersen[][2] = {{0, 1}, {1, 2}, {2, 3}, {3, 4}, {4, 0}, {0, 5}, {1, 6}, {2, 7},
                         {3, 8}, {4, 9}, {5, 7}, {7, 9}, {9, 6}, {6, 8}, {8, 5}};
    int estrela[][2] = {{0, 1}, {0, 2}, {0, 3}};
    int caminho[][2] = {{0, 1}, {1, 2}, {2, 3}, {3, 4}};

    analisar_hamiltoniano("Cubo", grafo_de_arestas(8, cubo, 12));
    analisar_hamiltoniano("Petersen", grafo_de_arestas(10, petersen, 15));
    analisar_hamiltoniano("Caminho 0-1-2-3-4", grafo_de_arestas(5, caminho, 4));
    analisar_hamiltoniano("Estrela com 3 folhas", grafo_de_arestas(4, estrela, 3));
}

static void exemplo_tsp(void) {
    int custos[5][5] = {
        {0, 3, 4, 2, 7},
        {3, 0, 4, 6, 3},
        {4, 4, 0, 5, 8},
        {2, 6, 5, 0, 6},
        {7, 3, 8, 6, 0}};
    InstanciaTSP *t = criar_instancia_tsp(5);
    for (int i = 0; i < 5; i++)
        for (int j = 0; j < 5; j++)
            t->custo[i][j] = custos[i][j];

    int rota[5], custo;
    printf("=== TSP com 5 cidades ===\n");
    custo = tsp_forca_bruta(t, rota);
    printf("  Forca bruta:            custo %2d | ", custo);
    imprimir_sequencia(rota, 5, 1);
    custo = tsp_vizinho_mais_proximo(t, 0, rota);
    printf("\n  Vizinho mais proximo:   custo %2d | ", custo);
    imprimir_sequencia(rota, 5, 1);
    custo = tsp_insercao(t, rota);
    printf("\n  Insercao mais proxima:  custo %2d | ", custo);
    imprimir_sequencia(rota, 5, 1);
    printf("\n\n");
    liberar_instancia_tsp(t);
}

static double cronometrar_heuristica(InstanciaTSP *t, int *rota, int *custo, int insercao) {
    int repeticoes = 0;
    clock_t inicio = clock(), agora;
    do {
        *custo = insercao ? tsp_insercao(t, rota) : tsp_vizinho_mais_proximo(t, 0, rota);
        repeticoes++;
        agora = clock();
    } while (agora - inicio < CLOCKS_PER_SEC / 20);
    return 1000.0 * (double)(agora - inicio) / CLOCKS_PER_SEC / repeticoes;
}

static void experimento(void) {
    int tamanhos[] = {5, 8, 10, 12};
    printf("=== Experimento: custos aleatorios de 1 a 100 ===\n");
    printf("  n |     (n-1)! | otimo | t forca bruta (ms) | VMP: custo (gap) tempo ms | Insercao: custo (gap) tempo ms\n");
    for (int k = 0; k < 4; k++) {
        int n = tamanhos[k];
        InstanciaTSP *t = criar_instancia_tsp(n);
        for (int i = 0; i < n; i++)
            for (int j = i + 1; j < n; j++)
                t->custo[i][j] = t->custo[j][i] = 1 + (int)(aleatorio() % 100);

        int *rota = malloc(n * sizeof(int));
        long fatorial = 1;
        for (int i = 2; i < n; i++) fatorial *= i;

        clock_t inicio = clock();
        int otimo = tsp_forca_bruta(t, rota);
        double t_fb = 1000.0 * (double)(clock() - inicio) / CLOCKS_PER_SEC;

        int c_vmp, c_ins;
        double t_vmp = cronometrar_heuristica(t, rota, &c_vmp, 0);
        double t_ins = cronometrar_heuristica(t, rota, &c_ins, 1);
        printf(" %2d | %10ld | %5d | %18.3f | %4d (+%5.1f%%) %10.4f | %4d (+%5.1f%%) %15.4f\n",
               n, fatorial, otimo, t_fb,
               c_vmp, 100.0 * (c_vmp - otimo) / otimo, t_vmp,
               c_ins, 100.0 * (c_ins - otimo) / otimo, t_ins);
        free(rota);
        liberar_instancia_tsp(t);
    }
}

int main(void) {
    exemplos_hamiltoniano();
    exemplo_tsp();
    experimento();
    return 0;
}
