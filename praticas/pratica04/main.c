#include <stdio.h>
#include <stdlib.h>
#include "grafo_lista.h"
#include "conectividade.h"
#include "planaridade.h"

static GrafoLista *grafo_completo(int n) {
    GrafoLista *g = criar_grafo_lista(n);
    for (int i = 0; i < n; i++)
        for (int j = i + 1; j < n; j++)
            inserir_aresta_lista(g, i, j);
    return g;
}

static GrafoLista *grafo_de_arestas(int n, int arestas[][2], int m) {
    GrafoLista *g = criar_grafo_lista(n);
    for (int i = 0; i < m; i++)
        inserir_aresta_lista(g, arestas[i][0], arestas[i][1]);
    return g;
}

static void demonstrar_conectividade(void) {
    int arestas[][2] = {{0, 1}, {1, 2}, {2, 0}, {1, 3}, {3, 4}};
    GrafoLista *g = grafo_de_arestas(5, arestas, 5);
    printf("=== Conectividade: triangulo 0-1-2 com cauda 1-3-4 ===\n");
    imprimir_grafo_lista(g, "01234");

    int eh_articulacao[5];
    int total = encontrar_articulacoes(g, eh_articulacao);
    printf("Vertices de corte (%d):", total);
    for (int i = 0; i < g->n; i++)
        if (eh_articulacao[i]) printf(" %d", i);

    Aresta pontes[5];
    int num_pontes = detectar_pontes(g, pontes);
    printf("\nPontes (%d):", num_pontes);
    for (int i = 0; i < num_pontes; i++)
        printf(" {%d,%d}", pontes[i].u, pontes[i].v);
    printf("\n\n");
    liberar_grafo_lista(g);
}

static void relatar_planaridade(const char *nome, GrafoLista *g) {
    int m = contar_arestas(g);
    int euler = eh_planar_euler(g);
    int resultado = eh_planar(g);
    printf("%-22s n=%2d m=%2d | m <= 3n-6? %-3s | K5: %-3s | K3,3: %-3s | planar: %s",
           nome, g->n, m, euler ? "sim" : "nao",
           contem_subdivisao_k5(g) ? "sim" : "nao",
           contem_subdivisao_k33(g) ? "sim" : "nao",
           resultado == 1 ? "sim" : resultado == 0 ? "nao" : "inconclusivo");
    if (resultado == 1)
        printf(" (faces = %d)", numero_de_faces(g));
    printf("\n");
    liberar_grafo_lista(g);
}

static void demonstrar_planaridade(void) {
    int quadrado[][2] = {{0, 1}, {1, 2}, {2, 3}, {3, 0}};
    int k33[][2] = {{0, 3}, {0, 4}, {0, 5}, {1, 3}, {1, 4}, {1, 5}, {2, 3}, {2, 4}, {2, 5}};
    int cubo[][2] = {{0, 1}, {1, 2}, {2, 3}, {3, 0}, {4, 5}, {5, 6}, {6, 7}, {7, 4},
                     {0, 4}, {1, 5}, {2, 6}, {3, 7}};
    int petersen[][2] = {{0, 1}, {1, 2}, {2, 3}, {3, 4}, {4, 0}, {0, 5}, {1, 6}, {2, 7},
                         {3, 8}, {4, 9}, {5, 7}, {7, 9}, {9, 6}, {6, 8}, {8, 5}};
    int k5_subdividido[][2] = {{0, 5}, {5, 1}, {0, 2}, {0, 3}, {0, 4}, {1, 2}, {1, 3},
                               {1, 4}, {2, 3}, {2, 4}, {3, 4}};

    printf("=== Planaridade ===\n");
    relatar_planaridade("Quadrado (Aula 05)", grafo_de_arestas(4, quadrado, 4));
    relatar_planaridade("K4", grafo_completo(4));
    relatar_planaridade("Cubo", grafo_de_arestas(8, cubo, 12));
    relatar_planaridade("K5", grafo_completo(5));
    relatar_planaridade("K3,3", grafo_de_arestas(6, k33, 9));
    relatar_planaridade("K5 subdividido", grafo_de_arestas(6, k5_subdividido, 11));
    relatar_planaridade("Petersen", grafo_de_arestas(10, petersen, 15));
}

int main(void) {
    demonstrar_conectividade();
    demonstrar_planaridade();
    return 0;
}
