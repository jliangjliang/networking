#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <syscall.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/wait.h>

#define MEMSIZE 1
#define SHSIZE 50

int main()
{
	int shmid;
	key_t key;
	char *shm;
	// char *s;

	key = ftok("shmfile",65); 

	shmid = shmget(key, SHSIZE, IPC_CREAT | 0666);
	// if (shmid < 0)
	// {
	// 	perror("shmget");
	// 	exit(1);
	// }

	shm = shmat(shmid, (void *) 0, 0);

	// memcpy(shm, "Helll World", 11);

	int pid;

	pid = fork();

	if (pid > 0)
	{
		// shm = shmat(shmid, NULL, 0);

		// if(shm == (char *) -1)
		// {
		// 	perror("shtmat");
		// 	exit(1);
		// }

		memcpy(shm, "Helll World", 11); // why use this one?

		// printf("%s\n", shm);

		// s = shm;

		// s+= 11;

		// *s = 0;

		// while(*shm != '*')
		// {
		// 	sleep(1);
		// }

		shmdt(shm);
	}
	if (pid == 0)
	{
		// shm = shmat(shmid, NULL, 0);

		printf("%s\n", shm);

		// for (s = shm; *s != 0; s++)
		// {
		// 	/* code */
		// 	printf("%c", *s);
		// }

		printf("\n");

		// *shm = '*';

		shmdt(shm);

		shmctl(shmid,IPC_RMID,NULL);

		return 0;
	}
	else
	{
		// printf("fork failed\n");
	}

	return 0;
}