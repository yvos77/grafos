#include <stdio.h>
#include "grafo_matriz.h"
#include "grafo_lista.h"

#define N 6

static void testar_matriz(void) {
    printf("=== MATRIZ DE ADJACENCIA ===\n\n");

    GrafoMatriz *g = criar_grafo_matriz(N);
    if (g == NULL) {
        printf("Falha ao alocar o grafo.\n");
        return;
    }

    inserir_aresta_matriz(g, 0, 1);
    inserir_aresta_matriz(g, 0, 2);
    inserir_aresta_matriz(g, 1, 2);
    inserir_aresta_matriz(g, 2, 3);
    inserir_aresta_matriz(g, 3, 4);
    inserir_aresta_matriz(g, 4, 0);

    printf("Grafo apos as insercoes:\n");
    imprimir_grafo_matriz(g);

    printf("\nGraus:\n");
    for (int i = 0; i < N; i++) printf("  grau(%d) = %d\n", i, grau_matriz(g, i));

    printf("\n0 e 1 sao adjacentes? %s\n", sao_adjacentes_matriz(g, 0, 1) ? "sim" : "nao");
    printf("0 e 3 sao adjacentes? %s\n", sao_adjacentes_matriz(g, 0, 3) ? "sim" : "nao");

    printf("\nRemovendo a aresta (0,2)...\n");
    remover_aresta_matriz(g, 0, 2);
    imprimir_grafo_matriz(g);
    printf("grau(0) = %d\n", grau_matriz(g, 0));

    printf("\nCasos invalidos:\n");
    printf("  inserir (0,0)  -> %d\n", inserir_aresta_matriz(g, 0, 0));
    printf("  inserir (0,1)  -> %d (ja existe)\n", inserir_aresta_matriz(g, 0, 1));
    printf("  inserir (0,99) -> %d\n", inserir_aresta_matriz(g, 0, 99));
    printf("  remover (1,4)  -> %d (inexistente)\n", remover_aresta_matriz(g, 1, 4));
    printf("  grau(-1)       -> %d\n", grau_matriz(g, -1));

    liberar_grafo_matriz(g);
    printf("\nMemoria liberada.\n\n");
}

static void testar_lista(void) {
    printf("=== LISTA DE ADJACENCIA ===\n\n");

    GrafoLista *g = criar_grafo_lista(N);
    if (g == NULL) {
        printf("Falha ao alocar o grafo.\n");
        return;
    }

    inserir_aresta_lista(g, 0, 1);
    inserir_aresta_lista(g, 0, 2);
    inserir_aresta_lista(g, 1, 2);
    inserir_aresta_lista(g, 2, 3);
    inserir_aresta_lista(g, 3, 4);
    inserir_aresta_lista(g, 4, 0);

    printf("Grafo apos as insercoes:\n");
    imprimir_grafo_lista(g);

    printf("\nGraus:\n");
    for (int i = 0; i < N; i++) printf("  grau(%d) = %d\n", i, grau_lista(g, i));

    printf("\n0 e 1 sao adjacentes? %s\n", sao_adjacentes_lista(g, 0, 1) ? "sim" : "nao");
    printf("0 e 3 sao adjacentes? %s\n", sao_adjacentes_lista(g, 0, 3) ? "sim" : "nao");

    printf("\nRemovendo a aresta (0,2)...\n");
    remover_aresta_lista(g, 0, 2);
    imprimir_grafo_lista(g);
    printf("grau(0) = %d\n", grau_lista(g, 0));

    printf("\nCasos invalidos:\n");
    printf("  inserir (0,0)  -> %d\n", inserir_aresta_lista(g, 0, 0));
    printf("  inserir (0,1)  -> %d (ja existe)\n", inserir_aresta_lista(g, 0, 1));
    printf("  inserir (0,99) -> %d\n", inserir_aresta_lista(g, 0, 99));
    printf("  remover (1,4)  -> %d (inexistente)\n", remover_aresta_lista(g, 1, 4));
    printf("  grau(-1)       -> %d\n", grau_lista(g, -1));

    liberar_grafo_lista(g);
    printf("\nMemoria liberada.\n");
}

int main(void) {
    testar_matriz();
    testar_lista();
    return 0;
}