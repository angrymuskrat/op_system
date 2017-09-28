// firstly coonect to main socket
// get name for working socket
// next work with this socket

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>

int main(int argc, char *argv[]) {
  int fd_socket = socket(AF_UNIX, SOCK_SEQPACKET, 0);
  if (fd_socket == -1) {
    perror("Not init");
    return 1;
  }
  struct sockaddr_un name;
  name.sun_family = AF_UNIX;
  sprintf(name.sun_path, "main.socket");
  if (connect(fd_socket, (const struct sockaddr *)&name,
              sizeof(struct sockaddr_un)) == -1) {
    perror("connection error");
    exit(1);
  }
  char id[50];
  read(fd_socket, id, 50);
  // in id now id of socket
  close(fd_socket);

  // real work start
  fd_socket = socket(AF_UNIX, SOCK_SEQPACKET, 0);
  if (fd_socket == -1) {
    perror("Not init");
    return 1;
  }
  name.sun_family = AF_UNIX;
  sprintf(name.sun_path, "%s.socket", id);
  if (connect(fd_socket, (const struct sockaddr *)&name,
              sizeof(struct sockaddr_un)) == -1) {
    perror("connection error");
    exit(1);
  }
  char buffer[30];
  while (1) {
    // fill with zeroes
    memset(buffer, 0, 30);
    // read from console
    scanf("%s", buffer);
    // write to socket
    write(fd_socket, buffer, strlen(buffer));
    // break if user want to stop
    if (buffer[0] == '$') {
      break;
    }
    memset(buffer, 0, 30);
    // get response
    read(fd_socket, buffer, 30);
    // write to console
    printf("%s\n", buffer);
  }
  return 0;
}
