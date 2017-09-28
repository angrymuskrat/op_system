// write to pipe 0
// start from first vertex
// when we in new vertex, read from pipe x and write to pipe (x + 1)
// next wait for stop of childs process
// stop process

#include "graph.c"
#include <errno.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

int main() {
  int v = 0;
  graph *g = init();
  int fd[2];
  pipe(fd);
  int before = 0;
  write(fd[1], &before, sizeof(int));
  while (1) {
    printf("I'm %i\n", v);
    printf("my pid %i\n", getpid());
    usleep(1000000);
    if (getEdgesList(g, v)[0] == 0) {
      exit(0);
    }
    int *edges = getEdgesList(g, v);
    int i, is_son = 0;
    for (i = 1; i <= edges[0]; i++) {
      int son = edges[i];
      int pid = fork();
      is_son = 0;
      switch (pid) {
      // error
      case -1:
        perror("fork error\n");
        exit(1);
        break;
      // son
      case 0:
        is_son = 1;
        v = son;
        int how_many;
        // read from pipe
        read(fd[0], &how_many, sizeof(int));
        how_many++;
        // write to pipe
        write(fd[1], &how_many, sizeof(int));
        break;
      }
      if (is_son == 1)
        break;
    }
    if (is_son == 0) {
      int status, pid;
      // ECHILD - no child process
      while (pid = wait(&status)) {
        if (status == ECHILD || (pid == -1 && errno == ECHILD))
          break;
      }
      if (v == 0)
        break;
      exit(0);
    }
  }
  int res;
  read(fd[0], &res, sizeof(int));
  // res + 1 because first vertex not counted
  printf("%d\n", res + 1);
  return 0;
}
