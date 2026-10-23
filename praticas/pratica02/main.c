#include <stdio.h>
#include <stdlib.h>
#include "grafo_lista.h"
#include "busca_largura.h"
#include "busca_profundidade.h"

static GrafoLista *grafo_da_aula(void) {
    GrafoLista *g = criar_grafo_lista(5);
    inserir_aresta_lista(g, 0, 1);
    inserir_aresta_lista(g, 0, 2);
    inserir_aresta_lista(g, 1, 3);
    inserir_aresta_lista(g, 2, 3);
    inserir_aresta_lista(g, 3, 4);
    return g;
}

static void imprimir_caminho(int *pred, int destino, const char *rotulos) {
    if (pred[destino] != -1) {
        imprimir_caminho(pred, pred[destino], rotulos);
        printf(" -> ");
    }
    printf("%c", rotulos[destino]);
}

static void demonstrar_buscas(void) {
    const char *r = "12345";
    GrafoLista *g = grafo_da_aula();
    printf("=== Grafo da Aula 03 ===\n");
    imprimir_grafo_lista(g, r);

    int visitado[5] = {0}, entrada[5], saida[5], tempo = 0;
    dfs_recursiva(g, 0, visitado, entrada, saida, &tempo);
    printf("\nDFS recursiva a partir de 1 (entrada/saida):\n");
    for (int i = 0; i < g->n; i++)
        printf("  %c: %d/%d\n", r[i], entrada[i], saida[i]);

    int ordem[5];
    int k = dfs_iterativa(g, 0, ordem);
    printf("DFS iterativa (pilha) a partir de 1:");
    for (int i = 0; i < k; i++)
        printf(" %c", r[ordem[i]]);

    int dist[5], pred[5];
    bfs(g, 0, dist, pred);
    printf("\n\nBFS a partir de 1 (distancia em arestas):\n");
    for (int i = 0; i < g->n; i++)
        printf("  dist(%c) = %d\n", r[i], dist[i]);
    printf("Menor caminho de 1 ate 5: ");
    imprimir_caminho(pred, 4, r);
    printf("\n");

    printf("\nBipartido? %s\n", eh_bipartido(g) ? "sim" : "nao");
    printf("Tem ciclo? %s\n", tem_ciclo(g) ? "sim" : "nao");
    printf("Componentes: %d\n", contar_componentes(g));
    liberar_grafo_lista(g);
}

static void demonstrar_componentes(void) {
    GrafoLista *g = criar_grafo_lista(6);
    inserir_aresta_lista(g, 0, 1);
    inserir_aresta_lista(g, 1, 2);
    inserir_aresta_lista(g, 3, 4);
    printf("\n=== Grafo da Aula 05: {A,B}, {B,C}, {D,E} e F isolado ===\n");
    printf("Componentes: %d\n", contar_componentes(g));
    printf("Tem ciclo? %s\n", tem_ciclo(g) ? "sim" : "nao");
    liberar_grafo_lista(g);
}

static void demonstrar_triangulo(void) {
    GrafoLista *g = criar_grafo_lista(3);
    inserir_aresta_lista(g, 0, 1);
    inserir_aresta_lista(g, 1, 2);
    inserir_aresta_lista(g, 2, 0);
    printf("\n=== Triangulo (ciclo impar) ===\n");
    printf("Bipartido? %s\n", eh_bipartido(g) ? "sim" : "nao");
    printf("Tem ciclo? %s\n", tem_ciclo(g) ? "sim" : "nao");
    liberar_grafo_lista(g);
}

int main(void) {
    demonstrar_buscas();
    demonstrar_componentes();
    demonstrar_triangulo();
    return 0;
}
