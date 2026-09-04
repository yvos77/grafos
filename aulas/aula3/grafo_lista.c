#include <stdlib.h>
#include "grafo_lista.h"

GrafoLista *criar_grafo(int n)
{
    GrafoLista *g = (GrafoLista *)malloc(sizeof(GrafoLista));
    g->num_vertices = n;
    g->lista = (No **)malloc(sizeof(No *) * n);
    for (int i = 0; i < n; i++)
    {
        // g->lista[i] = (No *)malloc(sizeof(No));
        // g->lista[i]->vertice = i;
        g->lista[i] = NULL;
    }
    return g;
}

void adicionar_aresta(GrafoLista *g, int u, int v)
{
    // u -> v
    No *proximo_u = g->lista[u];
    No *no_u = (No *)malloc(sizeof(No));
    no_u->vertice = v;
    no_u->proximo = proximo_u;
    g->lista[u] = no_u;

    // v -> u
    No *proximo_v = g->lista[v];
    No *no_v = (No *)malloc(sizeof(No));
    no_v->vertice = u;
    no_v->proximo = proximo_v;
    g->lista[v] = no_v;
}
