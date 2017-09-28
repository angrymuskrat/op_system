#include <stdlib.h>

typedef struct { int **edges; } graph;

graph *create(int n) {
  graph *a = (graph *)malloc(sizeof(graph));
  a->edges = malloc(n * sizeof(int *));
  int i;
  for (i = 0; i < n; i++) {
    a->edges[i] = malloc((n + 1) * sizeof(int *));
    a->edges[i][0] = 0;
  }
  return a;
}

void addEdge(graph *g, int from, int to) {
  int pos = ++(g->edges[from][0]);
  g->edges[from][pos] = to;
}

graph *init() {
  graph *g = create(10);
  addEdge(g, 0, 9);
  addEdge(g, 9, 2);
  addEdge(g, 9, 3);
  addEdge(g, 2, 5);
  addEdge(g, 2, 4);
  addEdge(g, 4, 6);
  addEdge(g, 4, 7);
  addEdge(g, 4, 8);
  addEdge(g, 4, 1);
  return g;
}

int *getEdgesList(graph *g, int v) { return g->edges[v]; }
