#include <stdio.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <errno.h>
#include <sys/ipc.h>

#include <semaphore.h> // compilation -lpthread -lrt
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/shm.h>


void erreur(const char *msg)
{
		perror(msg);
		exit(1);
}


int main(int argc, char *argv[])
{
	int shmid_nbV;

    if(argc - 1 == 1) {
      shmid_nbV = atoi(argv[1]);
    } else {
      erreur("argument : descripteur d'entrée du tube");
    }

	int *nbVoiture = (int *) shmat(shmid_nbV, NULL, 0);

    printf("Nb voiture(s) commandée(s) : %d", *nbVoiture);

}
