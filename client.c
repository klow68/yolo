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
		printf("Commande de voiture\n 1 voiture :25$\n 10 voiture : 250000£ \n 10000000 voitures : payment nature\n");
		scanf("%d",&nbVoiture);
		printf("%d nb voiture commander",nbVoiture);	
}
