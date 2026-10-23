#include <stdio.h>
#include <stdlib.h>
#include "dijkstra.h"

#define N 7
#define ORIGEM 0

static void testar_min_heap(void) {
    printf("=== TESTE DO MIN-HEAP ===\n\n");

    MinHeap *h = criar_min_heap(6);
    if (h == NULL) {
        printf("Falha ao alocar o heap.\n");
        return;
    }

    int chaves[] = {40, 15, 60, 25, 10, 50};
    for (int v = 0; v < 6; v++) inserir_min_heap(h, v, chaves[v]);

    printf("Inseridos (vertice:distancia):");
    for (int v = 0; v < 6; v++) printf(" %d:%d", v, chaves[v]);
    printf("\n");

    printf("diminuir_chave(2, 5)  -> %d\n", diminuir_chave(h, 2, 5));
    printf("diminuir_chave(2, 90) -> %d (nao diminui)\n", diminuir_chave(h, 2, 90));

    printf("Ordem de extracao:");
    while (!heap_vazio(h)) {
        NoHeap x = extrair_minimo(h);
        printf(" %d(%d)", x.vertice, x.distancia);
    }
    printf("\n");

    NoHeap vazio = extrair_minimo(h);
    printf("Extracao em heap vazio -> vertice %d\n", vazio.vertice);

    liberar_min_heap(h);
    printf("\n");
}

static GrafoListaPonderado *montar_grafo(void) {
    GrafoListaPonderado *g = criar_grafo_ponderado(N);
    if (g == NULL) return NULL;

    inserir_aresta_ponderada(g, 0, 1, 4);
    inserir_aresta_ponderada(g, 0, 2, 1);
    inserir_aresta_ponderada(g, 1, 2, 2);
    inserir_aresta_ponderada(g, 1, 3, 5);
    inserir_aresta_ponderada(g, 2, 3, 8);
    inserir_aresta_ponderada(g, 2, 4, 10);
    inserir_aresta_ponderada(g, 3, 4, 2);
    inserir_aresta_ponderada(g, 3, 5, 6);
    inserir_aresta_ponderada(g, 4, 5, 2);

    return g;
}

static void testar_dijkstra(void) {
    printf("=== DIJKSTRA ===\n\n");

    GrafoListaPonderado *g = montar_grafo();
    if (g == NULL) {
        printf("Falha ao alocar o grafo.\n");
        return;
    }

    printf("Lista de adjacencia ponderada:\n");
    imprimir_grafo_ponderado(g);

    int dist[N], pred[N];
    if (!dijkstra(g, ORIGEM, dist, pred)) {
        printf("Dijkstra falhou.\n");
        liberar_grafo_ponderado(g);
        return;
    }

    printf("\nResultado a partir do vertice %d:\n", ORIGEM);
    printf("  v   dist   pred\n");
    for (int v = 0; v < N; v++) {
        if (dist[v] == INFINITO) printf("%3d    inf   %4d\n", v, pred[v]);
        else printf("%3d   %4d   %4d\n", v, dist[v], pred[v]);
    }

    printf("\nCaminhos minimos:\n");
    for (int v = 0; v < N; v++) imprimir_caminho(ORIGEM, v, dist, pred);

    printf("\nExportacao Graphviz:\n");
    printf("  arvore.dot      -> %d\n", exportar_dot(g, ORIGEM, -1, dist, pred, "arvore.dot"));
    printf("  caminho_0_3.dot -> %d\n", exportar_dot(g, ORIGEM, 3, dist, pred, "caminho_0_3.dot"));
    printf("  gere a imagem com: dot -Tpng arvore.dot -o arvore.png\n");

    printf("\nCasos invalidos:\n");
    printf("  dijkstra com origem 99 -> %d\n", dijkstra(g, 99, dist, pred));
    printf("  inserir aresta de peso -3 -> %d\n", inserir_aresta_ponderada(g, 0, 5, -3));
    printf("  inserir laco (2,2) -> %d\n", inserir_aresta_ponderada(g, 2, 2, 7));

    int *sem_caminho = reconstruir_caminho(ORIGEM, 6, pred);
    printf("  reconstruir_caminho(0, 6) -> %s\n", sem_caminho == NULL ? "NULL" : "caminho");
    free(sem_caminho);

    liberar_grafo_ponderado(g);
    printf("\nMemoria liberada.\n");
}

int main(void) {
    testar_min_heap();
    testar_dijkstra();
    return 0;
}