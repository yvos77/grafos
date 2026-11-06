#include <stdio.h>
#include <stdlib.h>
#include "agm.h"

static void imprimir_agm(const char *metodo, Aresta *agm, int k, int peso, const char *rotulos) {
    printf("%s (ordem de escolha):", metodo);
    for (int i = 0; i < k; i++)
        printf(" %c-%c(%d)", rotulos[agm[i].u], rotulos[agm[i].v], agm[i].peso);
    printf("\n  %d arestas, peso total = %d\n", k, peso);
}

static void comparar(const char *titulo, int n, Aresta *arestas, int m, const char *rotulos) {
    GrafoListaPonderado *g = criar_grafo_ponderado(n);
    for (int i = 0; i < m; i++)
        inserir_aresta_ponderada(g, arestas[i].u, arestas[i].v, arestas[i].peso);

    Aresta *agm = malloc(n * sizeof(Aresta));
    int peso, k;
    printf("=== %s ===\n", titulo);
    k = kruskal(n, arestas, m, agm, &peso);
    imprimir_agm("Kruskal", agm, k, peso, rotulos);
    if (k < n - 1)
        printf("  Grafo desconexo: Kruskal gerou uma floresta (%d componentes)\n", n - k);
    k = prim(g, 0, agm, &peso);
    imprimir_agm("Prim a partir do primeiro vertice", agm, k, peso, rotulos);
    if (k < n - 1)
        printf("  Prim so alcanca o componente da origem\n");
    printf("\n");
    free(agm);
    liberar_grafo_ponderado(g);
}

static void demonstrar_dsu(void) {
    DSU *d = criar_dsu(5);
    printf("=== Union-Find com 5 elementos ===\n");
    int r1 = unir(d, 0, 1);
    int r2 = unir(d, 2, 3);
    int r3 = unir(d, 1, 3);
    int r4 = unir(d, 0, 2);
    printf("unir(0,1) = %d, unir(2,3) = %d, unir(1,3) = %d, unir(0,2) = %d\n", r1, r2, r3, r4);
    printf("find: ");
    for (int i = 0; i < 5; i++)
        printf("find(%d)=%d ", i, find(d, i));
    printf("\n0 e 3 no mesmo conjunto? %s | 0 e 4? %s\n\n",
           find(d, 0) == find(d, 3) ? "sim" : "nao",
           find(d, 0) == find(d, 4) ? "sim" : "nao");
    liberar_dsu(d);
}

int main(void) {
    demonstrar_dsu();

    Aresta classico[] = {
        {0, 1, 4}, {0, 7, 8}, {1, 2, 8}, {1, 7, 11}, {2, 3, 7}, {2, 5, 4}, {2, 8, 2},
        {3, 4, 9}, {3, 5, 14}, {4, 5, 10}, {5, 6, 2}, {6, 7, 1}, {6, 8, 6}, {7, 8, 7}};
    comparar("Grafo classico com 9 vertices (a..i)", 9, classico, 14, "abcdefghi");

    Aresta pequeno[] = {{0, 1, 1}, {0, 2, 4}, {1, 2, 2}, {1, 3, 6}, {2, 3, 3}};
    comparar("Grafo pequeno A..D", 4, pequeno, 5, "ABCD");

    Aresta desconexo[] = {{0, 1, 5}, {1, 2, 3}, {0, 2, 1}, {3, 4, 2}};
    comparar("Grafo desconexo {A,B,C} e {D,E}", 5, desconexo, 4, "ABCDE");
    return 0;
}
