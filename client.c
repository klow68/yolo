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

#include <stdlib.h>
#include <sys/wait.h>

#define IFLAGS (SEMPERM | IPC_CREAT)
#define SKEY   (key_t) IPC_PRIVATE	
#define SEMPERM 0600				  /* Permission */

/* VARIABLES GLOBALES */
int shmid_nbVoitures;
int *nbVoiture;

/*                    */
void erreur(const char *msg)
{
		perror(msg);
		exit(1);
}

pid_t atelierSieges()
{
	pid_t pid;

	switch (pid = fork()) 

	{

		case (pid_t) 0:
			/* on est dans le processus fils */
			// lancer l'atelier de production de sièges
			printf("valeur de fork = %d \n", pid);
			printf("je suis le processus fils %d de pere %d\n", getpid(), getppid());

			char c = (char)(((int)'0')+shmid_nbVoitures);
			char *ch = &c;
			if (execl("./atelier_sieges", "atelier_sieges", ch, NULL) == -1)
                  erreur("execl");

			printf("fin du processus fils\n");
			exit(0);/* id		em */

		default:
			/* on est dans le processus pere */
			wait(NULL); // on attend qu'un processus fils meurt ou ne s'execute pas si plus de fils
			printf("valeur de fork = %d \n", pid);
			printf("je suis le processus pere %d et mon grand pere est : %d\n", getpid(), getppid());
			printf("fin du processus pere\n");

	}

	return pid;
}



int main()
{

	int status = 0;
	int semid;

	shmid_nbVoitures = shmget(IPC_PRIVATE, sizeof(int), 0666);
  	nbVoiture = (int *) shmat(shmid_nbVoitures, NULL, 0);
  	*nbVoiture = 0;	//initialisation à 0 du compteur
	

		pid_t pidSieges, pidTissus, pidFils, pidColoration, pidLivraison;

		pidSieges = atelierSieges();
/*
		pidTissus = atelierTissus();
		pidFils = atelierFils();
		pidColoration = atelierColoration();
		pidLivraison = livraison();
*/
		printf("Commande de voiture\n 1 voiture :25$\n 10 voitures : 250000£ \n 10000000 voitures : paiement nature\n");
		scanf("%d", nbVoiture);
		printf("%d voiture(s) commandée(s)\n",*nbVoiture);

		shmctl(shmid_nbVoitures, IPC_RMID, NULL);

		//int ret = semctl(semid, 0, IPC_RMID, ctl_arg);
		
}
