// hw9_35.c
// most code from hw3_17.c
#include <sys/types.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>

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
    int Catalan[num];
    
    for(int i = 1; i <= num; i++){
        int up = 1, down = 1;
        for(int j = 1; j <= 2*i; j++)     up *= j;
        for(int j = 1; j <= i + 1; j++) down *= j;
        for(int j = 1; j <= i; j++)     down *= j;
        Catalan[i-1] = up / down;
    }
    char Cat_s[3000] = "";
    char str[30];
    for(int i = 0; i < num; i++){
      sprintf(str, "%d", Catalan[i]);
      strcat(Cat_s,str);
      strcat(Cat_s," ");
    }
    sprintf(shared_memory, Cat_s);
    
    return 0;
  }/* parent process */
  else{
    wait(NULL);
    printf("%s\n", shared_memory);
    shmdt(shared_memory);
    shmctl(segment_id, IPC_RMID, NULL);
    printf("Discussion: \n");
    printf("  The different between shared memory and process copy in Time:\n");
    printf("    Shared Memory saves the time for copy all array, which may cause\n");
    printf("    the time usage. And the time usage is increasing with the length\n");
    printf("    of array\n\n");
    printf("  The different between shared memory and process copy in Space:\n");
    printf("    Using the shared memory can decrease the usage of memory, system\n");
    printf("    is not need to allocate double space to the program. That opera-\n");
    printf("    tion will incease the probability of page fault, which also inc-\n");
    printf("    rease the thrasing probability.\n");
    return 0;
  }
}
