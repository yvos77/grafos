#ifndef PLANARIDADE_H
#define PLANARIDADE_H

#include "grafo_lista.h"

#define MAX_KURATOWSKI 10

int contar_arestas(GrafoLista *g);
int eh_planar_euler(GrafoLista *g);
int numero_de_faces(GrafoLista *g);
int contem_subdivisao_k5(GrafoLista *g);
int contem_subdivisao_k33(GrafoLista *g);
int eh_planar(GrafoLista *g);

#endif
