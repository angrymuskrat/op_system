// just reading -> evaluating -> printing in endless cycle
// each process waiting for his flag (flag in shared memory)
// sharing matrix by shared mem too

#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/mman.h>
#include <sys/shm.h>
#include <sys/types.h>
#include <unistd.h>

// flag set 1 in begin
// flag who work: read | eval | write

#define READ 1
#define EVAL 2
#define WRITE 4

// keys
#define shm_flag 100
#define shm_matrix1 200
#define shm_matrix2 300
#define shm_matrix_res 400
#define shm_size 500

int shm_flag_id, shm_size_id, shm_matrix1_id, shm_matrix2_id, shm_matrix_res_id;

int pid_read, pid_eval, pid_print;

// interrupt console handler
void intHandler() {
  printf("bye\n");
  shmctl(shm_flag_id, IPC_RMID, NULL);
  shmctl(shm_size_id, IPC_RMID, NULL);
  shmctl(shm_matrix1_id, IPC_RMID, NULL);
  shmctl(shm_matrix2_id, IPC_RMID, NULL);
  shmctl(shm_matrix_res_id, IPC_RMID, NULL);
  exit(0);
}

void error() { intHandler(); }

void init_mem() {
  shm_flag_id = shmget(shm_flag, 1, IPC_CREAT | 0666);
  shm_size_id = shmget(shm_size, 2, IPC_CREAT | 0666);
  shm_matrix1_id = shmget(shm_matrix1, 256 * 256, IPC_CREAT | 0666);
  shm_matrix2_id = shmget(shm_matrix2, 256 * 256, IPC_CREAT | 0666);
  shm_matrix_res_id = shmget(shm_matrix_res, 256 * 256, IPC_CREAT | 0666);
  if (shm_flag_id < 0 || shm_size_id < 0 || shm_matrix1_id < 0 ||
      shm_matrix2_id < 0 || shm_matrix_res_id < 0) {
    perror("faaail");
    exit(0);
  }
}

void get_mem() {
  shm_flag_id = shmget(shm_flag, 1, 0666);
  shm_size_id = shmget(shm_size, 2, 0666);
  shm_matrix1_id = shmget(shm_matrix1, 256 * 256, 0666);
  shm_matrix2_id = shmget(shm_matrix2, 256 * 256, 0666);
  shm_matrix_res_id = shmget(shm_matrix_res, 256 * 256, 0666);
  if (shm_flag_id < 0 || shm_size_id < 0 || shm_matrix1_id < 0 ||
      shm_matrix2_id < 0 || shm_matrix_res_id < 0) {
    perror("faaail");
    exit(0);
  }
  char *flag = (char *)shmat(shm_flag_id, NULL, 0);
  *flag = 0;
}
void reader() {
  // attaching
  get_mem();
  char *flag, *matrix1, *matrix2, *size;
  flag = (char *)shmat(shm_flag_id, NULL, 0);
  matrix1 = (char *)shmat(shm_matrix1_id, NULL, 0);
  matrix2 = (char *)shmat(shm_matrix2_id, NULL, 0);
  size = (char *)shmat(shm_size_id, NULL, 0);
  *flag = (char)READ;
  while (1) {
    if ((*flag & READ) == 0) {
      usleep(20000);
      continue;
    }
    // printf("start read\n");
    int n = -1, m, i;
    scanf("%i%i", &n, &m);
    // printf("read n*m\n");
    if (n == -1) {
      continue;
    }
    size[0] = n;
    size[1] = m;
    for (i = 0; i < n * m; i++)
      scanf("%d", &(matrix1[i]));
    // printf("%d %d\n", n, m);
    printf("\n");
    for (i = 0; i < n * m; i++)
      scanf("%d", &(matrix2[i]));
    //    printf("read stop");
    *flag = (char)EVAL;
  }
}

void eval() { // attaching
  get_mem();
  char *flag, *matrix1, *matrix2, *matrix_res, *size;
  flag = (char *)shmat(shm_flag_id, NULL, 0);
  matrix1 = (char *)shmat(shm_matrix1_id, NULL, 0);
  matrix2 = (char *)shmat(shm_matrix2_id, NULL, 0);
  matrix_res = (char *)shmat(shm_matrix_res_id, NULL, 0);
  size = (char *)shmat(shm_size_id, NULL, 0);
  while (1) {
    if ((*flag & EVAL) == 0) {
      usleep(20000);
      continue;
    }
    // printf("eval\n");
    int i;
    for (i = 0; i < (int)size[0] * size[1]; i++)
      matrix_res[i] = matrix1[i] + matrix2[i];
    *flag = (char)WRITE;
    // printf("eval stop\n");
  }
}

void print() { // attaching
  get_mem();
  char *flag, *matrix_res, *size;
  flag = (char *)shmat(shm_flag_id, NULL, 0);
  matrix_res = (char *)shmat(shm_matrix_res_id, NULL, 0);
  size = (char *)shmat(shm_size_id, NULL, 0);
  while (1) {
    if ((*flag & WRITE) == 0) {
      usleep(20000);
      continue;
    }
    printf("print\n");
    int i;
    for (i = 0; i < (int)size[0] * size[1]; i++) {
      if (i % size[1] == 0)
        printf("\n");
      printf("%d ", matrix_res[i]);
    }
    fflush(stdout);
    *flag = (char)READ;
  }
}

int main() {
  init_mem();
  signal(SIGINT, intHandler);
  pid_read = fork();
  if (pid_read == -1) {
    error();
  }
  if (pid_read == 0) {
    reader();
  } else {
    pid_eval = fork();
    if (pid_eval == -1) {
      error();
    }
    if (pid_eval == 0) {
      eval();
    } else {
      pid_print = fork();
      if (pid_print == -1) {
        error();
      }
      if (pid_print == 0) {
        print();
      }
    }
  }

  while (1) {
    // lol
  }
  return 0;
}
