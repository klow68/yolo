#include <stdio.h>
#include <pthread.h>

# define TIMEPROD 


/* Déclaration des variables globales */
int nbPiecesAProduire, nbPostes;



/* Déclaration variables moniteur */
pthread_t *tab_ateliers;
pthread_mutex_t mutex;
pthread_cond_t produire, attendre;


/*                    */
void erreur(const char *msg)
{
		perror(msg);
		exit(1);
}

int main(int argc, char *argv[])
{

    if(argc - 1 == 2)
    {

      nbPostes = argv[1];
      nbPiecesAProduire = argv[2];

      tab_ateliers = (pthread_t *) malloc(nbPostes*sizeof(pthread_t));

    } else {
      erreur("Arguments : 1) Nombre de postes 2) Nombre de pièces à produire");
    }


}