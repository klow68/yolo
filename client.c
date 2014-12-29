#include <stdio.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <errno.h>


void erreur(const char *msg)
{
		perror(msg);
		exit(1);
}

int main()
{
		int nbVoiture=0;
		printf("Commande de voiture\n 1 voiture :25$\n 10 voitures : 250000£ \n 10000000 voitures : paiement nature\n");
		scanf("%d",&nbVoiture);
		printf("%d voiture(s) commandée(s)\n",nbVoiture);

		pid_t pid;

		switch (pid = fork()) {

				case (pid_t) 0:
						/* on est dans le processus fils */
						printf("valeur de fork = %d \n", pid);
						printf("je suis le processus fils %d de pere %d\n", getpid(), getppid());
						printf("fin du processus fils\n");
						exit(0);/* id		em */

				default:
						/* on est dans le processus pere */
						wait(NULL); // on attend qu'un processus fils meurt ou ne s'execute pas si plus de fils
						printf("valeur de fork = %d \n", pid);
						printf("je suis le processus pere %d et mon grand pere est : %d\n", getpid(), getppid());
						printf("fin du processus pere\n");
						exit(0);

		}
}
