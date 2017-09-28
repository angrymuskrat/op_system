#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

void copyFile(const char *before, const char *after) {
  int fd = open(before, O_RDONLY);
  creat(after, S_IRWXU | S_IRWXG | S_IRWXO);
  int fd_out = open(after, O_WRONLY | O_TRUNC);
  if (fd < 0 || fd_out < 0) {
    perror("fail opening\n");
    return;
  }
  int byte_read, buffer_size = 128;
  char *buffer = (char *)malloc(buffer_size);
  while (byte_read = read(fd, buffer, buffer_size)) {
    write(fd_out, buffer, byte_read);
  }
  free(buffer);
  if (close(fd) < 0 || close(fd_out) < 0) {
    perror("fail closing\n");
  }
}

int main(int argc, char *argv[]) {
  if (argc < 3) {
    printf("how to use:\n");
    printf("copy file to file: cp file1 file2\n");
    printf("copy file to catalog: cp file catalog/\n");
    printf("catalogs from path should be created before\n");
    return 1;
  }
  if (argv[2][strlen(argv[2]) - 1] == '/')
    copyFile(argv[1], strcat(argv[2], argv[1]));
  else
    copyFile(argv[1], argv[2]);
  return 0;
}

