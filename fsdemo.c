
#include "types.h"
#include "stat.h"
#include "user.h"
#include "fcntl.h"
#include "fs.h"

#define TESTSIZE (BSIZE * 15 + 73)

char *persistfile = "persist.txt";
char *blockfile = "blocks.bin";

void
test_persistence(void)
{
  int pid, fd, n;
  char buf[100];
  struct stat st;

  printf(1, "\n=== FILE PERSISTENCE TEST ===\n");

  pid = fork();

  if(pid < 0){
    printf(1, "fork failed\n");
    return;
  }

  if(pid == 0){
    fd = open(persistfile, O_CREATE | O_WRONLY);
    if(fd < 0){
      printf(1, "child: open failed\n");
      exit();
    }

    if(write(fd, "xv6 file persistence test\n", 26) != 26){
      printf(1, "child: write failed\n");
      close(fd);
      exit();
    }

    close(fd);
    printf(1, "Child: wrote data and closed the file\n");
    exit();
  }

  wait();

  fd = open(persistfile, O_RDONLY);
  if(fd < 0){
    printf(1, "parent: reopen failed\n");
    return;
  }

  n = read(fd, buf, sizeof(buf) - 1);
  close(fd);

  if(n < 0){
    printf(1, "parent: read failed\n");
    return;
  }

  buf[n] = '\0';
  printf(1, "Parent: reopened file and read: %s", buf);

  if(stat(persistfile, &st) == 0){
    printf(1, "stat(): type=%d ino=%d nlink=%d size=%d\n",
           st.type, st.ino, st.nlink, st.size);
  } else {
    printf(1, "stat() failed\n");
  }

  fd = open(persistfile, O_RDONLY);
  if(fd >= 0){
    if(fstat(fd, &st) == 0){
      printf(1, "fstat(): type=%d ino=%d nlink=%d size=%d\n",
             st.type, st.ino, st.nlink, st.size);
    }
    close(fd);
  }
}

void
test_multiblock(void)
{
  int fd, n, total, i, errors;
  char *writebuf;
  char *readbuf;
  struct stat st;

  printf(1, "\n=== MULTI-BLOCK TEST ===\n");

  writebuf = malloc(TESTSIZE);
  readbuf = malloc(TESTSIZE);

  if(writebuf == 0 || readbuf == 0){
    printf(1, "malloc failed\n");
    if(writebuf != 0)
      free(writebuf);
    if(readbuf != 0)
      free(readbuf);
    return;
  }

  for(i = 0; i < TESTSIZE; i++){
    writebuf[i] = 'A' + (i % 26);
    readbuf[i] = 0;
  }

  unlink(blockfile);

  fd = open(blockfile, O_CREATE | O_WRONLY);
  if(fd < 0){
    printf(1, "open for writing failed\n");
    free(writebuf);
    free(readbuf);
    return;
  }

  n = write(fd, writebuf, TESTSIZE);
  close(fd);

  printf(1, "Bytes requested: %d\n", TESTSIZE);
  printf(1, "Bytes written:   %d\n", n);

  if(n != TESTSIZE){
    printf(1, "Write verification failed\n");
    free(writebuf);
    free(readbuf);
    return;
  }

  fd = open(blockfile, O_RDONLY);
  if(fd < 0){
    printf(1, "open for reading failed\n");
    free(writebuf);
    free(readbuf);
    return;
  }

  total = 0;
  while(total < TESTSIZE){
    n = read(fd, readbuf + total, TESTSIZE - total);
    if(n <= 0)
      break;
    total += n;
  }
  close(fd);

  errors = 0;
  if(total == TESTSIZE){
    for(i = 0; i < TESTSIZE; i++){
      if(readbuf[i] != writebuf[i])
        errors++;
    }
  } else {
    errors = 1;
  }

  printf(1, "Bytes read:      %d\n", total);
  printf(1, "Mismatched bytes: %d\n", errors);

  if(total == TESTSIZE && errors == 0)
    printf(1, "PASS: all data matches across block boundaries\n");
  else
    printf(1, "FAIL: data verification unsuccessful\n");

  if(stat(blockfile, &st) == 0){
    printf(1, "File size: %d bytes\n", st.size);
    printf(1, "Inode number: %d\n", st.ino);
  }

  free(writebuf);
  free(readbuf);
}

int
main(void)
{
  test_persistence();
  test_multiblock();

  printf(1, "\n=== FILE-SYSTEM TESTS COMPLETED ===\n");
  exit();
}
