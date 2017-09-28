// multithread server
// for each client new thread
// first connect to main socket, wich say for user thread id
// next, user connect to socket with name "%d.socket" (in same package as
// server)
// user can end connection by sending $

#include "user_processing.c"
#include <pthread.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>

// function for each user processing
// it's runs in new thread
void *user_processor(void *args) {
  // get thread id for unique socket name
  int thread_num = *((int *)args);
  int fd_socket = socket(AF_UNIX, SOCK_SEQPACKET, 0);
  if (fd_socket == -1) {
    perror("Not init");
    return 0;
  }
  struct sockaddr_un name;
  name.sun_family = AF_UNIX;
  sprintf(name.sun_path, "%d.socket", thread_num);
  // delete socket with this name
  unlink(name.sun_path);
  if (bind(fd_socket, (const struct sockaddr *)&name,
           sizeof(struct sockaddr_un)) == -1) {
    perror("bind error");
    return 0;
  }
  if (listen(fd_socket, 20) == -1) {
    perror("listen fail");
    return 0;
  }
  int data_socket;
  char buffer[30];
  data_socket = accept(fd_socket, NULL, NULL);
  if (data_socket == -1) {
    perror("accept err");
    return 0;
  }
  while (1) {
    memset(buffer, 0, 30);
    if (read(data_socket, buffer, 30) == -1) {
      perror("read fail");
      return 0;
    }
    if (buffer[0] == '$') {
      close(data_socket);
      return 0;
    }
    // do something with received string
    func(buffer);
    if (write(data_socket, buffer, 30) == -1) {
      perror("write err");
      return 0;
    }
  }
}

int main() {
  int fd_socket = socket(AF_UNIX, SOCK_SEQPACKET, 0);
  if (fd_socket == -1) {
    perror("Not init");
    return 1;
  }
  struct sockaddr_un name;
  name.sun_family = AF_UNIX;
  strcpy(name.sun_path, "main.socket");
  unlink(name.sun_path);
  if (bind(fd_socket, (const struct sockaddr *)&name,
           sizeof(struct sockaddr_un)) == -1) {
    perror("bind error");
    return 1;
  }
  if (listen(fd_socket, 20) == -1) {
    perror("listen fail");
    return 1;
  }
  int data_socket;
  char *buffer = malloc(10);
  int user_id = 0;
  // endless cycle for get first connect for each user
  // and route to working socket
  while (1) {
    data_socket = accept(fd_socket, NULL, NULL);
    if (data_socket == -1) {
      perror("accept err");
      return 1;
    }
    user_id++;
    printf("%d", user_id);
    sprintf(buffer, "%d", user_id);
    buffer[9] = 0;
    pthread_t thread;
    if (pthread_create(&thread, NULL, user_processor, (void *)&user_id) == -1) {
      perror("creating thread failed");
      return 1;
    }
    // lol sleep need because thread create socket slower than clinet try to
    // connect
    usleep(1000);
    if (write(data_socket, buffer, 10) == -1) {
      perror("writing error");
      return 1;
    }
    close(data_socket);
  }
  return 0;
}
