// as server_tcp, but with some changes

#include "user_processing.c"
#include <pthread.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/un.h>
#include <unistd.h>

#define UNIX_PATH_MAX 108

int user_id;

void *user_processor(void *args) {
  int thread_num = user_id;
  int fd_socket = socket(AF_UNIX, SOCK_DGRAM, 0);
  if (fd_socket == -1) {
    perror("Not init");
    return 0;
  }
  struct sockaddr_un server, client;
  int client_size = sizeof(client);

  memset(&server, 0, sizeof(struct sockaddr_un)); // IT'S IMPORTANT
  memset(&client, 0, sizeof(struct sockaddr_un)); // this too
  // fprintf(stderr, "%s", (void *)args);
  server.sun_family = AF_UNIX;
  snprintf(server.sun_path, UNIX_PATH_MAX, "#client.socket%d", thread_num);
  printf("%s - server(second)\n", server.sun_path);
  server.sun_path[0] = 0;
  //  unlink(server.sun_path);

  if (bind(fd_socket, (struct sockaddr *)&server, sizeof(struct sockaddr_un)) ==
      -1) {
    perror("bind error");
    printf("%d %s\n", fd_socket, server.sun_path + 1);
    return 0;
  }
  int data_socket;
  char buffer[30];
  while (1) {
    memset(buffer, 0, 30);
    if (recvfrom(fd_socket, buffer, 30, 0, (struct sockaddr *)&client,
                 &client_size) == -1) {
      perror("recvfrom error");
      return 0;
    }
    if (buffer[0] == '$') {
      close(data_socket);
      return 0;
    }
    func(buffer);
    printf("%s\n", buffer);
    printf("%s\n", client.sun_path + 1);
    if (sendto(fd_socket, buffer, strlen(buffer), 0, (struct sockaddr *)&client,
               client_size) == -1) {
      perror("sendto error");
      return 0;
    }
  }
}

int main() {
  int fd_socket = socket(AF_UNIX, SOCK_DGRAM, 0);
  printf("%d socket fd\n", fd_socket);
  if (fd_socket == -1) {
    perror("Not init");
    return 1;
  }
  struct sockaddr_un server, client;
  int client_size = sizeof(client);
  memset(&server, 0, sizeof(struct sockaddr_un));
  server.sun_family = AF_UNIX;
  snprintf(server.sun_path, UNIX_PATH_MAX, "#main.socket");
  printf("%s - server(first)\n", server.sun_path);
  server.sun_path[0] = 0;
  // printf("(fail)\n");
  if (bind(fd_socket, (struct sockaddr *)&server, sizeof(struct sockaddr_un)) ==
      -1) {
    perror("bind error");
    close(fd_socket);
    return 1;
  }
  int data_socket;
  char buffer[10];
  user_id = 0;
  while (1) {
    memset(buffer, 0, 10);
    if (recvfrom(fd_socket, buffer, 10, 0, (struct sockaddr *)&client,
                 &client_size) == -1) {
      perror("recvfrom error");
      close(fd_socket);
      return 1;
    }
    printf("%s - get\n", buffer);
    user_id++;
    printf("%d - id\n", user_id);
    printf("%s - client\n", client.sun_path + 1);
    sprintf(buffer, "%d", user_id);
    buffer[9] = 0;
    pthread_t thread;
    if (pthread_create(&thread, NULL, user_processor, (void *)&user_id) == -1) {
      perror("creating thread failed");
      close(fd_socket);
      return 1;
    }
    // 100 ms waiting
    usleep(100000);
    if (sendto(fd_socket, buffer, 10, 0, (struct sockaddr *)&client,
               client_size) == -1) {
      perror("sendto error(main)");
      close(fd_socket);
      return 1;
    }
  }
  close(fd_socket);
  return 0;
}
