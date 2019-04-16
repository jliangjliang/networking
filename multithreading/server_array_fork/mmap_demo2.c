#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/file.h>
#include <sys/mman.h>
#include <sys/wait.h>
#include <semaphore.h>
#include <errno.h>

#define NUMWORKER 2
#define PORT 25969
#define LISTENSIZE 2
#define BUFFERSIZE 2000
#define QUEUECAPACITY 2
#define MEMSIZE 1024

/* declare functions */
struct array_queue {
	int head;
    int tail;
    int count;
    int capacity;
    int *array;
    sem_t mutex;
};

struct array_queue shared;

int main()
{	
	// struct shared shared;
	int fd, i, nloop;
	struct array_queue *ptr;
	const char *memname = "mem";

	nloop = 100;
	fd = shm_open(memname, O_CREAT | O_TRUNC | O_RDWR, 0666);
	write(fd, &shared, sizeof(struct array_queue)); // Write or write
	ptr = mmap(NULL, sizeof(struct array_queue), PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
	fprintf(stderr, "mmap failed: %d %s\n", errno, strerror(errno));
	close(fd);

	ptr->head = 0;
	ptr->tail = 0;
	ptr->count = 0;
	ptr->capacity = QUEUECAPACITY;
	ptr->array = (int *) malloc(QUEUECAPACITY*sizeof(int));
	sem_init(&ptr->mutex, 1, 1);

	setbuf(stdout, NULL);

	if (fork() > 0)
	{
		for (i = 0; i < nloop; i++)
		{
			/* code */
			sem_wait(&ptr->mutex);
			printf("parent: %d\n", ptr->count++);
			sem_post(&ptr->mutex);
		}
		exit(0);
	}
	else
	{
		for (i = 0; i < nloop; i++)
		{
			/* code */
			sem_wait(&ptr->mutex);
			printf("chile: %d\n", ptr->count++);
			sem_post(&ptr->mutex);
		}
		exit(0);
	}

	return 0;
}