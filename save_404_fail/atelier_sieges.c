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
#include <pthread.h>

#define SKEY (key_t) 1234
#define MAX_SIEGE_CONTENEUR 16


/* VARIABLES GLOBALES */
int shmid_nbVoitures;
int *nbVoiture;
int commande = 0;
int conteneur1;
int conteneur2;
/*                    */

void erreur(const char *msg)
{
		perror(msg);
		exit(1);
}

void traitant_SIGUSR(){
  commande = 1;
}

void* ouvrier(void* _d){
  int num = (int*) _d;
  printf("ouvrier %d\n",num);

  //si pas de travail attendre
  /*
  if(NbClientsAttente>0) 
    {
     // dit à un client de s'installer sur le fauteuil
            printf("poste %d attend du travail\n",num)
        pthread_cond_signal(&attendre);
            
    }
    //sinon travailler
  else       {
                         
    printf("le coiffeur dort car pas de clients \n");
            pthread_mutex_unlock(&mutex);
            pthread_cond_wait(&dormir, &mutex);
          ; // dit à un client de s'installer sur le fauteuil
            printf("le coiffeur se réveille\n");
    }
    */
}

int main(int argc, char *argv[])
{
  int i;
  pthread_t threads[10];

  signal(SIGUSR1,traitant_SIGUSR);
  
    if(argc - 1 == 1) {
      shmid_nbVoitures = atoi(argv[1]);
    } else {
      erreur("argument : id IPC");
    }

    
    //printf("\n[shmid %d, shmat %d]\n", shmid_nbVoitures, nbVoiture);

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

    /*********************création des ouvrier*************************/
// ouvrier = problème du coiffeur -> ouvrier = coiffeur, pièces a construire = clients

    // thread ouvrier




printf("Atelier sièges : OK\n");
kill(getppid(),SIGUSR1);


// Attend la commande du client
while (commande==0) {
}



  	//printf("\n[shmid %d, shmat %d]\n", shmid_nbVoitures, *nbVoiture);
    printf("(atelier_sieges) Nb voiture(s) commandée(s) : %d\n", *nbVoiture);
    if (*nbVoiture != 0){
      printf("\n\nYOU FUCKING WIN\n\n");
    }
    exit(0);

}
