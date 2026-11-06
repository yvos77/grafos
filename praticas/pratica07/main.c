#include <stdio.h>
#include <stdlib.h>
#include "bellman_ford.h"

static void imprimir_caminho(int *pred, int v, const char *rotulos) {
    if (pred[v] != -1) {
        imprimir_caminho(pred, pred[v], rotulos);
        printf(" -> ");
    }
    printf("%c", rotulos[v]);
}

static void imprimir_resultado(GrafoListaPonderado *g, int *dist, int *pred, const char *rotulos) {
    for (int v = 0; v < g->n; v++) {
        printf("  %c: ", rotulos[v]);
        if (dist[v] == INFINITO) {
            printf("inalcancavel\n");
            continue;
        }
        printf("dist = %3d | caminho: ", dist[v]);
        imprimir_caminho(pred, v, rotulos);
        printf("\n");
    }
}

static void exemplo_sem_ciclo(void) {
    const char *r = "stxyz";
    GrafoListaPonderado *g = criar_grafo_ponderado(5);
    inserir_arco_ponderado(g, 0, 1, 6);
    inserir_arco_ponderado(g, 0, 3, 7);
    inserir_arco_ponderado(g, 1, 2, 5);
    inserir_arco_ponderado(g, 1, 3, 8);
    inserir_arco_ponderado(g, 1, 4, -4);
    inserir_arco_ponderado(g, 2, 1, -2);
    inserir_arco_ponderado(g, 3, 2, -3);
    inserir_arco_ponderado(g, 3, 4, 9);
    inserir_arco_ponderado(g, 4, 0, 2);
    inserir_arco_ponderado(g, 4, 2, 7);

    int dist[5], pred[5];
    printf("=== Exemplo 1: pesos negativos, sem ciclo negativo (origem s) ===\n");
    if (bellman_ford(g, 0, dist, pred))
        imprimir_resultado(g, dist, pred, r);
    else
        printf("  Ciclo negativo encontrado\n");
    liberar_grafo_ponderado(g);
}

static void exemplo_com_ciclo(void) {
    GrafoListaPonderado *g = criar_grafo_ponderado(5);
    inserir_arco_ponderado(g, 0, 1, 1);
    inserir_arco_ponderado(g, 1, 2, -1);
    inserir_arco_ponderado(g, 2, 3, -1);
    inserir_arco_ponderado(g, 3, 1, -1);
    inserir_arco_ponderado(g, 3, 4, 2);

    int dist[5], pred[5], ciclo[5];
    printf("\n=== Exemplo 2: ciclo 1 -> 2 -> 3 -> 1 com custo -3 (origem 0) ===\n");
    if (bellman_ford(g, 0, dist, pred)) {
        printf("  Sem ciclo negativo\n");
    } else {
        printf("  bellman_ford retornou 0: ha ciclo negativo alcancavel\n  ");
        reconstruir_ciclo_negativo(g, dist, pred, ciclo);
    }
    liberar_grafo_ponderado(g);
}

static void exemplo_inalcancavel(void) {
    const char *r = "ABCD";
    GrafoListaPonderado *g = criar_grafo_ponderado(4);
    inserir_arco_ponderado(g, 0, 1, 4);
    inserir_arco_ponderado(g, 1, 2, -2);
    inserir_arco_ponderado(g, 3, 0, 1);

    int dist[4], pred[4];
    printf("\n=== Exemplo 3: vertice D nao e alcancavel a partir de A ===\n");
    if (bellman_ford(g, 0, dist, pred))
        imprimir_resultado(g, dist, pred, r);
    liberar_grafo_ponderado(g);
}

int main(void) {
    exemplo_sem_ciclo();
    exemplo_com_ciclo();
    exemplo_inalcancavel();
    return 0;
}
