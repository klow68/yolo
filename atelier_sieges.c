#include <stdlib.h>
#include <unistd.h>
/*------------------*/
#include <stdio.h>
#include <sys/wait.h>
#include <unistd.h>
#include <errno.h>


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

	int *nbVoiture = (int *) shmat(shmid_nbV, NULL, 0);

    printf("Nb voiture(s) commandée(s) : %d", *nbVoiture);

    exit(0);

}
