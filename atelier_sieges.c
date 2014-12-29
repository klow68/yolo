#include <stdlib.h>
#include <unistd.h>
/*------------------*/
#include <stdio.h>
#include <sys/wait.h>
#include <unistd.h>
#include <errno.h>

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

	printf("yolo\n");

	int shmid_nbV;

    if(argc - 1 == 1) {
      shmid_nbV = atoi(argv[1]);
    } else {
      erreur("argument : id IPC");
    }

    printf("[shmid %d, shmat %d]\n", shmid_nbVoitures, nbVoiture);

    if(shmid_nbVoitures = shmget(1234, sizeof(int), 0666) < 0)
    	erreur("shmget");
  	if(nbVoiture = (int *) shmat(shmid_nbVoitures, NULL, 0) == - 1)
  		erreur("shmat");

  	printf("[shmid %d, shmat %d]\n", shmid_nbVoitures, nbVoiture);

    printf("(atelier_sieges) Nb voiture(s) commandée(s) : %d", *nbVoiture);

    exit(0);

}
