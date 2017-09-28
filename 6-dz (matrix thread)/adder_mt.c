#include <pthread.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define READ 1
#define EVAL 2
#define WRITE 4

volatile int *matrix1, *matrix2, *matrix_res, *size, *flag;
pthread_t reader, evaluator, printer;

void intHandler() {
  printf("bye\n");
  pthread_kill(reader, 15);
  pthread_kill(evaluator, 15);
  pthread_kill(printer, 15);
  free(matrix1);
  free(matrix2);
  free(matrix_res);
  free(size);
  free(flag);
}

void *reading(void *args) {
  while (1) {
    if (*flag != READ) {
      usleep(200000);
      continue;
    }
    scanf("%i%i", &(size[0]), &(size[1]));
    matrix1 = malloc(sizeof(int) * size[0] * size[1]);
    matrix2 = malloc(sizeof(int) * size[0] * size[1]);
    matrix_res = malloc(sizeof(int) * size[0] * size[1]);
    int i;
    for (i = 0; i < size[0] * size[1]; i++)
      scanf("%i", &(matrix1[i]));
    for (i = 0; i < size[0] * size[1]; i++)
      scanf("%i", &(matrix2[i]));
    *flag = EVAL;
  }
}

void *evaluating(void *args) {
  while (1) {
    if (*flag != EVAL) {
      usleep(200000);
      continue;
    }
    int i;
    for (i = 0; i < size[0] * size[1]; i++) {
      matrix_res[i] = matrix1[i] + matrix2[i];
    }
    *flag = WRITE;
  }
}

void *printing(void *args) {
  while (1) {
    if (*flag != WRITE) {
      usleep(200000);
      continue;
    }
    int i;
    for (i = 0; i < (int)size[0] * size[1]; i++) {
      if (i % size[1] == 0)
        printf("\n");
      printf("%i ", matrix_res[i]);
    }
    fflush(stdout);
    *flag = (char)READ;
    free(matrix1);
    free(matrix2);
    free(matrix_res);
  }
}

int main() {
  size = malloc(sizeof(int) * 2);
  flag = malloc(sizeof(int));
  signal(SIGINT, intHandler);
  if (pthread_create(&reader, NULL, reading, NULL) != 0 ||
      pthread_create(&evaluator, NULL, evaluating, NULL) != 0 ||
      pthread_create(&printer, NULL, printing, NULL) != 0) {
    perror("error");
    return 1;
  }
  *flag = READ;
  pthread_join(reader, NULL);
  return 0;
}
