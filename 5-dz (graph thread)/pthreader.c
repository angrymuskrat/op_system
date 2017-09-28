// just using volatile counter
// and waiting for all sons (by join)

#include "graph.c"
#include <pthread.h>
#include <stdio.h>

graph *g;

volatile int number_of_vertex = 0;

void *thread_worker(void *args) {
  ++number_of_vertex;
  int v = *((int *)args), i;
  printf("%i\n", v);
  int *edges = getEdgesList(g, v);
  int *child_thread_arg;
  pthread_t *threads = malloc(sizeof(pthread_t) * edges[0]);
  for (i = 1; i <= edges[0]; i++) {
    child_thread_arg = malloc(sizeof(int));
    *child_thread_arg = edges[i];
    if (pthread_create(&(threads[i - 1]), NULL, thread_worker,
                       (void *)child_thread_arg) != 0) {
      perror("creating error");
      break;
    }
  }
  // join - wait for thread end
  for (i = 1; i <= edges[0]; i++) {
    if (pthread_join(threads[i - 1], NULL)) {
      perror("join error");
    }
  }
  free(threads);
  free(args);
  return 0;
}

int main() {
  g = init();
  pthread_t thread;
  int *init_arg = malloc(sizeof(int));
  if (pthread_create(&thread, NULL, thread_worker, (void *)init_arg) != 0) {
    perror("creating error");
    exit(1);
  }
  pthread_join(thread, NULL);
  printf("%i vertexs\n", number_of_vertex);
  return 0;
}
