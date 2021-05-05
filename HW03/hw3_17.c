#include <sys/types.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>

#define len 40

typedef struct {
  long fib_seq[len];
  int seq_size;
} shared_fib;

int main(int argc, char *argv[]){
  int num;
  if(argc != 2){
    printf("Input element num is wrong.\n");
    return 0;
  }
  else{
    char *str = argv[1];
    num = strtol(str, NULL, 10);
    if(errno != 0 || num == 0){
      printf("Please insert integer bigger than 0.\n");
      return 0;
    }
  }
  
  int segment_id;
  char *shared_memory;
  const int size = 4096;
  segment_id = shmget(IPC_PRIVATE, size, S_IRUSR | S_IWUSR);
  shared_memory = (char *)shmat(segment_id, NULL, 0);

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
    
    char fib_s[3000] = "";
    char str[30];
    for(int i = 0; i < num; i++){
      sprintf(str, "%d", fib[i]);
      //printf("%d\n", str);
      strcat(fib_s,str);
      strcat(fib_s," ");
    }
    sprintf(shared_memory, fib_s);
    
    printf("Child process-end,pid = %d\n", getpid());
    return 0;
  }/* parent process */
  else{
    printf("Parent process-start,pid = %d\n", getpid());
    printf("Create a shared-memory segament, segment_id = %d\n", segment_id);
    wait(NULL);
    int fib_s[len] = {0};
    printf("%s\n", shared_memory);
    shmdt(shared_memory);
    shmctl(segment_id, IPC_RMID, NULL);
    printf("Parent process-end,pid = %d\n", getpid());
    return 0;
  }
}
