
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>

#define UNIX_PATH_MAX 108

int main(int argc, char *argv[]) {
  int fd_socket = socket(AF_UNIX, SOCK_DGRAM, 0);
  if (fd_socket == -1) {
    perror("Not init");
    return 1;
  }
  struct sockaddr_un other, me;
  memset(&other, 0, sizeof(other));
  memset(&me, 0, sizeof(me));
  int size_other = sizeof(other);

  other.sun_family = AF_UNIX;
  snprintf(other.sun_path, UNIX_PATH_MAX, "#main.socket");
  printf("%s - server(first)\n", other.sun_path);
  other.sun_path[0] = 0;

  me.sun_family = AF_UNIX;
  sprintf(me.sun_path, "#client.socket%d", getpid());
  printf("%s - client\n", me.sun_path);
  me.sun_path[0] = 0;

  if (bind(fd_socket, (struct sockaddr *)&me, sizeof(struct sockaddr_un))) {
    perror("bind");
    close(fd_socket);
    return 1;
  }
  char id[30];
  memset(id, 0, 30);
  id[0] = 66;
  // read(fd_socket, id, 50);
  if (sendto(fd_socket, id, strlen(id), 0, (struct sockaddr *)&other,
             sizeof(other)) == -1) {
    perror("send fail(1)");
    printf("%s\n", id);
    // return 1;
  }
  if (recvfrom(fd_socket, id, 30, 0, (struct sockaddr *)&other, &size_other) ==
      -1) {
    perror("read fail");
    return 1;
  }
  // real work start
  memset(&other, 0, sizeof(other));
  other.sun_family = AF_UNIX;
  sprintf(other.sun_path, "#client.socket%s", id);
  printf("%s - server(second)\n", other.sun_path);
  other.sun_path[0] = 0;
  char buffer[30];
  while (1) {
    memset(buffer, 0, 30);
    scanf("%s", buffer);
    if (sendto(fd_socket, buffer, strlen(buffer), 0, (struct sockaddr *)&other,
               sizeof(other)) == -1) {
      perror("send fail(2)");
      return 1;
    }
    if (buffer[0] == '$') {
      break;
    }
    memset(buffer, 0, 30);
    if (recvfrom(fd_socket, buffer, 30, 0, (struct sockaddr *)&other,
                 &size_other) == -1) {
      perror("read fail");
      return 1;
    }
    printf("%s\n", buffer);
  }
  return 0;
}
