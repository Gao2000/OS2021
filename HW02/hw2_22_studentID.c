#define _POSIX_SOURCE
#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/ptrace.h>
#include<sys/types.h>
#include<sys/wait.h>
#include<sys/reg.h>

int main(int argc,char **argv)
{
	pid_t child_pid;
	int p_status;
	long syscall;

	child_pid = fork();

	if(child_pid == 0) // child process
	{
		ptrace(PTRACE_TRACEME, child_pid, NULL, NULL);
		kill(getpid(), SIGSTOP);
	
		// Implement your file copy code
	
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
