#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int
main(int argc, char *argv[])
{
  int p1[2], p2[2], pid;
  char buffer[2]; // one bit
  pipe(p1); pipe(p2);

  if(fork() == 0) {
    //child proc
    close(p1[1]); close(p2[0]);
    read(p1[0], buffer, 1);
    if(buffer[0] == 'a') {
      pid = getpid();
      printf("%d: received ping\n", pid);
      buffer[0] = 'b';
      write(p2[1], buffer, 1);
    } else {
      fprintf(2, "Child receives a wrong bit!\n");
      exit(1);
    }
    close(p1[0]); close(p2[1]);
  } else {
    // parent proc
    close(p1[0]); close(p2[1]);
    buffer[0] = 'a';
    write(p1[1], buffer, 1);
    read(p2[0], buffer, 1);
    if(buffer[0] == 'b') {
      pid = getpid();
      printf("%d: received pong\n", pid);
    } else {
      fprintf(2, "Parent receives a wrong bit!\n");
      exit(1);
    }
    close(p1[1]); close(p2[0]);
  }
  exit(0);
}

