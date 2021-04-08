#define _POSIX_SOURCE
#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/ptrace.h>
#include<sys/types.h>
#include<sys/wait.h>
#include<sys/reg.h>

#include<fcntl.h>
#include<signal.h>
#include<string.h>

int main(int argc,char **argv)
{
	pid_t child_pid;
	int p_status;
	long syscall;
  //
  int fd = -1, fi = -1;
  char fileNameI[21] = "", fileNameD[21] = "";
  ssize_t size = -1;
  char buf[100];

	child_pid = fork();

	if(child_pid == 0) // child process
	{
	  ptrace(PTRACE_TRACEME, child_pid, NULL, NULL);
	  kill(getpid(), SIGSTOP);
	  
		// Implement your file copy code
    // Open the input file
    while(1){
      printf("Please input the source file name:\n");
      scanf("%s", &fileNameI);
      fi = open(fileNameI, O_RDONLY);
      if(-1 == fi){
        printf("Failed to open file %s: %d\n", fileNameI, fi);
        continue;;
      }else{
        printf("Success to open file %s\n", fileNameI);
        break;
      }
    }
    // Open the dst file
    while(1){
      printf("Please input the destination file name:\n");
      scanf("%s", &fileNameD);
      fd = open(fileNameD, O_CREAT | O_EXCL | O_RDWR,  0777); // 0777 can R/W for all usr
      if(-1 == fd){
        printf("Failed to open file %s: %d\n", fileNameD, fd);
        printf("The file is exist, please enter the new name.\n");
        continue;
      }else{
        printf("Success to open file %s\n", fileNameD);
        break;
      }
    }
    // Copy file
    while(size){
      size = read(fi,buf,100);
      if(-1 == size){
        printf("Read file %s error occurs\n", fileNameD);
        break;
      }else{
        if(size > 0){
          //printf("\n//----test: %s----//\n", buf);
          write(fd,buf,strlen(buf));
        }else
          printf("Reach the end of file\n");
      }
    }
    // END of copy
    close(fi); close(fd);
    printf("%s has generated\n", fileNameD);
		exit(0);
	}
	else if(child_pid > 0) // parent process
		while(waitpid(child_pid, &p_status, 0))
			if(WIFEXITED(p_status))
				break;
			else
			{
				syscall = ptrace(PTRACE_PEEKUSER, child_pid, sizeof(long)*ORIG_RAX, NULL);
				printf("system call: %ld\n", syscall);
				ptrace(PTRACE_SYSCALL, child_pid, NULL, NULL);
			}
	else
		printf("fork error\n");

	return 0;
}
