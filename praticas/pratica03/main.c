#include <stdio.h>
#include <stdlib.h>
#include "grafo_lista.h"
#include "dag.h"

static void imprimir_ordenacao(const char *titulo, int *ordem, int tamanho, const char *rotulos) {
    printf("%s:", titulo);
    if (ordem == NULL) {
        printf(" impossivel (o digrafo tem ciclo)\n");
        return;
    }
    for (int i = 0; i < tamanho; i++)
        printf(" %c", rotulos[ordem[i]]);
    printf("\n");
}

static void analisar(const char *nome, GrafoLista *g, const char *rotulos) {
    int tamanho;
    printf("=== %s ===\n", nome);
    imprimir_grafo_lista(g, rotulos);
    printf("E um DAG? %s\n", eh_dag(g) ? "sim" : "nao");

    int *kahn = ordenacao_topologica_kahn(g, &tamanho);
    imprimir_ordenacao("Kahn (BFS)", kahn, tamanho, rotulos);
    free(kahn);

    int *dfs = ordenacao_topologica_dfs(g, &tamanho);
    imprimir_ordenacao("DFS (pilha de termino)", dfs, tamanho, rotulos);
    free(dfs);
    printf("\n");
}

int main(void) {
    GrafoLista *aula = criar_grafo_lista(4);
    inserir_arco_lista(aula, 0, 1);
    inserir_arco_lista(aula, 0, 2);
    inserir_arco_lista(aula, 1, 3);
    inserir_arco_lista(aula, 2, 3);
    analisar("DAG da Aula 04", aula, "ABCD");
    liberar_grafo_lista(aula);

    GrafoLista *modulos = criar_grafo_lista(6);
    inserir_arco_lista(modulos, 1, 0);
    inserir_arco_lista(modulos, 2, 0);
    inserir_arco_lista(modulos, 3, 1);
    inserir_arco_lista(modulos, 3, 2);
    inserir_arco_lista(modulos, 4, 2);
    inserir_arco_lista(modulos, 5, 3);
    inserir_arco_lista(modulos, 5, 4);
    analisar("Modulos da Aula 01 (arco X -> Y: X deve vir antes de Y)", modulos, "ABCDEF");
    liberar_grafo_lista(modulos);

    GrafoLista *ciclo = criar_grafo_lista(3);
    inserir_arco_lista(ciclo, 0, 1);
    inserir_arco_lista(ciclo, 1, 2);
    inserir_arco_lista(ciclo, 2, 0);
    analisar("Digrafo com ciclo A -> B -> C -> A", ciclo, "ABC");
    liberar_grafo_lista(ciclo);
    return 0;
}
