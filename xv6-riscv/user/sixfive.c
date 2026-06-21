// Lab utilities: print numbers that are multiples of 5 or 6

#include "kernel/types.h"
#include "kernel/stat.h"
#include "kernel/fcntl.h"
#include "user/user.h"

char buf[1024];

void
sixfive(int fd)
{
  int n, m;
  char *p, *q;

  m = 0;
  while ((n = read(fd, buf + m, sizeof(buf) - m - 1)) > 0) {
    m += n;
    buf[m] = '\0';
    p = buf;
    while ((q = strchr(p, '\n')) != 0) {
      /**q = 0;*/
      *q = '\n';
      write(1, p, q + 1 - p);
      p = q + 1;
    }
  }
}

int
main(int argc, char *argv[])
{
  int fd;

  if (argc != 2) {
    fprintf(2, "usage: sixfive file\n");
    exit(1);
  }

  if ((fd = open(argv[1], O_RDONLY)) < 0) {
    printf("sixfive: cannot open %s\n", argv[1]);
    exit(1);
  }
  sixfive(fd);
  close(fd);

  exit(0);
}
