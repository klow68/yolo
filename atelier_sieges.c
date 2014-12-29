#include <stdlib.h>
#include <unistd.h>
/*------------------*/
#include <stdio.h>
#include <sys/wait.h>
#include <unistd.h>
#include <errno.h>
#include <sys/mman.h>
#include <sys/shm.h>
#include <signal.h>

#define SKEY (key_t) 1234


/* VARIABLES GLOBALES */
int shmid_nbVoitures;
int *nbVoiture;

/*                    */

void erreur(const char *msg)
{
		perror(msg);
		exit(1);
}


int main(int argc, char *argv[])
{
sigset_t myset;
(void) sigemptyset(&myset);
	printf("yolo\n");

    if(argc - 1 == 1) {
      shmid_nbVoitures = atoi(argv[1]);
    } else {
      erreur("argument : id IPC");
    }

    printf("\n[shmid %d, shmat %d]\n", shmid_nbVoitures, nbVoiture);

/*
     * Locate the segment.
     */
    shmid_nbVoitures = shmget(1234, sizeof(int), 0666);
    if(shmid_nbVoitures < 0) erreur("shmget");

/*
     * Now we attach the segment to our data space.
     */
  	nbVoiture = (int *) shmat(shmid_nbVoitures, NULL, 0);
  	if(nbVoiture == -1) erreur("shmat");


while (1) {
    printf("pause\n");
    sigpause(SIGUSR1);
}



  	printf("\n[shmid %d, shmat %d]\n", shmid_nbVoitures, nbVoiture);

    printf("(atelier_sieges) Nb voiture(s) commandée(s) : %d\n", *nbVoiture);
    if (*nbVoiture!=0){
      printf("\n\nYOU FUCKING WIN\n\n");
    }
    exit(0);

}
