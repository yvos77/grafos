#ifndef TPS_H
#define TPS_H

#include "hamiltoniano.h"

typedef struct {
    int n;
    int **custo;
} InstanciaTSP;

InstanciaTSP *criar_instancia_tsp(int n);
void liberar_instancia_tsp(InstanciaTSP *t);
int custo_rota(InstanciaTSP *t, int *rota);

int tsp_forca_bruta(InstanciaTSP *t, int *rota);
int tsp_vizinho_mais_proximo(InstanciaTSP *t, int origem, int *rota);
int tsp_insercao(InstanciaTSP *t, int *rota);
InstanciaTSP *grafo_para_tsp(GrafoMatriz *g);

#endif
