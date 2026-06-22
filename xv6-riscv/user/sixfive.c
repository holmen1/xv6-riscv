// Lab utilities: print numbers that are multiples of 5 or 6

#include "kernel/types.h"
#include "kernel/fcntl.h"
#include "user/user.h"

static void flush_token(char *linebuf, int len, int in_token, int token_is_numeric);

// Read file character-by-character, accumulate numeric tokens
void
sixfive(int fd)
{
  char c;
  char linebuf[64];  // Accumulate current token
  int len = 0;       // Length of current token
  int in_token = 0;  // True if we're building a token
  int token_is_numeric = 1;  // True if all chars so far are digits

  while (read(fd, &c, 1) > 0) {
    if (strchr(" -\r\t\n./,", c)) {
      // Separator: flush pending token if valid.
      flush_token(linebuf, len, in_token, token_is_numeric);
      len = 0;
      in_token = 0;
      token_is_numeric = 1;
    } else {
      // Non-separator: accumulate character if it's a digit.
      in_token = 1;
      if (c >= '0' && c <= '9') {
        if (len < sizeof(linebuf) - 1)
          linebuf[len++] = c;
        else
          token_is_numeric = 0;  // Token too long; reject it.
      } else {
        token_is_numeric = 0;  // Non-digit in token; reject it.
      }
    }
  }

  // Flush any pending token at end-of-file.
  flush_token(linebuf, len, in_token, token_is_numeric);
}

int
main(int argc, char *argv[])
{
  int fd, i;

  if (argc < 2) {
    fprintf(2, "usage: sixfive file [file...]\n");
    exit(1);
  }

  for(i = 1; i < argc; i++){
    if((fd = open(argv[i], O_RDONLY)) < 0){
      fprintf(2, "sixfive: cannot open %s\n", argv[i]);
      exit(1);
    }
    sixfive(fd);
    close(fd);
  }

  exit(0);
}

// Parse and print token if it is a valid, complete, numeric string
// that is divisible by 5 or 6.
static void
flush_token(char *linebuf, int len, int in_token, int token_is_numeric)
{
  int num;

  // Accept only tokens that are non-empty, all-digit, and in progress.
  if (!(in_token && token_is_numeric && len > 0))
    return;

  linebuf[len] = '\0';
  num = atoi(linebuf);
  if (num % 5 == 0 || num % 6 == 0)
    fprintf(1, "%d\n", num);
}
