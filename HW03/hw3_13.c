#include <sys/types.h>
#include <stdio.h>
#include <unistd.h>
#include <errno.h>

#define len 100

int main(int argc, char *argv[]){
  int num;
  if(argc != 2){
    printf("Input element num is wrong.\n");
    return 0;
  }
  else{
    char *str = argv[1];
    num = strtol(str, NULL, 10);
    if(errno != 0 || num == 0 || num > 30){
      printf("Please insert integer bigger than 0 and smaller than 30.\n");
      return 0;
    }
  }

  pid_t pid;
  pid = fork();
  /* error occurred */
  if(pid < 0){
    fprintf(stderr, "Fork Failed");
    return 1;
  }/* child process */
  else if(pid == 0){
    printf("Child process-start,pid = %d\n", getpid());

    int fib[len] = {0};
    fib[0] = 0; fib[1] = 1;
    
    for(int i = 2; i < num; i++)
      fib[i] = fib[i - 1] + fib[i - 2];
    
    for(int i = 0; i < num - 1; i++)
      printf("%d ", fib[i]);
    printf("%d\n", fib[num-1]);
    
    printf("Child process-end,pid = %d\n", getpid());
    return 0;
  }/* parent process */
  else{
    printf("Parent process-start,pid = %d\n", getpid());
    wait(NULL);
    printf("Parent process-end,pid = %d\n", getpid());
    return 0;
  }
}
